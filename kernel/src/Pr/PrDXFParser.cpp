#include <iostream>
#include <fstream>
#include "arxHeaders.h"
#include "kernel.h"
#include "DbObjectManager.h"
#include "RxWorkerThreadManager.h"
#include "PrDXFParser.h"
#include "AcDebug.h"
#include "AcMap.h"
#include "DbImpl.h"
#include "DbDatabaseImpl.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <string>


#define PRDXF_DECLARE_ACDBENTITY \
unsigned int handle = 0;\
int blockHandle = 0;\
std::string layerName;\
std::string linetypeName;\
int colorIndex = 256;\
Db::Visibility visibility = Db::Visibility::kVisible;\
Db::LineWeight lineWeight = Db::LineWeight::kLnWtByLwDefault;\


#define PRDXF_SET_ACDBENTITY(entity)\
entity->setColorIndex(colorIndex);\
entity->setVisibility(visibility);\
DbObjectId layerId = this->layerTableRecordMap->find(layerName);\
entity->setLayer(layerId);\
entity->setLineWeight(lineWeight);\
::setDbObjectHandle(entity, handle);\
PrObjectInidex* objectInidex = new PrObjectInidex(entity);\
objectInidex->parentHandle = blockHandle;\
this->objects.push_back(objectInidex);\


#define PRDXF_READ_ACDBOBJECT(next)\
if (isAcDbObject == true) {\
	switch (record->key) {\
	case 5:\
		handle = std::stoi(record->value, NULL, 16);\
		break;\
	case 330:\
		blockHandle = stol(record->value, NULL, 16);\
		break;\
	case 100:\
		isAcDbObject = false;\
		next = true;\
		break;\
	}\
}\


#define PRDXF_READ_ACDBENTITY(next)\
else if (isAcDbEntity == true) {\
	switch (record->key) {\
	case 8:\
		layerName = record->value;\
		break;\
	case 6:\
		linetypeName = record->value;\
		break;\
	case 60:\
		if (AcString::toInt32(record->value) != 0) {\
			visibility = Db::Visibility::kInvisible;\
		}\
		break;\
	case 62:\
		colorIndex = AcString::toInt32(record->value);\
		break;\
	case 370:\
		lineWeight = (Db::LineWeight)AcString::toInt32(record->value);\
		break;\
	case 100:\
		isAcDbObject = false;\
		isAcDbEntity = false;\
		next = true;\
		break;\
	}\
}\




static void readRecordValue(const char* content, PrDxfRecord* record)
{
	if (record->isValue == false)
	{
		record->isValue = true;
		for (int u = record->satrt; u < record->end; u++)
		{
			record->value += content[u];
		}
	}
}
static void setDbObjectHandle(DbObject* object, const unsigned int handle)
{
	DB_IMP_OBJECT(object->m_pImpl)->handle = handle;
}



class PrObjectInidex {
public:
	PrObjectInidex(DbObject* object) {
		this->object = object;
		this->parentHandle = 0;
	}

	DbObject* object;
	unsigned int parentHandle;
};
class PrDictionaryRecord {
public:
	PrDictionaryRecord() {
		this->handle = 0;
		this->parentHandle = 0;
		this->isAppend = false;
		this->dictionary = NULL;
		this->mapRecord = NULL;
	}
	virtual ~PrDictionaryRecord() {
		if (this->mapRecord != NULL) {
			delete this->mapRecord;
		}
	}

	bool isAppend;
	unsigned int handle;
	unsigned int parentHandle;
	DbDictionary* dictionary;
	AcMap<std::string, unsigned int>* mapRecord;
};

class PrDxfObjectRecord
{
public:
	PrDxfObjectRecord()
	{
		this->startPos = 0;
		this->endPos = 0;
		this->code = PrDxfRecord::kNone;
	}
	~PrDxfObjectRecord()
	{
	}

	int startPos;
	int endPos;
	PrDxfRecord::EDxfCode code;
};


class PrDXFParserImpl {
public:
	PrDXFParserImpl() {
		this->buffers = NULL;
		this->countByte = 0;
		this->nByte = 0;

		this->isHeaderSection = false;
		this->isTableSection = false;
		this->isBlockSection = false;
		this->isEntitySection = false;
		this->isObjectSection = false;
		this->tempStr = new char[1024 * 1024 * 10];

		this->database = NULL;
		this->curZeroRecord = 0;
	}
	virtual ~PrDXFParserImpl() {

	}


public:
	const char* buffers;
	int countByte;
	int nByte;
	char* tempStr;

	AcString textStyle;

	bool isHeaderSection;
	bool isTableSection;
	bool isBlockSection;
	bool isEntitySection;
	bool isObjectSection;

	int curZeroRecord;
	std::vector<PrDxfRecord*> records;
	std::vector<int> zeroRecords;

	DbDatabase* database;

	AcString defaultTextStyle;
	AcMap<std::string, DbObjectId>* layerTableRecordMap;
	AcMap<std::string, DbObjectId>* blockTableRecordMap;
	AcMap<std::string, DbObjectId>* textStyleTableRecordMap;
	AcMap<std::string, DbObjectId>* dimStyleTableRecordMap;
};


/** 计算记录 */
class PrDxfComputeRecordTask {
public:
	PrDxfComputeRecordTask() {
		this->startVal = 0;
		this->endVal = 0;
		this->content = NULL;
		this->isMainThread = false;
		this->isDone.store(false);
	}
	virtual ~PrDxfComputeRecordTask() {

	}

	int startVal;
	int endVal;
	const char* content;
	std::vector<int> wrapRecords;
	std::vector<PrDxfRecord*> records;
	std::vector<int> zeroRecordPos;
	bool isMainThread;
	std::atomic<bool> isDone;

	static void computeRecords(PrDxfComputeRecordTask* thread) {

		const char* buffers = thread->content;

		int start = thread->startVal;
		int end = thread->endVal;

		int charStart = 0;
		int charEnd = 0;
		char keyBuf[11];
		for (int i = start; i < end; i++)
		{
			if (i % 2 == 0)
			{
				if (i == 0)
				{
					charStart = 0;
				}
				else
				{
					charStart = thread->wrapRecords[i - 1];
				}
				charEnd = thread->wrapRecords[i];

				int num = charEnd - charStart;
				if (num < 10)
				{
					int index = 0;
					for (int u = charStart + 1; u < charEnd; u++)
					{
						keyBuf[index++] = buffers[u];
					}
					keyBuf[index++] = '\0';
					PrDxfRecord* record = new PrDxfRecord();
					record->key = std::stoi(keyBuf);
					record->satrt = charEnd + 1;
					if (i + 1 < thread->wrapRecords.size())
					{
						record->end = thread->wrapRecords[i + 1] - 1;
					}
					thread->records.push_back(record);

					if (record->key == 0)
					{
						thread->zeroRecordPos.push_back(thread->records.size() - 1);
					}
				}
			}
		}

		thread->isDone.store(true);
	}
};

/** 计算头段任务(一个线程一个任务) */
class PrDxfComputeHeaderTask {
public:
	PrDxfComputeHeaderTask() {
		this->isDone.store(false);
		this->content = NULL;
		this->isMainThread = false;
	}
	virtual ~PrDxfComputeHeaderTask() {

	}

	bool isMainThread;
	const char* content;
	std::vector<PrDxfRecord*> records;
	std::atomic<bool> isDone;

	static void computeHeadTask(PrDxfComputeHeaderTask* task, PrDXFParser* dxfParser)
	{
		int curRecord = 0;
		bool isTextStyle = false;
		std::vector<PrDxfRecord*> records = task->records;
		for (; curRecord < records.size(); curRecord++)
		{
			if (records[curRecord]->key != 9)
			{
				continue;
			}
			readRecordValue(task->content, records[curRecord]);

			if (isTextStyle == false && records[curRecord]->value == "$TEXTSTYLE") {

				isTextStyle = true;

				curRecord++;
				readRecordValue(task->content, records[curRecord]);

				dxfParser->m_pImpl->textStyle = records[curRecord]->value;
				//std::cout << "查询到文字样式:" << dxfParser->m_pImpl->textStyle.constPtr() << std::endl;
			}
		}

		task->isDone.store(true);
	}
};

/** 计算表段任务 */
class PrDxfComputeTableTask {
public:
	PrDxfComputeTableTask() {
		this->isDone.store(false);
		this->content = NULL;
		this->curRecord = 0;
		this->database = NULL;
		this->isMainThread = false;
	}
	virtual ~PrDxfComputeTableTask() {

	}

	bool isMainThread;
	const char* content;
	int curRecord;
	DbDatabase* database;
	std::vector<PrDxfRecord*> records;
	std::vector<PrObjectInidex*> objects;
	std::atomic<bool> isDone;

	void readDbViewportTableRecord()
	{

		DbViewportTableRecord* pViewportTableRecord = new DbViewportTableRecord();

		// 读取视口表记录数据
		unsigned int handle = 0;
		AcString name = "";
		double height = 1;
		double width = 1;
		double lensLength = 50;
		GePoint2d centerPoint;
		GePoint2d lowerLeftCorner;
		GePoint2d upperRightCorner;
		GePoint3d target;
		GeVector3d viewDirection;
		double frontClipDistance = 0;
		double backClipDistance = 0;
		double viewTwist = 0;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 2)
			{
				name = record->value;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 10)
			{
				lowerLeftCorner.x = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 20)
			{
				lowerLeftCorner.y = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 11)
			{
				upperRightCorner.x = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 21)
			{
				upperRightCorner.y = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 12)
			{
				centerPoint.x = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 22)
			{
				centerPoint.y = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 17)
			{
				target.x = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 27)
			{
				target.y = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 37)
			{
				target.z = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 40)
			{
				height = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 41)
			{
				width = height / AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 42)
			{
				lensLength = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 16)
			{
				viewDirection.x = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 26)
			{
				viewDirection.y = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 36)
			{
				viewDirection.z = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 43)
			{
				frontClipDistance = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 44)
			{
				backClipDistance = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 51)
			{
				viewTwist = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 280)
			{
			}

			this->curRecord++;
		};

		// 添加视口记录
		pViewportTableRecord->setName(name);
		pViewportTableRecord->setHeight(height);
		pViewportTableRecord->setWidth(width);
		pViewportTableRecord->setLensLength(lensLength);
		pViewportTableRecord->setCenterPoint(centerPoint);
		pViewportTableRecord->setLowerLeftCorner(lowerLeftCorner);
		pViewportTableRecord->setUpperRightCorner(upperRightCorner);
		pViewportTableRecord->setTarget(target);
		pViewportTableRecord->setViewDirection(viewDirection);
		pViewportTableRecord->setFrontClipDistance(frontClipDistance);
		pViewportTableRecord->setBackClipDistance(backClipDistance);
		pViewportTableRecord->setViewTwist(viewTwist);

		// 设置句柄
		::setDbObjectHandle(pViewportTableRecord, handle);

		// 添加到数组
		this->objects.push_back(new PrObjectInidex(pViewportTableRecord));
	}
	void readDbLinetypeTableRecord()
	{

		DbLinetypeTableRecord* pLinetypeTableRecord = new DbLinetypeTableRecord();

		// 读取线型表记录数据
		unsigned int handle = 0;
		AcString name = "";
		AcString comments = "";
		double patternLength = 0;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 2)
			{
				name = record->value;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 3)
			{
				comments = record->value;
			}
			else if (record->key == 40)
			{
				patternLength = AcString::toFloat64(record->value.c_str());
			}

			this->curRecord++;
		}

		// 添加线型记录
		pLinetypeTableRecord->setName(name);
		pLinetypeTableRecord->setComments(comments);
		pLinetypeTableRecord->setPatternLength(patternLength);

		// 设置句柄
		::setDbObjectHandle(pLinetypeTableRecord, handle);

		// 添加到数组
		this->objects.push_back(new PrObjectInidex(pLinetypeTableRecord));
	}
	void readDbLayerTableRecord()
	{

		DbLayerTableRecord* pLayerTableRecord = new DbLayerTableRecord();

		// 读取层表记录数据
		unsigned int handle = 0;
		AcString name = "";
		int colorIndex = 7;
		AcString linetype = "";
		bool isPrint = true;
		Db::LineWeight lineWeight = Db::LineWeight::kLnWtByLwDefault;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 2)
			{
				name = record->value;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 62)
			{
				colorIndex = AcString::toInt32(record->value);
			}
			else if (record->key == 6)
			{
				linetype = record->value;
			}
			else if (record->key == 290)
			{
				if (AcString::toInt32(record->value) == 0)
				{
					isPrint = false;
				}
				else
				{
					isPrint = true;
				}
			}
			else if (record->key == 370)
			{
				lineWeight = (Db::LineWeight)AcString::toInt32(record->value);
			}

			this->curRecord++;
		}

		// 获得线型id
		DbObjectId linetypeId = NULL;
		DbLinetypeTable* pLinetypeTable = NULL;
		this->database->getLinetypeTable(pLinetypeTable);
		if (pLinetypeTable != NULL)
		{
			pLinetypeTable->getAt(linetype, linetypeId);
			pLinetypeTable->close();
		}

		// 添加层表记录
		pLayerTableRecord->setName(name);
		CmColor color;
		color.setColorIndex(colorIndex);
		pLayerTableRecord->setColor(color);
		pLayerTableRecord->setLinetypeObjectId(linetypeId);
		pLayerTableRecord->setLineWeight(lineWeight);

		// 设置句柄
		::setDbObjectHandle(pLayerTableRecord, handle);

		// 添加到数组
		this->objects.push_back(new PrObjectInidex(pLayerTableRecord));
	}
	void readDbTextStyleTableRecord()
	{

		DbTextStyleTableRecord* pTextStyleTableRecord = new DbTextStyleTableRecord();

		// 读取文字样式表记录数据
		unsigned int handle = 0;
		AcString name = "";
		double textSize = 3.5;
		double widthFactor = 1;
		double obliquingAngle = 0;
		AcString fileName = "";
		AcString bigFontFileName = "";
		AcString pTypeface;
		Adesk::Boolean bold = false;
		Adesk::Boolean italic = false;
		Charset charset = Charset::kChineseSimpCharset;
		FontUtils::FontPitch pitch = FontUtils::FontPitch::kVariable;
		FontUtils::FontFamily family = FontUtils::FontFamily::kDecorative;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 2)
			{
				name = record->value;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 40)
			{
				textSize = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 41)
			{
				widthFactor = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 50)
			{
				obliquingAngle = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 71)
			{
			}
			else if (record->key == 3)
			{
				fileName = record->value;
			}
			else if (record->key == 4)
			{
				bigFontFileName = record->value;
			}
			else if (record->key == 1000) {
				pTypeface = record->value;
			}

			this->curRecord++;
		}

		// 添加文字样式记录
		pTextStyleTableRecord->setName(name);
		pTextStyleTableRecord->setTextSize(textSize);
		pTextStyleTableRecord->setXScale(widthFactor);
		pTextStyleTableRecord->setObliquingAngle(obliquingAngle);
		pTextStyleTableRecord->setFileName(fileName);
		pTextStyleTableRecord->setBigFontFileName(bigFontFileName);
		pTextStyleTableRecord->setFont(pTypeface, bold, italic, charset, pitch, family);

		// 设置句柄
		::setDbObjectHandle(pTextStyleTableRecord, handle);

		// 添加到数组
		this->objects.push_back(new PrObjectInidex(pTextStyleTableRecord));
	}
	void readDbViewTableRecord()
	{

		DbViewTableRecord* pViewTableRecord = new DbViewTableRecord();

		// 读取视图表记录数据
		unsigned int handle = 0;
		AcString name = "";
		double height = 1;
		double width = 1;
		double lensLength = 50;
		GePoint2d centerPoint;
		GePoint3d target;
		GeVector3d viewDirection = GeVector3d::kZAxis;
		double frontClipDistance = 0;
		double backClipDistance = 0;
		double viewTwist = 0;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 2)
			{
				name = record->value;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 12)
			{
				centerPoint.x = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 22)
			{
				centerPoint.y = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 17)
			{
				target.x = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 27)
			{
				target.y = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 37)
			{
				target.z = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 40)
			{
				height = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 41)
			{
				width = height / AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 42)
			{
				lensLength = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 16)
			{
				viewDirection.x = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 26)
			{
				viewDirection.y = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 36)
			{
				viewDirection.z = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 43)
			{
				frontClipDistance = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 44)
			{
				backClipDistance = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 51)
			{
				viewTwist = AcString::toFloat64(record->value.c_str());
			}
			else if (record->key == 280)
			{
			}

			this->curRecord++;
		}

		// 添加视图记录
		pViewTableRecord->setName(name);
		pViewTableRecord->setHeight(height);
		pViewTableRecord->setWidth(width);
		pViewTableRecord->setLensLength(lensLength);
		pViewTableRecord->setCenterPoint(centerPoint);
		pViewTableRecord->setTarget(target);
		pViewTableRecord->setViewDirection(viewDirection);
		pViewTableRecord->setFrontClipDistance(frontClipDistance);
		pViewTableRecord->setBackClipDistance(backClipDistance);
		pViewTableRecord->setViewTwist(viewTwist);

		// 设置句柄
		::setDbObjectHandle(pViewTableRecord, handle);

		// 添加到数组
		this->objects.push_back(new PrObjectInidex(pViewTableRecord));
	}
	void readDbUCSTableRecord()
	{

		DbUCSTableRecord* pUCSTableRecord = new DbUCSTableRecord();

		// 读取坐标系表记录数据
		unsigned int handle = 0;
		AcString name = "";
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 2)
			{
				name = record->value;
			}

			this->curRecord++;
		}

		// 添加坐标系记录
		pUCSTableRecord->setName(name);

		// 设置句柄
		::setDbObjectHandle(pUCSTableRecord, handle);

		// 添加到数组
		this->objects.push_back(new PrObjectInidex(pUCSTableRecord));
	}
	void readDimStyleTableRecord()
	{

		DbDimStyleTableRecord* pDimStyleTableRecord = new DbDimStyleTableRecord();

		// 读取表记录数据
		unsigned int handle = 0;
		AcString name = "";
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 2)
			{
				name = record->value;
				break;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}

			this->curRecord++;
		}

		// 添加块表记录
		pDimStyleTableRecord->setName(name);

		// 设置句柄
		::setDbObjectHandle(pDimStyleTableRecord, handle);

		// 添加到数组
		this->objects.push_back(new PrObjectInidex(pDimStyleTableRecord));
	}
	void readDbBlockTableRecord()
	{

		DbBlockTableRecord* pBlockTableRecord = new DbBlockTableRecord();

		// 读取表记录数据
		unsigned int handle = 0;
		unsigned int layoutHandle = 0;
		AcString name = "";
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 2)
			{
				name = record->value;
				break;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 340) {
				layoutHandle = std::stoi(record->value, NULL, 16);
			}

			this->curRecord++;
		}

		// 添加块表记录
		pBlockTableRecord->setName(name);

		// 设置句柄
		::setDbObjectHandle(pBlockTableRecord, handle);

		// 添加到数组
		this->objects.push_back(new PrObjectInidex(pBlockTableRecord));
	}
	static void computeTableRecord(PrDxfComputeTableTask* thread, PrDxfRecord::EDxfCode code)
	{
		for (; thread->curRecord < thread->records.size(); thread->curRecord++)
		{
			if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kViewTableRecord)
			{
				thread->readDbViewTableRecord();
				continue;
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kViewportTableRecord)
			{
				thread->readDbViewportTableRecord();
				continue;
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kLinetypeTableRecord)
			{
				thread->readDbLinetypeTableRecord();
				continue;
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kLayerTableRecord)
			{
				thread->readDbLayerTableRecord();
				continue;
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kTextStyleTableRecord)
			{
				thread->readDbTextStyleTableRecord();
				continue;
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kUCSTableRecord)
			{
				thread->readDbUCSTableRecord();
				continue;
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kBlockTableRecord)
			{
				thread->readDbBlockTableRecord();
				continue;
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kDimStyleTableRecord)
			{
				thread->readDimStyleTableRecord();
				continue;
			}
		}

		thread->isDone.store(true);
	}

};

/** 计算实体任务 */
class PrDxfComputeEntityTask {
public:
	PrDxfComputeEntityTask() {
		this->isDone.store(false);
		this->content = NULL;
		this->curRecord = 0;
		this->database = NULL;
		this->isMainThread = false;

		this->layerTableRecordMap = NULL;
		this->blockTableRecordMap = NULL;
		this->textStyleTableRecordMap = NULL;
		this->dimStyleTableRecordMap = NULL;
	}
	virtual ~PrDxfComputeEntityTask() {

	}

	bool isMainThread;
	const char* content;
	int curRecord;
	DbDatabase* database;
	std::vector<PrDxfRecord*> records;
	std::vector<PrObjectInidex*> objects;
	std::atomic<bool> isDone;

	AcString defaultTextStyle;
	AcMap<std::string, DbObjectId>* layerTableRecordMap;
	AcMap<std::string, DbObjectId>* blockTableRecordMap;
	AcMap<std::string, DbObjectId>* textStyleTableRecordMap;
	AcMap<std::string, DbObjectId>* dimStyleTableRecordMap;

	void computePoint()
	{

		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d position;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbPoint = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbPoint)
			else if (isAcDbPoint == true) {
				switch (record->key) {
				case 10:
					position.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					position.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					position.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}

		DbPoint* pEntity = new DbPoint();
		pEntity->setPosition(position);
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeLine()
	{

		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d startPoint;
		GePoint3d endPoint;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbLine = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbLine)
			else if (isAcDbLine == true) {
				switch (record->key) {
				case 10:
					startPoint.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					startPoint.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					startPoint.z = AcString::toFloat64(record->value.c_str());
					break;
				case 11:
					endPoint.x = AcString::toFloat64(record->value.c_str());
					break;
				case 21:
					endPoint.y = AcString::toFloat64(record->value.c_str());
					break;
				case 31:
					endPoint.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}

		DbLine* pEntity = new DbLine();
		pEntity->setStartPoint(startPoint);
		pEntity->setEndPoint(endPoint);
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeXline()
	{

		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d basePoint;
		GeVector3d unitDir;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbXline = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbXline)
			else if (isAcDbXline == true) {
				switch (record->key)
				{
				case 10:
					basePoint.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					basePoint.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					basePoint.z = AcString::toFloat64(record->value.c_str());
					break;
				case 11:
					unitDir.x = AcString::toFloat64(record->value.c_str());
					break;
				case 21:
					unitDir.y = AcString::toFloat64(record->value.c_str());
					break;
				case 31:
					unitDir.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}

		DbXline* pEntity = new DbXline();
		pEntity->setBasePoint(basePoint);
		pEntity->setUnitDir(unitDir);
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeRay()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d basePoint;
		GeVector3d unitDir;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbRay = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbRay)
			else if (isAcDbRay == true) {
				switch (record->key)
				{
				case 10:
					basePoint.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					basePoint.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					basePoint.z = AcString::toFloat64(record->value.c_str());
					break;
				case 11:
					unitDir.x = AcString::toFloat64(record->value.c_str());
					break;
				case 21:
					unitDir.y = AcString::toFloat64(record->value.c_str());
					break;
				case 31:
					unitDir.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}

		DbRay* pEntity = new DbRay();
		pEntity->setBasePoint(basePoint);
		pEntity->setUnitDir(unitDir);
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeCircle()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d center;
		double radius = 0;
		GeVector3d normal = GeVector3d::kZAxis;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbCircle = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbCircle)
			else if (isAcDbCircle == true) {
				switch (record->key)
				{
				case 10:
					center.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					center.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					center.z = AcString::toFloat64(record->value.c_str());
					break;
				case 40:
					radius = AcString::toFloat64(record->value.c_str());
					break;
				case 210:
					normal.x = AcString::toFloat64(record->value.c_str());
					break;
				case 220:
					normal.y = AcString::toFloat64(record->value.c_str());
					break;
				case 230:
					normal.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}

		DbCircle* pEntity = new DbCircle();
		pEntity->setCenter(center);
		pEntity->setRadius(radius);
		pEntity->setNormal(normal);
		if (normal.isEqualTo(GeVector3d::kZAxis) == true) {
			pEntity->setCenter(center);
		}
		else {
			GeMatrix3d mat;
			mat.setToPlaneToWorld(normal);
			pEntity->setCenter(center.transformBy(mat));
		}
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeArc()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d center;
		double radius = 0;
		GeVector3d normal = GeVector3d::kZAxis;
		double startAngle = 0;
		double endAngle = 0;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbArc = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbArc)
			else if (isAcDbArc == true) {
				switch (record->key)
				{
				case 10:
					center.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					center.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					center.z = AcString::toFloat64(record->value.c_str());
					break;
				case 40:
					radius = AcString::toFloat64(record->value.c_str());
					break;
				case 50:
					startAngle = AcString::toFloat64(record->value.c_str());
					break;
				case 51:
					endAngle = AcString::toFloat64(record->value.c_str());
					break;
				case 210:
					normal.x = AcString::toFloat64(record->value.c_str());
					break;
				case 220:
					normal.y = AcString::toFloat64(record->value.c_str());
					break;
				case 230:
					normal.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}

		// 创建实体
		DbArc* pEntity = new DbArc();
		pEntity->setCenter(center);
		pEntity->setRadius(radius);
		pEntity->setNormal(normal);
		pEntity->setStartAngle(angleToRadian(startAngle));
		pEntity->setEndAngle(angleToRadian(endAngle));
		if (normal.isEqualTo(GeVector3d::kZAxis) == true) {
			pEntity->setCenter(center);
		}
		else {
			GeMatrix3d mat;
			mat.setToPlaneToWorld(normal);
			pEntity->setCenter(center.transformBy(mat));
		}
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computePolyline() {

		PRDXF_DECLARE_ACDBENTITY;
		bool isClosed = false;
		double globalWidth = 0;
		double startWidth = 0;
		double endWidth = 0;
		double bulge = 0;
		double elevation = 0.0;
		GePoint2d vertex = GePoint2d::kOrigin;
		GePoint2dArray vertexs;
		GeDoubleArray bulges;
		GeDoubleArray startWidths;
		GeDoubleArray endWidths;
		GeVector3d normal = GeVector3d::kZAxis;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbPolyline = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbPolyline)
			else if (isAcDbPolyline == true) {
				switch (record->key)
				{

				case 38:
					elevation = AcString::toFloat64(record->value.c_str());
					break;
				case 70:
					if (AcString::toInt32(record->value) == 1)
					{
						isClosed = true;
					}
					break;
				case 40:
					startWidth = AcString::toFloat64(record->value.c_str());
					break;
				case 41:
					endWidth = AcString::toFloat64(record->value.c_str());
					break;
				case 42:
					bulge = AcString::toFloat64(record->value.c_str());
					break;
				case 43:
					globalWidth = AcString::toFloat64(record->value.c_str());
					break;
				case 10:
					vertex = GePoint2d::kOrigin;
					vertex.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					vertex.y = AcString::toFloat64(record->value.c_str());

					if (abs(startWidth) < 0.00001 && abs(endWidth) < 0.00001)
					{
						startWidth = globalWidth;
						endWidth = globalWidth;
					}

					bulges.append(bulge);
					startWidths.append(startWidth);
					endWidths.append(endWidth);
					vertexs.append(vertex);

					bulge = 0;
					startWidth = 0;
					endWidth = 0;
					break;
				case 210:
					normal.x = AcString::toFloat64(record->value.c_str());
					break;
				case 220:
					normal.y = AcString::toFloat64(record->value.c_str());
					break;
				case 230:
					normal.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}
		bulges.append(0.0);

		DbPolyline* pEntity = new DbPolyline();
		for (int i = 0; i < vertexs.length(); i++) {
			pEntity->addVertexAt(i, vertexs[i], bulges[i + 1], startWidths[i], endWidths[i]);
		}
		pEntity->setElevation(elevation);
		pEntity->setNormal(normal.normalize());
		pEntity->setClosed(isClosed);
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeSolid() {

		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d vertex = GePoint3d::kOrigin;
		GePoint3dArray vertexs;
		double thickness = 0;
		GeVector3d normal = GeVector3d::kZAxis;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbSolid = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbSolid)
			else if (isAcDbSolid == true) {
				switch (record->key)
				{
				case 10:
					vertex = GePoint3d::kOrigin;
					vertex.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					vertex.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					vertex.z = AcString::toFloat64(record->value.c_str());
					vertexs.append(vertex);
					break;
				case 11:
					vertex = GePoint3d::kOrigin;
					vertex.x = AcString::toFloat64(record->value.c_str());
					break;
				case 21:
					vertex.y = AcString::toFloat64(record->value.c_str());
					break;
				case 31:
					vertex.z = AcString::toFloat64(record->value.c_str());
					vertexs.append(vertex);
					break;
				case 12:
					vertex = GePoint3d::kOrigin;
					vertex.x = AcString::toFloat64(record->value.c_str());
					break;
				case 22:
					vertex.y = AcString::toFloat64(record->value.c_str());
					break;
				case 32:
					vertex.z = AcString::toFloat64(record->value.c_str());
					vertexs.append(vertex);
					break;
				case 13:
					vertex = GePoint3d::kOrigin;
					vertex.x = AcString::toFloat64(record->value.c_str());
					break;
				case 23:
					vertex.y = AcString::toFloat64(record->value.c_str());
					break;
				case 33:
					vertex.z = AcString::toFloat64(record->value.c_str());
					vertexs.append(vertex);
					break;
				case 39:
					thickness = AcString::toFloat64(record->value.c_str());
					break;
				case 210:
					normal.x = AcString::toFloat64(record->value.c_str());
					break;
				case 220:
					normal.y = AcString::toFloat64(record->value.c_str());
					break;
				case 230:
					normal.z = AcString::toFloat64(record->value.c_str());
					break;

				};
			}

			this->curRecord++;
		}

		// 创建实体
		DbSolid* pEntity = new DbSolid();
		pEntity->setNormal(normal);
		for (int i = 0; i < vertexs.length(); i++)
		{
			pEntity->setPointAt(i, vertexs[i]);
		}
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeViewport() {

		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d centerPoint = GePoint3d::kOrigin;
		short number = 1;
		double width = 0.0;
		double height = 0.0;
		GePoint2d viewCenter = GePoint2d::kOrigin;
		GePoint2d snapBasePoint = GePoint2d::kOrigin;
		GeVector2d snapIncrement = GeVector2d(10, 10);
		GeVector2d gridIncrement = GeVector2d(10, 10);
		GeVector3d viewDirection = GeVector3d(0, 0, 1);
		GePoint3d viewTarget = GePoint3d::kOrigin;
		double lensLength = 50.0;
		double viewHeight = 0.0;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbViewport = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbViewport)
			else if (isAcDbViewport == true) {

				switch (record->key)
				{
				case 10:
					centerPoint.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					centerPoint.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					centerPoint.z = AcString::toFloat64(record->value.c_str());
					break;
				case 40:
					width = AcString::toFloat64(record->value.c_str());
					break;
				case 41:
					height = AcString::toFloat64(record->value.c_str());
					break;
				case 12:
					viewCenter.x = AcString::toFloat64(record->value.c_str());
					break;
				case 22:
					viewCenter.y = AcString::toFloat64(record->value.c_str());
					break;
				case 13:
					snapBasePoint.x = AcString::toFloat64(record->value.c_str());
					break;
				case 23:
					snapBasePoint.y = AcString::toFloat64(record->value.c_str());
					break;
				case 14:
					snapIncrement.x = AcString::toFloat64(record->value.c_str());
					break;
				case 24:
					snapIncrement.y = AcString::toFloat64(record->value.c_str());
					break;
				case 15:
					gridIncrement.x = AcString::toFloat64(record->value.c_str());
					break;
				case 25:
					gridIncrement.y = AcString::toFloat64(record->value.c_str());
					break;
				case 16:
					viewDirection.x = AcString::toFloat64(record->value.c_str());
					break;
				case 26:
					viewDirection.y = AcString::toFloat64(record->value.c_str());
					break;
				case 36:
					viewDirection.z = AcString::toFloat64(record->value.c_str());
					break;
				case 17:
					viewTarget.x = AcString::toFloat64(record->value.c_str());
					break;
				case 27:
					viewTarget.y = AcString::toFloat64(record->value.c_str());
					break;
				case 37:
					viewTarget.z = AcString::toFloat64(record->value.c_str());
					break;
				case 42:
					lensLength = AcString::toFloat64(record->value.c_str());
					break;
				case 45:
					viewHeight = AcString::toFloat64(record->value.c_str());
					break;
				case 69:
					number = AcString::toInt32(record->value);
					break;
				}
			}

			this->curRecord++;
		}

		// 创建实体
		DbViewport* pEntity = new DbViewport();
		pEntity->setCenterPoint(centerPoint);
		pEntity->setWidth(width);
		pEntity->setHeight(height);
		pEntity->setViewCenter(viewCenter);
		pEntity->setSnapBasePoint(snapBasePoint);
		pEntity->setSnapIncrement(snapIncrement);
		pEntity->setGridIncrement(gridIncrement);
		pEntity->setViewDirection(viewDirection);
		pEntity->setViewTarget(viewTarget);
		pEntity->setLensLength(lensLength);
		pEntity->setViewHeight(viewHeight);
		DB_IMP_VIEWPORT(pEntity->m_pImpl)->number = number;
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeText()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d position;
		double height = 0;
		double widthFactor = 1;
		AcString textString;
		std::string textStyle = "";
		GeVector3d normal = GeVector3d::kZAxis;
		double rotation = 0.0;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbText = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbText)
			else if (isAcDbText == true) {
				switch (record->key)
				{
				case 1:
					textString = record->value;
					break;
				case 7:
					textStyle = record->value;
					break;
				case 50:
					rotation = AcString::toFloat64(record->value.c_str());
					break;
				case 10:
					position.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					position.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					position.z = AcString::toFloat64(record->value.c_str());
					break;
				case 40:
					height = AcString::toFloat64(record->value.c_str());
					break;
				case 41:
					widthFactor = AcString::toFloat64(record->value.c_str());
					break;
				case 210:
					normal.x = AcString::toFloat64(record->value.c_str());
					break;
				case 220:
					normal.y = AcString::toFloat64(record->value.c_str());
					break;
				case 230:
					normal.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}

		DbText* pEntity = new DbText();
		pEntity->setWidthFactor(widthFactor);
		pEntity->setHeight(height);
		pEntity->setNormal(normal);
		pEntity->setRotation(rotation * PI / 180.0);
		pEntity->setTextString(textString);
		if (textStyle.length() == 0) {
			textStyle = this->defaultTextStyle;
		}
		DbObjectId textStyleId = this->textStyleTableRecordMap->find(textStyle);
		pEntity->setTextStyle(textStyleId);
		if (normal.isEqualTo(GeVector3d::kZAxis) == true) {
			pEntity->setPosition(position);
		}
		else {
			GeMatrix3d mat;
			mat.setToPlaneToWorld(normal);
			pEntity->setPosition(position.transformBy(mat));
		}
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeMText()
	{
		PRDXF_DECLARE_ACDBENTITY;
		AcString contents;
		GePoint3d location;
		double height = 0.0;
		double textHeight = 0.0;
		std::string textStyle;
		GeVector3d normal(0, 0, 1);
		double rotation = 0.0;;
		DbMText::AttachmentPoint attachmentPoint = DbMText::AttachmentPoint::kTopLeft;
		DbMText::FlowDirection flowDirection = DbMText::FlowDirection::kByStyle;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbMText = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbMText)
			else if (isAcDbMText == true) {

				switch (record->key)
				{
				case 1:
					contents = record->value;
					break;
				case 7:
					textStyle = record->value;
					break;
				case 10:
					location.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					location.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					location.z = AcString::toFloat64(record->value.c_str());
					break;
				case 40:
					textHeight = AcString::toFloat64(record->value.c_str());
					break;
				case 50:
					rotation = AcString::toFloat64(record->value.c_str()) / 180.0 * PI;
					break;
				case 41:
					break;
				case 71:
					attachmentPoint = (DbMText::AttachmentPoint)AcString::toInt32(record->value);
					break;
				case 72:
					flowDirection = (DbMText::FlowDirection)AcString::toInt32(record->value);
					break;
				case 210:
					normal.x = AcString::toFloat64(record->value.c_str());
					break;
				case 220:
					normal.y = AcString::toFloat64(record->value.c_str());
					break;
				case 230:
					normal.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}

		DbMText* pEntity = new DbMText();
		pEntity->setContents(contents);
		pEntity->setHeight(height);
		pEntity->setTextHeight(textHeight);
		pEntity->setAttachment(attachmentPoint);
		pEntity->setFlowDirection(flowDirection);
		pEntity->setNormal(normal);
		pEntity->setRotation(rotation * PI / 180.0);
		if (normal.isEqualTo(GeVector3d::kZAxis) == true) {
			pEntity->setLocation(location);
		}
		else {
			GeMatrix3d mat;
			mat.setToPlaneToWorld(normal);
			pEntity->setLocation(location.transformBy(mat));
		}
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeBlockReference()
	{
		PRDXF_DECLARE_ACDBENTITY;
		std::string blockName;
		GePoint3d position = GePoint3d::kOrigin;
		GeVector3d normal = GeVector3d::kZAxis;
		GeScale3d scale = GeScale3d::kIdentity;
		double rotation = 0;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbBlockReference = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbBlockReference)
			else if (isAcDbBlockReference == true) {
				switch (record->key)
				{
				case 2:
					blockName = record->value;
					break;
				case 10:
					position.x = AcString::toFloat64(record->value.c_str());
					break;
				case 20:
					position.y = AcString::toFloat64(record->value.c_str());
					break;
				case 30:
					position.z = AcString::toFloat64(record->value.c_str());
					break;
				case 41:
					scale.sx = AcString::toFloat64(record->value.c_str());
					break;
				case 42:
					scale.sy = AcString::toFloat64(record->value.c_str());
					break;
				case 43:
					scale.sz = AcString::toFloat64(record->value.c_str());
					break;
				case 50:
					rotation = AcString::toFloat64(record->value.c_str());
					break;
				case 210:
					normal.x = AcString::toFloat64(record->value.c_str());
					break;
				case 220:
					normal.y = AcString::toFloat64(record->value.c_str());
					break;
				case 230:
					normal.z = AcString::toFloat64(record->value.c_str());
					break;
				};
			}

			this->curRecord++;
		}

		DbBlockReference* pEntity = new DbBlockReference();
		DbObjectId blockTableRecordId = this->blockTableRecordMap->find(blockName);
		pEntity->setBlockTableRecord(blockTableRecordId);
		pEntity->setPosition(position);
		pEntity->setNormal(normal);
		pEntity->setRotation(angleToRadian(rotation));
		pEntity->setScaleFactors(scale);
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeDimension() {

		PRDXF_DECLARE_ACDBENTITY;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbDimension = false;
		bool isAcDbAlignedDimension = false;
		bool isAcDbRotatedDimension = false;
		bool isAcDbArcDimension = false;
		bool isAcDbOrdinateDimension = false;
		bool isAcDbRadialDimension = false;
		bool isAcDbRadialDimensionLarge = false;
		bool isAcDbDiametricDimension = false;
		bool isAcDb2LineAngularDimension = false;
		bool isAcDb3PointAngularDimension = false;
		std::string blockName;
		GePoint3d textPosition;
		DbMText::AttachmentPoint textAttachment = DbMText::AttachmentPoint::kMiddleCenter;
		double measurement = 0.0;
		AcString dimStyleName;
		GePoint3d xLine1Point;
		GePoint3d xLine2Point;
		GePoint3d leader1Point;
		GePoint3d leader2Point;
		double rotation = 0.0;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 100 && record->value == "AcDbDimension") {
				isAcDbDimension = true;
				isAcDbAlignedDimension = false;
				isAcDbRotatedDimension = false;
				isAcDbArcDimension = false;
				isAcDbOrdinateDimension = false;
				isAcDbRadialDimension = false;
				isAcDbRadialDimensionLarge = false;
				isAcDbDiametricDimension = false;
				isAcDb2LineAngularDimension = false;
				isAcDb3PointAngularDimension = false;
			}
			else if (record->key == 100 && record->value == "AcDbAlignedDimension") {
				isAcDbDimension = false;
				isAcDbAlignedDimension = true;
				isAcDbRotatedDimension = false;
				isAcDbArcDimension = false;
				isAcDbOrdinateDimension = false;
				isAcDbRadialDimension = false;
				isAcDbRadialDimensionLarge = false;
				isAcDbDiametricDimension = false;
				isAcDb2LineAngularDimension = false;
				isAcDb3PointAngularDimension = false;
			}
			else if (record->key == 100 && record->value == "AcDbRotatedDimension") {
				isAcDbDimension = false;
				isAcDbAlignedDimension = false;
				isAcDbRotatedDimension = true;
				isAcDbArcDimension = false;
				isAcDbOrdinateDimension = false;
				isAcDbRadialDimension = false;
				isAcDbRadialDimensionLarge = false;
				isAcDbDiametricDimension = false;
				isAcDb2LineAngularDimension = false;
				isAcDb3PointAngularDimension = false;
			}
			else if (record->key == 100 && record->value == "AcDbArcDimension") {
				isAcDbDimension = false;
				isAcDbAlignedDimension = false;
				isAcDbRotatedDimension = false;
				isAcDbArcDimension = true;
				isAcDbOrdinateDimension = false;
				isAcDbRadialDimension = false;
				isAcDbRadialDimensionLarge = false;
				isAcDbDiametricDimension = false;
				isAcDb2LineAngularDimension = false;
				isAcDb3PointAngularDimension = false;
			}
			else if (record->key == 100 && record->value == "AcDbOrdinateDimension") {
				isAcDbDimension = false;
				isAcDbAlignedDimension = false;
				isAcDbRotatedDimension = false;
				isAcDbArcDimension = false;
				isAcDbOrdinateDimension = true;
				isAcDbRadialDimension = false;
				isAcDbRadialDimensionLarge = false;
				isAcDbDiametricDimension = false;
				isAcDb2LineAngularDimension = false;
				isAcDb3PointAngularDimension = false;
			}
			else if (record->key == 100 && record->value == "AcDbRadialDimension") {
				isAcDbDimension = false;
				isAcDbAlignedDimension = false;
				isAcDbRotatedDimension = false;
				isAcDbArcDimension = false;
				isAcDbOrdinateDimension = false;
				isAcDbRadialDimension = true;
				isAcDbRadialDimensionLarge = false;
				isAcDbDiametricDimension = false;
				isAcDb2LineAngularDimension = false;
				isAcDb3PointAngularDimension = false;
			}
			else if (record->key == 100 && record->value == "AcDbRadialDimensionLarge") {
				isAcDbDimension = false;
				isAcDbAlignedDimension = false;
				isAcDbRotatedDimension = false;
				isAcDbArcDimension = false;
				isAcDbOrdinateDimension = false;
				isAcDbRadialDimension = false;
				isAcDbRadialDimensionLarge = true;
				isAcDbDiametricDimension = false;
				isAcDb2LineAngularDimension = false;
				isAcDb3PointAngularDimension = false;
			}
			else if (record->key == 100 && record->value == "AcDbDiametricDimension") {
				isAcDbDimension = false;
				isAcDbAlignedDimension = false;
				isAcDbRotatedDimension = false;
				isAcDbArcDimension = false;
				isAcDbOrdinateDimension = false;
				isAcDbRadialDimension = false;
				isAcDbRadialDimensionLarge = false;
				isAcDbDiametricDimension = true;
				isAcDb2LineAngularDimension = false;
				isAcDb3PointAngularDimension = false;
			}
			else if (record->key == 100 && record->value == "AcDb2LineAngularDimension") {
				isAcDbDimension = false;
				isAcDbAlignedDimension = false;
				isAcDbRotatedDimension = false;
				isAcDbArcDimension = false;
				isAcDbOrdinateDimension = false;
				isAcDbRadialDimension = false;
				isAcDbRadialDimensionLarge = false;
				isAcDbDiametricDimension = false;
				isAcDb2LineAngularDimension = true;
				isAcDb3PointAngularDimension = false;
			}
			else if (record->key == 100 && record->value == "AcDb3PointAngularDimension") {
				isAcDbDimension = false;
				isAcDbAlignedDimension = false;
				isAcDbRotatedDimension = false;
				isAcDbArcDimension = false;
				isAcDbOrdinateDimension = false;
				isAcDbRadialDimension = false;
				isAcDbRadialDimensionLarge = false;
				isAcDbDiametricDimension = false;
				isAcDb2LineAngularDimension = false;
				isAcDb3PointAngularDimension = true;
			}
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
				PRDXF_READ_ACDBENTITY(isAcDbDimension)
				/* AcDbDimension */
			else if (isAcDbDimension == true) {
				switch (record->key)
				{
				case 2:
					blockName = record->value;
					break;
				case 10:
					break;
				case 20:
					break;
				case 30:
					break;
				case 11:
					textPosition.x = AcString::toFloat64(record->value.c_str());
					break;
				case 21:
					textPosition.y = AcString::toFloat64(record->value.c_str());
					break;
				case 31:
					textPosition.z = AcString::toFloat64(record->value.c_str());
					break;
				case 70:
					break;
				case 71:
					textAttachment = (DbMText::AttachmentPoint)AcString::toInt32(record->value);
					break;
				case 42:
					measurement = AcString::toFloat64(record->value.c_str());
					break;
				case 3:
					dimStyleName = record->value;
					break;
				}
			}
			/* AcDbAlignedDimension */
			else if (isAcDbAlignedDimension == true) {
				switch (record->key)
				{
				case 13:
					xLine1Point.x = AcString::toFloat64(record->value.c_str());
					break;
				case 23:
					xLine1Point.y = AcString::toFloat64(record->value.c_str());
					break;
				case 33:
					xLine1Point.z = AcString::toFloat64(record->value.c_str());
					break;
				case 14:
					xLine2Point.x = AcString::toFloat64(record->value.c_str());
					break;
				case 24:
					xLine2Point.y = AcString::toFloat64(record->value.c_str());
					break;
				case 34:
					xLine2Point.z = AcString::toFloat64(record->value.c_str());
					break;
				case 50:
					rotation = AcString::toFloat64(record->value.c_str());
					break;
				}
			}
			/* AcDbRotatedDimension */
			else if (isAcDbRotatedDimension == true) {

			}
			/* AcDbArcDimension */
			else if (isAcDbArcDimension == true) {
				switch (record->key)
				{
				case 16:
					leader1Point.x = AcString::toFloat64(record->value.c_str());
					break;
				case 26:
					leader1Point.y = AcString::toFloat64(record->value.c_str());
					break;
				case 36:
					leader1Point.z = AcString::toFloat64(record->value.c_str());
					break;
				case 17:
					leader2Point.x = AcString::toFloat64(record->value.c_str());
					break;
				case 27:
					leader2Point.y = AcString::toFloat64(record->value.c_str());
					break;
				case 37:
					leader2Point.z = AcString::toFloat64(record->value.c_str());
					break;
				};

				this->curRecord++;
			}

			this->curRecord++;
		};

		DbDimension* pEntity = NULL;
		if (isAcDbAlignedDimension == true) {
			pEntity = new DbAlignedDimension();
			((DbAlignedDimension*)(pEntity))->setXLine1Point(xLine1Point);
			((DbAlignedDimension*)(pEntity))->setXLine2Point(xLine2Point);
		}
		else if (isAcDbRotatedDimension == true) {
			pEntity = new DbRotatedDimension();
			((DbRotatedDimension*)(pEntity))->setRotation(rotation / 180.0 * PI);
			((DbRotatedDimension*)(pEntity))->setXLine1Point(xLine1Point);
			((DbRotatedDimension*)(pEntity))->setXLine2Point(xLine2Point);
		}
		else if (isAcDbArcDimension == true) {
			pEntity = new DbArcDimension();
			((DbArcDimension*)(pEntity))->setXLine1Point(xLine1Point);
			((DbArcDimension*)(pEntity))->setXLine2Point(xLine2Point);
			((DbArcDimension*)(pEntity))->setLeader1Point(leader1Point);
			((DbArcDimension*)(pEntity))->setLeader2Point(leader2Point);
		}
		else if (isAcDbOrdinateDimension == true) {
			pEntity = new DbOrdinateDimension();
		}
		else if (isAcDbRadialDimension == true) {
			pEntity = new DbRadialDimension();
		}
		else if (isAcDbRadialDimensionLarge == true) {
			pEntity = new DbRadialDimensionLarge();
		}
		else if (isAcDbDiametricDimension == true) {
			pEntity = new DbDiametricDimension();
		}
		else if (isAcDb2LineAngularDimension == true) {
			pEntity = new Db2LineAngularDimension();
		}
		else if (isAcDb3PointAngularDimension == true) {
			pEntity = new Db3PointAngularDimension();
		}
		if (pEntity == NULL) {
			return;
		}

		DbObjectId dimBlockId = this->blockTableRecordMap->find(blockName);
		pEntity->setDimBlockId(dimBlockId);
		PRDXF_SET_ACDBENTITY(pEntity);
	}
	void computeArcDimension() {
		this->computeDimension();
	}
	void computeRadialDimensionLarge() {
		this->computeDimension();
	}
	static void computeEntity(PrDxfComputeEntityTask* thread)
	{
		for (; thread->curRecord < thread->records.size(); thread->curRecord++)
		{
			if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kPoint)
			{
				thread->computePoint();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kLine)
			{
				thread->computeLine();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kXLine)
			{
				thread->computeXline();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kRay)
			{
				thread->computeRay();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kCircle)
			{
				thread->computeCircle();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kText)
			{
				thread->computeText();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kMText)
			{
				thread->computeMText();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kArc)
			{
				thread->computeArc();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kBlockReference)
			{
				thread->computeBlockReference();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kDimension)
			{
				thread->computeDimension();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kArcDimension)
			{
				thread->computeArcDimension();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kRadialDimensionLarge)
			{
				thread->computeRadialDimensionLarge();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kPolyline)
			{
				thread->computePolyline();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kSolid)
			{
				thread->computeSolid();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kViewport)
			{
				thread->computeViewport();
			}
		}

		thread->isDone.store(true);
	}
};

/** 计算块段任务 */
class PrDxfComputeBlockTask :public PrDxfComputeEntityTask {
public:
	PrDxfComputeBlockTask() {

	}
	virtual ~PrDxfComputeBlockTask() {

	}

};

/** 计算对象任务 */
class PrDxfComputeObjectTask {
public:
	PrDxfComputeObjectTask() {
		this->isDone.store(false);
		this->content = NULL;
		this->curRecord = 0;
		this->database = NULL;
		this->isMainThread = false;
	}
	virtual ~PrDxfComputeObjectTask() {

	}

	bool isMainThread;
	const char* content;
	int curRecord;
	DbDatabase* database;
	std::vector<PrDxfRecord*> records;
	std::vector<PrObjectInidex*> objects;
	std::atomic<bool> isDone;

	AcArray<PrDictionaryRecord*> dictionaryIndexs;

	void readDbDictionary() {

		unsigned int handle = 0;
		unsigned int parentHandle = 0;
		std::string ownerName;
		unsigned int ownerHandle = 0;
		AcMap<std::string, unsigned int>* map = new AcMap<std::string, unsigned int>();
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 330)
			{
				parentHandle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 3)
			{
				ownerName = record->value;
			}
			else if (record->key == 350)
			{
				ownerHandle = std::stoi(record->value, NULL, 16);
				map->insert(ownerName, ownerHandle);
			}

			this->curRecord++;
		}

		DbDictionary* pDictionary = new DbDictionary();
		::setDbObjectHandle(pDictionary, handle);
		PrDictionaryRecord* dictionaryIndex = new PrDictionaryRecord();
		dictionaryIndex->dictionary = pDictionary;
		dictionaryIndex->handle = handle;
		dictionaryIndex->parentHandle = parentHandle;
		dictionaryIndex->mapRecord = map;
		this->dictionaryIndexs.append(dictionaryIndex);
	}
	void readDbLayout() {

		unsigned int handle = 0;
		unsigned int parentHandle = 0;
		unsigned int blockHandle = 0;
		unsigned int viewHandle = 0;
		AcString layoutName;
		bool isAcDbLayout = false;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 330 && isAcDbLayout == false)
			{
				parentHandle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 330 && isAcDbLayout == true)
			{
				blockHandle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 331 && isAcDbLayout == true)
			{
				viewHandle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 1)
			{
				layoutName = record->value;
			}
			else if (record->key == 100 && record->value == "AcDbLayout")
			{
				isAcDbLayout = true;
			}

			this->curRecord++;
		}

		DbLayout* pLayout = new DbLayout();
		pLayout->setLayoutName(layoutName);

		DbObjectId blockTableRecordId = NULL;
		this->database->getDbObjectId(blockTableRecordId, false, blockHandle);
		DB_IMP_LAYOUT(pLayout->m_pImpl)->blockTableRecordId = blockTableRecordId;

		DbObjectId viewportId = NULL;
		if (viewHandle > 0) {
			this->database->getDbObjectId(viewportId, false, viewHandle);
			DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId = viewportId;
		}
		::setDbObjectHandle(pLayout, handle);

		// 添加到数组
		PrObjectInidex* objectInidex = new PrObjectInidex(pLayout);
		objectInidex->parentHandle = parentHandle;
		this->objects.push_back(objectInidex);
	}
	void readDbXdata() {

		unsigned int handle = 0;
		unsigned int parentHandle = 0;
		while (this->curRecord < this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark)
			{
				break;
			}
			else if (record->key == 5)
			{
				handle = std::stoi(record->value, NULL, 16);
			}
			else if (record->key == 330)
			{
				parentHandle = std::stoi(record->value, NULL, 16);
			}

			this->curRecord++;
		}
	}
	static void computeObject(PrDxfComputeObjectTask* thread)
	{
		for (; thread->curRecord < thread->records.size(); thread->curRecord++)
		{
			if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kDictionary)
			{
				thread->readDbDictionary();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kLayout)
			{
				thread->readDbLayout();
			}
			else if (thread->records[thread->curRecord]->key == PrDxfRecord::EDxfCode::kXdata)
			{
				thread->readDbXdata();
			}
		}

		thread->isDone.store(true);
	}
};



PrDXFParser::PrDXFParser()
{
	this->m_pImpl = new PrDXFParserImpl();
}
PrDXFParser::~PrDXFParser()
{
	delete this->m_pImpl;
}
DbDatabase* PrDXFParser::load(std::string _fileName)
{

#if WIN32
	do
	{

		std::string content;
		std::ifstream myfile(_fileName, std::ios::binary);
		if (myfile.is_open())
		{
			myfile.seekg(0, std::ios::end);
			content.resize(myfile.tellg());
			myfile.seekg(0, std::ios::beg);
			myfile.read(&content[0], content.size());
			myfile.close();
		}
		else
		{
			return NULL;
		}

		return this->loadBuffer(content.c_str(), content.length());

	} while (false);
#endif
	return NULL;
}
DbDatabase* PrDXFParser::loadBuffer(std::string _buff)
{
	return this->loadBuffer(_buff.c_str(), _buff.length());
}
DbDatabase* PrDXFParser::loadBuffer(const char* _buff, unsigned int len)
{

	::kernel()->acrxWorkerThreadManager()->pause();

	this->m_pImpl->buffers = _buff;
	this->m_pImpl->countByte = len;

	// 创建数据库
	this->m_pImpl->database = new DbDatabase(false);
	this->m_pImpl->database->pImpl->isInspect = false;

	// 解析记录
	std::cout << "开始解析记录" << std::endl;
	this->computeRecords();
	std::cout << "解析记录完成" << std::endl;

	// 计算头段
	std::cout << "开始解析头段" << std::endl;
	this->computeHeaderSection();
	std::cout << "解析头段完成" << std::endl;

	// 计算表段
	std::cout << "开始计算表段" << std::endl;
	this->computeTableSection();
	std::cout << "计算表段完成" << std::endl;

	// 获得所有图层map
	AcMap<std::string, DbObjectId>* layerTableRecordMap = new AcMap<std::string, DbObjectId>();
	{
		DbLayerTable* pLayerTable = NULL;
		this->m_pImpl->database->getLayerTable(pLayerTable);

		DbLayerTableIterator* pLayerTableIterator = NULL;
		pLayerTable->newIterator(pLayerTableIterator);

		for (pLayerTableIterator->start(); !pLayerTableIterator->done(); pLayerTableIterator->step()) {
			DbLayerTableRecord* pLayerTableRecord = NULL;
			pLayerTableIterator->getRecord(pLayerTableRecord);
			if (pLayerTableRecord == NULL) {
				continue;
			}

			AcString layerName;
			pLayerTableRecord->getName(layerName);
			DbObjectId layerId = pLayerTableRecord->objectId();
			layerTableRecordMap->insert(layerName.string(), layerId);

			pLayerTableRecord->close();


		}
		delete pLayerTableIterator;
	}
	this->m_pImpl->layerTableRecordMap = layerTableRecordMap;

	// 获得文字样式map
	AcMap<std::string, DbObjectId>* textStyleTableRecordMap = new AcMap<std::string, DbObjectId>();
	{
		DbTextStyleTable* pTextStyleTable = NULL;
		this->m_pImpl->database->getTextStyleTable(pTextStyleTable);

		DbTextStyleTableIterator* pTextStyleTableIterator = NULL;
		pTextStyleTable->newIterator(pTextStyleTableIterator);

		for (pTextStyleTableIterator->start(); !pTextStyleTableIterator->done(); pTextStyleTableIterator->step()) {
			DbTextStyleTableRecord* pTextStyleTableRecord = NULL;
			pTextStyleTableIterator->getRecord(pTextStyleTableRecord);
			if (pTextStyleTableRecord == NULL) {
				continue;
			}

			AcString textStyleName;
			pTextStyleTableRecord->getName(textStyleName);
			DbObjectId textStyleId = pTextStyleTableRecord->objectId();
			textStyleTableRecordMap->insert(textStyleName.string(), textStyleId);
			pTextStyleTableRecord->close();
		}
		delete pTextStyleTableIterator;
	}
	this->m_pImpl->textStyleTableRecordMap = textStyleTableRecordMap;

	// 获得所有块表记录map
	AcMap<std::string, DbObjectId>* blockTableRecordMap = new AcMap<std::string, DbObjectId>();
	{
		DbBlockTable* pBlockTable = NULL;
		this->m_pImpl->database->getBlockTable(pBlockTable);

		DbBlockTableIterator* pBlockTableIterator = NULL;
		pBlockTable->newIterator(pBlockTableIterator);

		for (pBlockTableIterator->start(); !pBlockTableIterator->done(); pBlockTableIterator->step()) {
			DbBlockTableRecord* pBlockTableRecord = NULL;
			pBlockTableIterator->getRecord(pBlockTableRecord);
			if (pBlockTableRecord == NULL) {
				continue;
			}

			AcString blockName;
			pBlockTableRecord->getName(blockName);
			DbObjectId blockId = pBlockTableRecord->objectId();
			blockTableRecordMap->insert(blockName.string(), blockId);

			DbBlockTableRecordIterator* pBlockTableRecordIterator = NULL;
			pBlockTableRecord->newIterator(pBlockTableRecordIterator);
			int b = 0;
			for (pBlockTableRecordIterator->start(); !pBlockTableRecordIterator->done(); pBlockTableRecordIterator->step()) {
				DbEntity* entity = NULL;
				pBlockTableRecordIterator->getEntity(entity);
				if (entity != NULL) {
					entity->close();
					b++;
				}
			}
			delete pBlockTableRecordIterator;
			pBlockTableRecord->close();
			//std::cout << blockName.constPtr() << ":" << b << std::endl;

			pBlockTableRecord->close();
		}
	}
	this->m_pImpl->blockTableRecordMap = blockTableRecordMap;

	// 获得所有标注样式记录map
	AcMap<std::string, DbObjectId>* dimStyleTableRecordMap = new AcMap<std::string, DbObjectId>();
	{
		DbDimStyleTable* pDimStyleTable = NULL;
		this->m_pImpl->database->getDimStyleTable(pDimStyleTable);

		DbDimStyleTableIterator* pDimStyleTableIterator = NULL;
		pDimStyleTable->newIterator(pDimStyleTableIterator);

		for (pDimStyleTableIterator->start(); !pDimStyleTableIterator->done(); pDimStyleTableIterator->step()) {
			DbDimStyleTableRecord* pDimStyleTableRecord = NULL;
			pDimStyleTableIterator->getRecord(pDimStyleTableRecord);
			if (pDimStyleTableRecord == NULL) {
				continue;
			}

			AcString dimStyleName;
			pDimStyleTableRecord->getName(dimStyleName);
			DbObjectId dimStyleId = pDimStyleTableRecord->objectId();
			dimStyleTableRecordMap->insert(dimStyleName.string(), dimStyleId);

			pDimStyleTableRecord->close();
		}
	}
	this->m_pImpl->dimStyleTableRecordMap = dimStyleTableRecordMap;

	// 计算块段
	this->computeBlockSection();

	// 计算实体段
	this->computeEntitySection();

	// 计算对象段
	this->computeObjectSection();

	// 设置文字样式
	DbObjectId textStyleId = textStyleTableRecordMap->find(this->m_pImpl->textStyle.string());
	this->m_pImpl->database->setTextstyle(textStyleId);

	// 释放对象
	delete layerTableRecordMap;
	delete textStyleTableRecordMap;
	delete blockTableRecordMap;
	delete dimStyleTableRecordMap;
	for (int i = this->m_pImpl->records.size() - 1; i >= 0; i--) {
		PrDxfRecord* temp = this->m_pImpl->records[i];
		delete temp;
	}

	// 修复数据库
	this->m_pImpl->database->pImpl->isInspect = true;
	::repairDatabase(this->m_pImpl->database);

	::kernel()->acrxWorkerThreadManager()->pause(false);


	return this->m_pImpl->database;
}

void PrDXFParser::computeViewTable() {

	unsigned int handle = 0;
	int index = this->m_pImpl->zeroRecords[this->m_pImpl->curZeroRecord] + 1;
	for (int i = index; i < this->m_pImpl->records.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[index];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[index]->key == 0)
		{
			break;
		}
		else if (this->m_pImpl->records[index]->key == 5)
		{
			handle = std::stoi(this->m_pImpl->records[index]->value, NULL, 16);
		}

		index++;
	}

	DbViewTable* pTable = new DbViewTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pViewTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeLinetypeTable() {

	unsigned int handle = 0;
	int index = this->m_pImpl->zeroRecords[this->m_pImpl->curZeroRecord] + 1;
	for (int i = index; i < this->m_pImpl->records.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[index];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[index]->key == 0)
		{
			break;
		}
		else if (this->m_pImpl->records[index]->key == 5)
		{
			handle = std::stoi(this->m_pImpl->records[index]->value, NULL, 16);
		}

		index++;
	}

	DbLinetypeTable* pTable = new DbLinetypeTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pLinetypeTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeLayerTable() {

	unsigned int handle = 0;
	int index = this->m_pImpl->zeroRecords[this->m_pImpl->curZeroRecord] + 1;
	for (int i = index; i < this->m_pImpl->records.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[index];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[index]->key == 0)
		{
			break;
		}
		else if (this->m_pImpl->records[index]->key == 5)
		{
			handle = std::stoi(this->m_pImpl->records[index]->value, NULL, 16);
		}

		index++;
	}

	DbLayerTable* pTable = new DbLayerTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pLayerTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeTextStyleTable() {

	unsigned int handle = 0;
	int index = this->m_pImpl->zeroRecords[this->m_pImpl->curZeroRecord] + 1;
	for (int i = index; i < this->m_pImpl->records.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[index];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[index]->key == 0)
		{
			break;
		}
		else if (this->m_pImpl->records[index]->key == 5)
		{
			handle = std::stoi(this->m_pImpl->records[index]->value, NULL, 16);
		}

		index++;
	}

	DbTextStyleTable* pTable = new DbTextStyleTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pTextStyleTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeViewportTable() {

	unsigned int handle = 0;
	int index = this->m_pImpl->zeroRecords[this->m_pImpl->curZeroRecord] + 1;
	for (int i = index; i < this->m_pImpl->records.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[index];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[index]->key == 0)
		{
			break;
		}
		else if (this->m_pImpl->records[index]->key == 5)
		{
			handle = std::stoi(this->m_pImpl->records[index]->value, NULL, 16);
		}

		index++;
	}

	DbViewportTable* pTable = new DbViewportTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pViewportTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeUCSTable() {

	unsigned int handle = 0;
	int index = this->m_pImpl->zeroRecords[this->m_pImpl->curZeroRecord] + 1;
	for (int i = index; i < this->m_pImpl->records.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[index];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[index]->key == 0)
		{
			break;
		}
		else if (this->m_pImpl->records[index]->key == 5)
		{
			handle = std::stoi(this->m_pImpl->records[index]->value, NULL, 16);
		}

		index++;
	}

	DbUCSTable* pTable = new DbUCSTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pUCSTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeAppidTable() {

	unsigned int handle = 0;
	int index = this->m_pImpl->zeroRecords[this->m_pImpl->curZeroRecord] + 1;
	for (int i = index; i < this->m_pImpl->records.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[index];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[index]->key == 0)
		{
			break;
		}
		else if (this->m_pImpl->records[index]->key == 5)
		{
			handle = std::stoi(this->m_pImpl->records[index]->value, NULL, 16);
		}

		index++;
	}


}
void PrDXFParser::computeDimStyleTable() {

	unsigned int handle = 0;
	int index = this->m_pImpl->zeroRecords[this->m_pImpl->curZeroRecord] + 1;
	for (int i = index; i < this->m_pImpl->records.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[index];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[index]->key == 0)
		{
			break;
		}
		else if (this->m_pImpl->records[index]->key == 5)
		{
			handle = std::stoi(this->m_pImpl->records[index]->value, NULL, 16);
		}

		index++;
	}

	DbDimStyleTable* pTable = new DbDimStyleTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pDimStyleTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeBlockTable() {

	unsigned int handle = 0;
	int index = this->m_pImpl->zeroRecords[this->m_pImpl->curZeroRecord] + 1;
	for (int i = index; i < this->m_pImpl->records.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[index];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[index]->key == 0)
		{
			break;
		}
		else if (this->m_pImpl->records[index]->key == 5)
		{
			handle = std::stoi(this->m_pImpl->records[index]->value, NULL, 16);
		}

		index++;
	}

	DbBlockTable* pTable = new DbBlockTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pBlockTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeTable(PrDxfRecord::EDxfCode code)
{

	std::string tableName = "View";
	PrDxfRecord::EDxfCode tableRecord;
	if (code == PrDxfRecord::EDxfCode::kViewTable)
	{
		tableName = "View";
		tableRecord = PrDxfRecord::EDxfCode::kViewTableRecord;
	}
	else if (code == PrDxfRecord::EDxfCode::kLinetypeTable)
	{
		tableName = "LTYPE";
		tableRecord = PrDxfRecord::EDxfCode::kLinetypeTableRecord;
	}
	else if (code == PrDxfRecord::EDxfCode::kLayerTable)
	{
		tableName = "LAYER";
		tableRecord = PrDxfRecord::EDxfCode::kLayerTableRecord;
	}
	else if (code == PrDxfRecord::EDxfCode::kTextStyleTable)
	{
		tableName = "STYLE";
		tableRecord = PrDxfRecord::EDxfCode::kTextStyleTableRecord;
	}
	else if (code == PrDxfRecord::EDxfCode::kViewportTable)
	{
		tableName = "VPORT";
		tableRecord = PrDxfRecord::EDxfCode::kViewportTableRecord;
	}
	else if (code == PrDxfRecord::EDxfCode::kUCSTable)
	{
		tableName = "UCS";
		tableRecord = PrDxfRecord::EDxfCode::kUCSTableRecord;
	}
	else if (code == PrDxfRecord::EDxfCode::kAppidTable)
	{
		tableName = "APPID";
		tableRecord = PrDxfRecord::EDxfCode::kAppidTableRecord;
	}
	else if (code == PrDxfRecord::EDxfCode::kDimStyleTable)
	{
		tableName = "DIMSTYLE";
		tableRecord = PrDxfRecord::EDxfCode::kDimStyleTableRecord;
	}
	else if (code == PrDxfRecord::EDxfCode::kBlockTable)
	{
		tableName = "BLOCK_RECORD";
		tableRecord = PrDxfRecord::EDxfCode::kBlockTableRecord;
	}

	// 读取表
	if (code == PrDxfRecord::EDxfCode::kViewportTable)
	{
		this->computeViewportTable();
	}
	else if (code == PrDxfRecord::EDxfCode::kLinetypeTable)
	{
		this->computeLinetypeTable();
	}
	else if (code == PrDxfRecord::EDxfCode::kLayerTable)
	{
		this->computeLayerTable();
	}
	else if (code == PrDxfRecord::EDxfCode::kTextStyleTable)
	{
		this->computeTextStyleTable();
	}
	else if (code == PrDxfRecord::EDxfCode::kViewTable)
	{
		this->computeViewTable();
	}
	else if (code == PrDxfRecord::EDxfCode::kUCSTable)
	{
		this->computeUCSTable();
	}
	else if (code == PrDxfRecord::EDxfCode::kDimStyleTable)
	{
		this->computeDimStyleTable();
	}
	else if (code == PrDxfRecord::EDxfCode::kBlockTable)
	{
		this->computeBlockTable();
	}

	std::vector<PrDxfObjectRecord*> objectRecords;
	for (int i = this->m_pImpl->curZeroRecord; i < this->m_pImpl->zeroRecords.size(); i++)
	{
		// 读取当前记录
		PrDxfRecord* curRecord = this->m_pImpl->records[this->m_pImpl->zeroRecords[i]];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		if (this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]->value == tableName)
		{
			this->m_pImpl->curZeroRecord = i + 1;
			PrDxfObjectRecord* objectRecord = new PrDxfObjectRecord();
			objectRecord->startPos = this->m_pImpl->zeroRecords[i];
			objectRecord->endPos = this->m_pImpl->zeroRecords[i + 1];
			objectRecords.push_back(objectRecord);
		}
		else if (this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]->value == "ENDTAB")
		{
			this->m_pImpl->curZeroRecord = i + 1;
			break;
		}
	}
	if (objectRecords.size() <= 0)
	{
		return;
	}

	// 创建任务
	AcArray<PrDxfComputeTableTask*> computeTableTasks;
	int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
	for (int i = 0; i < numWorkerThread - 1; i++) {
		PrDxfComputeTableTask* task = new PrDxfComputeTableTask();
		task->content = this->m_pImpl->buffers;
		task->database = this->m_pImpl->database;
		task->isMainThread = false;
		computeTableTasks.append(task);
	}
	PrDxfComputeTableTask* task = new PrDxfComputeTableTask();
	task->content = this->m_pImpl->buffers;
	task->database = this->m_pImpl->database;
	task->isMainThread = true;
	computeTableTasks.append(task);

	// 分配任务
	int indexThread = 0;
	for (int i = 0; i < objectRecords.size(); i++) {
		computeTableTasks[indexThread]->records.push_back(new PrDxfRecord(tableRecord));
		for (int u = objectRecords[i]->startPos; u < objectRecords[i]->endPos; u++)
		{
			computeTableTasks[indexThread]->records.push_back(this->m_pImpl->records[u]);
		}
		computeTableTasks[indexThread]->records.push_back(new PrDxfRecord(PrDxfRecord::EDxfCode::kObjectMark));

		indexThread++;
		if (indexThread >= computeTableTasks.length()) {
			indexThread = 0;
		}
	}

	// 释放objectRecords内存
	for (int i = objectRecords.size() - 1; i >= 0; i--)
	{
		PrDxfObjectRecord* temp = objectRecords[i];
		delete temp;
	}

	// 启动任务
	for (int i = 0; i < computeTableTasks.length(); i++)
	{
		if (computeTableTasks[i]->isMainThread == false)
		{
			PrDxfComputeTableTask* computeTableTask = computeTableTasks[i];
			::kernel()->acrxWorkerThreadManager()->addTask([computeTableTask, code]()->void {
				PrDxfComputeTableTask::computeTableRecord(computeTableTask, code);
				}, true);
		}
		else
		{
			PrDxfComputeTableTask::computeTableRecord(computeTableTasks[i], code);
		}
	}

	// 等待任务完成
	for (int i = 0; i < computeTableTasks.length(); i++)
	{
		if (computeTableTasks[i]->isMainThread == false && computeTableTasks[i]->isDone.load() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			i = 0;
		}
	}

	// 添加到数据库
	if (code == PrDxfRecord::EDxfCode::kViewportTable)
	{
		DbViewportTable* pViewportTable = NULL;
		this->m_pImpl->database->getViewportTable(pViewportTable, Db::kForWrite);
		if (pViewportTable != NULL)
		{
			for (int i = 0; i < computeTableTasks.length(); i++)
			{
				for (int u = 0; u < computeTableTasks[i]->objects.size(); u++)
				{
					DbViewportTableRecord* pViewportTableRecord = DbViewportTableRecord::cast(computeTableTasks[i]->objects[u]->object);
					pViewportTable->add(pViewportTableRecord);
					pViewportTableRecord->close();
				}
			}
			pViewportTable->close();
		}
	}
	else if (code == PrDxfRecord::EDxfCode::kLinetypeTable)
	{
		DbLinetypeTable* pLinetypeTable = NULL;
		this->m_pImpl->database->getLinetypeTable(pLinetypeTable, Db::kForWrite);
		if (pLinetypeTable != NULL)
		{
			for (int i = 0; i < computeTableTasks.length(); i++)
			{
				for (int u = 0; u < computeTableTasks[i]->objects.size(); u++)
				{
					DbLinetypeTableRecord* pLinetypeTableRecord = DbLinetypeTableRecord::cast(computeTableTasks[i]->objects[u]->object);
					pLinetypeTable->add(pLinetypeTableRecord);
					pLinetypeTableRecord->close();
				}
			}
			pLinetypeTable->close();
		}
	}
	else if (code == PrDxfRecord::EDxfCode::kLayerTable)
	{
		DbLayerTable* pLayerTable = NULL;
		this->m_pImpl->database->getLayerTable(pLayerTable, Db::kForWrite);
		if (pLayerTable != NULL)
		{
			for (int i = 0; i < computeTableTasks.length(); i++)
			{
				for (int u = 0; u < computeTableTasks[i]->objects.size(); u++)
				{
					DbLayerTableRecord* pLayerTableRecord = DbLayerTableRecord::cast(computeTableTasks[i]->objects[u]->object);
					pLayerTable->add(pLayerTableRecord);
					pLayerTableRecord->close();
				}
			}
			pLayerTable->close();
		}
	}
	else if (code == PrDxfRecord::EDxfCode::kTextStyleTable)
	{
		DbTextStyleTable* pTextStyleTable = NULL;
		this->m_pImpl->database->getTextStyleTable(pTextStyleTable, Db::kForWrite);
		if (pTextStyleTable != NULL)
		{
			for (int i = 0; i < computeTableTasks.length(); i++)
			{
				for (int u = 0; u < computeTableTasks[i]->objects.size(); u++)
				{
					DbTextStyleTableRecord* pTextStyleTableRecord = DbTextStyleTableRecord::cast(computeTableTasks[i]->objects[u]->object);
					pTextStyleTable->add(pTextStyleTableRecord);
					pTextStyleTableRecord->close();
				}
			}
			pTextStyleTable->close();
		}
	}
	else if (code == PrDxfRecord::EDxfCode::kViewTable)
	{
		DbViewTable* pViewTable = NULL;
		this->m_pImpl->database->getViewTable(pViewTable, Db::kForWrite);
		if (pViewTable != NULL)
		{
			for (int i = 0; i < computeTableTasks.length(); i++)
			{
				for (int u = 0; u < computeTableTasks[i]->objects.size(); u++)
				{
					DbViewTableRecord* pViewTableRecord = DbViewTableRecord::cast(computeTableTasks[i]->objects[u]->object);
					pViewTable->add(pViewTableRecord);
					pViewTableRecord->close();
				}
			}
			pViewTable->close();
		}
	}
	else if (code == PrDxfRecord::EDxfCode::kUCSTable)
	{
		DbUCSTable* pUCSTable = NULL;
		this->m_pImpl->database->getUCSTable(pUCSTable, Db::kForWrite);
		if (pUCSTable != NULL)
		{
			for (int i = 0; i < computeTableTasks.length(); i++)
			{
				for (int u = 0; u < computeTableTasks[i]->objects.size(); u++)
				{
					DbUCSTableRecord* pUCSTableRecord = DbUCSTableRecord::cast(computeTableTasks[i]->objects[u]->object);
					pUCSTable->add(pUCSTableRecord);
					pUCSTableRecord->close();
				}
			}
			pUCSTable->close();
		}
	}
	else if (code == PrDxfRecord::EDxfCode::kDimStyleTable)
	{
		int count = 0;
		DbDimStyleTable* pDimStyleTable = NULL;
		this->m_pImpl->database->getDimStyleTable(pDimStyleTable, Db::kForWrite);
		if (pDimStyleTable != NULL)
		{
			for (int i = 0; i < computeTableTasks.length(); i++)
			{
				for (int u = 0; u < computeTableTasks[i]->objects.size(); u++)
				{
					DbDimStyleTableRecord* pDimStyleTableRecord = DbDimStyleTableRecord::cast(computeTableTasks[i]->objects[u]->object);
					pDimStyleTable->add(pDimStyleTableRecord);
					pDimStyleTableRecord->close();
					count++;
				}
			}
			pDimStyleTable->close();
		}
	}
	else if (code == PrDxfRecord::EDxfCode::kBlockTable)
	{
		int count = 0;
		DbBlockTable* pBlockTable = NULL;
		this->m_pImpl->database->getBlockTable(pBlockTable, Db::kForWrite);
		if (pBlockTable != NULL)
		{
			for (int i = 0; i < computeTableTasks.length(); i++)
			{
				for (int u = 0; u < computeTableTasks[i]->objects.size(); u++)
				{
					DbBlockTableRecord* pBlockTableRecord = DbBlockTableRecord::cast(computeTableTasks[i]->objects[u]->object);
					pBlockTable->add(pBlockTableRecord);
					pBlockTableRecord->close();
					count++;
				}
			}
			pBlockTable->close();
		}
	}

	// 删除任务
	realloc_arry_all(computeTableTasks);
}

void PrDXFParser::computeRecords()
{

	// 获得换行位置
	char keyBuf[11];
	std::vector<int> wrapRecords;
	for (int i = 0; i < this->m_pImpl->countByte; i++)
	{
		if (this->m_pImpl->buffers[i] == '\n')
		{
			wrapRecords.push_back(i);
		}
	}

	// 创建任务
	AcArray<PrDxfComputeRecordTask*> computeRecords;
	int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
	numWorkerThread = 1;
	for (int i = 0; i < numWorkerThread - 1; i++) {
		PrDxfComputeRecordTask* task = new PrDxfComputeRecordTask();
		task->isMainThread = false;
		task->content = this->m_pImpl->buffers;
		computeRecords.append(task);
	}
	PrDxfComputeRecordTask* task = new PrDxfComputeRecordTask();
	task->isMainThread = true;
	task->content = this->m_pImpl->buffers;
	computeRecords.append(task);

	// 分配任务
	int single = int(wrapRecords.size() / computeRecords.length());
	if (single % 2 == 0)
	{
		single = single - 1;
	}
	for (int i = 0; i < computeRecords.length(); i++)
	{
		computeRecords[i]->startVal = i * single;
		computeRecords[i]->endVal = (i + 1) * single;
		computeRecords[i]->wrapRecords = wrapRecords;
		if (i == computeRecords.length() - 1)
		{
			computeRecords[i]->endVal = wrapRecords.size();
		}
	}

	// 启动任务
	for (int i = 0; i < computeRecords.length(); i++) {
		if (computeRecords[i]->isMainThread == true) {
			PrDxfComputeRecordTask::computeRecords(computeRecords[i]);
		}
		else {
			PrDxfComputeRecordTask* computeRecordTask = computeRecords[i];
			::kernel()->acrxWorkerThreadManager()->addTask([computeRecordTask]()->void {
				PrDxfComputeRecordTask::computeRecords(computeRecordTask);
				}, true);
		}
	}

	// 等待任务完成
	for (int i = 0; i < computeRecords.length(); i++)
	{
		if (computeRecords[i]->isMainThread == false && computeRecords[i]->isDone.load() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			i = 0;
		}
	}

	// 得到所有记录
	for (int i = 0; i < computeRecords.length(); i++) {
		std::vector<PrDxfRecord*> tempRecords = computeRecords[i]->records;
		for (int u = 0; u < tempRecords.size(); u++)
		{
			this->m_pImpl->records.push_back(tempRecords[u]);
		}
		int startZero = i * single;
		std::vector<int> tempZeroRecords = computeRecords[i]->zeroRecordPos;
		for (int u = 0; u < tempZeroRecords.size(); u++)
		{
			this->m_pImpl->zeroRecords.push_back(startZero + tempZeroRecords[u]);
		}
	}

	// 删除任务
	realloc_arry_all(computeRecords);
}
void PrDXFParser::computeHeaderSection()
{
	// 获得所有头信息开始和结束点
	int startPos = 0;
	int endPos = 0;
	for (int i = 0; i < this->m_pImpl->zeroRecords.size(); i++)
	{
		::readRecordValue(this->m_pImpl->buffers, this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]);
		if (this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]->value == "ENDSEC")
		{
			this->m_pImpl->curZeroRecord = i;
			endPos = this->m_pImpl->zeroRecords[i];
			break;
		}
	}
	std::cout << "获得所有头信息开始和结束点结束" << std::endl;
	// 创建任务
	AcArray<PrDxfComputeHeaderTask*> computeHeaderTasks;
	int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
	for (int i = 0; i < numWorkerThread - 1; i++) {
		PrDxfComputeHeaderTask* task = new PrDxfComputeHeaderTask();
		task->content = this->m_pImpl->buffers;
		task->isMainThread = false;
		computeHeaderTasks.append(task);
	}
	PrDxfComputeHeaderTask* task = new PrDxfComputeHeaderTask();
	task->content = this->m_pImpl->buffers;
	task->isMainThread = true;
	computeHeaderTasks.append(task);
	std::cout << "创建任务结束" << std::endl;
	// 分配任务
	int indexThread = 0;
	for (int i = startPos; i < endPos; i++) {
		if (this->m_pImpl->records[i]->key == 9) {
			indexThread++;
			if (indexThread >= computeHeaderTasks.length()) {
				indexThread = 0;
			}
		}

		computeHeaderTasks[indexThread]->records.push_back(this->m_pImpl->records[i]);
	}
	std::cout << "分配任务完成" << std::endl;
	// 启动任务
	for (int i = 0; i < computeHeaderTasks.length(); i++)
	{
		if (computeHeaderTasks[i]->isMainThread == true) {
			PrDxfComputeHeaderTask::computeHeadTask(computeHeaderTasks[i], this);
		}
		else {
			PrDxfComputeHeaderTask* computetask = computeHeaderTasks[i];
			::kernel()->acrxWorkerThreadManager()->addTask([computetask, this]()->void {
				PrDxfComputeHeaderTask::computeHeadTask(computetask, this);
				}, true);
		}
	}
	std::cout << "启动任务完成" << std::endl;
	// 等待任务完成
	for (int i = 0; i < computeHeaderTasks.length(); i++)
	{
		if (computeHeaderTasks[i]->isMainThread == false && computeHeaderTasks[i]->isDone.load() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			i = 0;
		}
	}

	std::cout << "等待任务完成" << std::endl;
	// 删除任务
	realloc_arry_all(computeHeaderTasks);
}
void PrDXFParser::computeTableSection()
{

	// 获得段块开始点
	int startPos = 0;
	for (int i = this->m_pImpl->curZeroRecord; i < this->m_pImpl->zeroRecords.size(); i++)
	{
		::readRecordValue(this->m_pImpl->buffers, this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]);
		if (this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]->value == "SECTION")
		{
			int nextRecord = this->m_pImpl->zeroRecords[i] + 1;
			::readRecordValue(this->m_pImpl->buffers, this->m_pImpl->records[nextRecord]);
			if (this->m_pImpl->records[nextRecord]->key == 2 && this->m_pImpl->records[nextRecord]->value == "TABLES")
			{
				this->m_pImpl->curZeroRecord = i + 1;
				startPos = nextRecord;
				break;
			}
		}
	}
	if (startPos == 0)
	{
		return;
	}

	this->computeTable(PrDxfRecord::EDxfCode::kViewportTable);
	this->computeTable(PrDxfRecord::EDxfCode::kLinetypeTable);
	this->computeTable(PrDxfRecord::EDxfCode::kLayerTable);
	this->computeTable(PrDxfRecord::EDxfCode::kTextStyleTable);
	this->computeTable(PrDxfRecord::EDxfCode::kViewTable);
	this->computeTable(PrDxfRecord::EDxfCode::kUCSTable);
	this->computeTable(PrDxfRecord::EDxfCode::kAppidTable);
	this->computeTable(PrDxfRecord::EDxfCode::kDimStyleTable);
	this->computeTable(PrDxfRecord::EDxfCode::kBlockTable);
}
void PrDXFParser::computeBlockSection()
{

	// 获得块段开始点
	int startPos = 0;
	for (int i = this->m_pImpl->curZeroRecord; i < this->m_pImpl->zeroRecords.size(); i++)
	{
		::readRecordValue(this->m_pImpl->buffers, this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]);
		if (this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]->value == "SECTION")
		{
			int nextRecord = this->m_pImpl->zeroRecords[i] + 1;
			::readRecordValue(this->m_pImpl->buffers, this->m_pImpl->records[nextRecord]);
			if (this->m_pImpl->records[nextRecord]->key == 2 && this->m_pImpl->records[nextRecord]->value == "BLOCKS")
			{
				this->m_pImpl->curZeroRecord = i + 1;
				startPos = nextRecord;
				break;
			}
		}
	}
	if (startPos == 0)
	{
		return;
	}

	std::vector<PrDxfObjectRecord*> objectRecords;
	for (int i = this->m_pImpl->curZeroRecord; i < this->m_pImpl->zeroRecords.size(); i++)
	{
		// 读取当前记录
		int nbnn = this->m_pImpl->zeroRecords[i];
		PrDxfRecord* curRecord = this->m_pImpl->records[this->m_pImpl->zeroRecords[i]];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		this->m_pImpl->curZeroRecord = i + 1;
		if (curRecord->value == "ENDSEC")
		{
			break;
		}

		PrDxfObjectRecord* objectRecord = new PrDxfObjectRecord();
		objectRecord->code = PrDxfRecord::EDxfCode::kNone;
		if (curRecord->value == "INSERT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kBlockReference;
		}
		else if (curRecord->value == "LINE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kLine;
		}
		else if (curRecord->value == "CIRCLE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kCircle;
		}
		else if (curRecord->value == "ARC")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kArc;
		}
		else if (curRecord->value == "POINT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kPoint;
		}
		else if (curRecord->value == "LWPOLYLINE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kPolyline;
		}
		else if (curRecord->value == "TEXT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kText;
		}
		else if (curRecord->value == "MTEXT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kMText;
		}
		else if (curRecord->value == "SOLID")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kSolid;
		}
		else if (curRecord->value == "DIMENSION")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kDimension;
		}
		else if (curRecord->value == "XLINE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kXLine;
		}
		else if (curRecord->value == "RAY")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kRay;
		}
		else if (curRecord->value == "ELLIPSE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kEllipse;
		}
		else if (curRecord->value == "LEADER")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kLeader;
		}
		else if (curRecord->value == "SPLINE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kSpline;
		}
		else if (curRecord->value == "ATTDEF")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kAttdef;
		}

		if (objectRecord->code != PrDxfRecord::EDxfCode::kNone) {
			objectRecord->startPos = this->m_pImpl->zeroRecords[i];
			objectRecord->endPos = this->m_pImpl->zeroRecords[i + 1];
			objectRecords.push_back(objectRecord);
		}
		else {
			delete objectRecord;
		}
	}
	if (objectRecords.size() <= 0)
	{
		return;
	}

	// 创建任务
	AcArray<PrDxfComputeBlockTask*> computeBlockTasks;
	int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
	for (int i = 0; i < numWorkerThread - 1; i++) {
		PrDxfComputeBlockTask* task = new PrDxfComputeBlockTask();
		task->content = this->m_pImpl->buffers;
		task->database = this->m_pImpl->database;
		task->isMainThread = false;
		task->defaultTextStyle = this->m_pImpl->defaultTextStyle;
		task->blockTableRecordMap = this->m_pImpl->blockTableRecordMap;
		task->layerTableRecordMap = this->m_pImpl->layerTableRecordMap;
		task->textStyleTableRecordMap = this->m_pImpl->textStyleTableRecordMap;
		task->dimStyleTableRecordMap = this->m_pImpl->dimStyleTableRecordMap;
		computeBlockTasks.append(task);
	}
	PrDxfComputeBlockTask* task = new PrDxfComputeBlockTask();
	task->content = this->m_pImpl->buffers;
	task->database = this->m_pImpl->database;
	task->isMainThread = true;
	task->defaultTextStyle = this->m_pImpl->defaultTextStyle;
	task->blockTableRecordMap = this->m_pImpl->blockTableRecordMap;
	task->layerTableRecordMap = this->m_pImpl->layerTableRecordMap;
	task->textStyleTableRecordMap = this->m_pImpl->textStyleTableRecordMap;
	task->dimStyleTableRecordMap = this->m_pImpl->dimStyleTableRecordMap;
	computeBlockTasks.append(task);

	// 分配任务
	int indexThread = 0;
	for (int i = 0; i < objectRecords.size(); i++) {
		computeBlockTasks[indexThread]->records.push_back(new PrDxfRecord(objectRecords[i]->code));
		for (int u = objectRecords[i]->startPos; u < objectRecords[i]->endPos; u++)
		{
			computeBlockTasks[indexThread]->records.push_back(this->m_pImpl->records[u]);
		}
		computeBlockTasks[indexThread]->records.push_back(new PrDxfRecord(PrDxfRecord::EDxfCode::kObjectMark));

		indexThread++;
		if (indexThread >= computeBlockTasks.length()) {
			indexThread = 0;
		}
	}

	// 释放objectRecords内存
	for (int i = objectRecords.size() - 1; i >= 0; i--)
	{
		PrDxfObjectRecord* temp = objectRecords[i];
		delete temp;
	}

	// 启动任务
	for (int i = 0; i < computeBlockTasks.length(); i++)
	{
		if (computeBlockTasks[i]->isMainThread == true) {
			PrDxfComputeBlockTask::computeEntity(computeBlockTasks[i]);
		}
		else {
			PrDxfComputeBlockTask* computetask = computeBlockTasks[i];
			::kernel()->acrxWorkerThreadManager()->addTask([computetask, this]()->void {
				PrDxfComputeBlockTask::computeEntity(computetask);
				}, true);
		}
	}

	// 等待任务完成
	for (int i = 0; i < computeBlockTasks.length(); i++)
	{
		if (computeBlockTasks[i]->isMainThread == false && computeBlockTasks[i]->isDone.load() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			i = 0;
		}
	}

	// 设置对象
	for (int i = 0; i < computeBlockTasks.length(); i++)
	{
		for (int u = 0; u < computeBlockTasks[i]->objects.size(); u++) {
			DbHandleTableRecord* record = ::kernel()->acdbObjectManager()->queryHandleTableRecord(this->m_pImpl->database, computeBlockTasks[i]->objects[u]->parentHandle);
			if (record == NULL) {
				continue;
			}
			DbObject* object = ::kernel()->acdbObjectManager()->openDbObject(record->objectId);
			if (object == NULL) {
				continue;
			}

			if (DB_IMP_OBJECT(object->m_pImpl)->isBlockTableRecord()) {
				DbBlockTableRecord* pBlockTableRecord = (DbBlockTableRecord*)object;
				DbObjectId entityId = NULL;
				pBlockTableRecord->appendDbEntity(entityId, (DbEntity*)computeBlockTasks[i]->objects[u]->object);
			}
		}
	}
	
	// 删除任务
	realloc_arry_all(computeBlockTasks);

}
void PrDXFParser::computeEntitySection()
{
	// 获得段开始点
	int startPos = 0;
	for (int i = this->m_pImpl->curZeroRecord; i < this->m_pImpl->zeroRecords.size(); i++)
	{
		::readRecordValue(this->m_pImpl->buffers, this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]);
		if (this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]->value == "SECTION")
		{
			int nextRecord = this->m_pImpl->zeroRecords[i] + 1;
			::readRecordValue(this->m_pImpl->buffers, this->m_pImpl->records[nextRecord]);
			if (this->m_pImpl->records[nextRecord]->key == 2 && this->m_pImpl->records[nextRecord]->value == "ENTITIES")
			{
				this->m_pImpl->curZeroRecord = i + 1;
				startPos = nextRecord;
				break;
			}
		}
	}
	if (startPos == 0)
	{
		return;
	}

	std::vector<PrDxfObjectRecord*> objectRecords;
	for (int i = this->m_pImpl->curZeroRecord; i < this->m_pImpl->zeroRecords.size(); i++)
	{
		// 读取当前记录
		PrDxfRecord* curRecord = this->m_pImpl->records[this->m_pImpl->zeroRecords[i]];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		this->m_pImpl->curZeroRecord = i + 1;
		if (curRecord->value == "ENDSEC")
		{
			break;
		}

		PrDxfObjectRecord* objectRecord = new PrDxfObjectRecord();
		objectRecord->code = PrDxfRecord::EDxfCode::kNone;
		if (curRecord->value == "INSERT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kBlockReference;
		}
		else if (curRecord->value == "POINT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kPoint;
		}
		else if (curRecord->value == "LINE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kLine;
		}
		else if (curRecord->value == "VIEWPORT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kViewport;
		}
		else if (curRecord->value == "CIRCLE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kCircle;
		}
		else if (curRecord->value == "ARC")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kArc;
		}
		else if (curRecord->value == "POINT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kPoint;
		}
		else if (curRecord->value == "LWPOLYLINE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kPolyline;
		}
		else if (curRecord->value == "TEXT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kText;
		}
		else if (curRecord->value == "MTEXT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kMText;
		}
		else if (curRecord->value == "SOLID")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kSolid;
		}
		else if (curRecord->value == "DIMENSION")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kDimension;
		}
		else if (curRecord->value == "ARC_DIMENSION")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kArcDimension;
		}
		else if (curRecord->value == "LARGE_RADIAL_DIMENSION")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kRadialDimensionLarge;
		}
		else if (curRecord->value == "XLINE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kXLine;
		}
		else if (curRecord->value == "RAY")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kRay;
		}
		else if (curRecord->value == "ELLIPSE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kEllipse;
		}
		else if (curRecord->value == "LEADER")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kLeader;
		}
		else if (curRecord->value == "SPLINE")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kSpline;
		}
		else if (curRecord->value == "ATTDEF")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kAttdef;
		}

		if (objectRecord->code != PrDxfRecord::EDxfCode::kNone) {
			objectRecord->startPos = this->m_pImpl->zeroRecords[i];
			objectRecord->endPos = this->m_pImpl->zeroRecords[i + 1];
			objectRecords.push_back(objectRecord);
		}
		else {
			delete objectRecord;
		}
	}
	if (objectRecords.size() <= 0)
	{
		return;
	}

	// 创建任务
	AcArray<PrDxfComputeEntityTask*> computeEntityTasks;
	int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
	for (int i = 0; i < numWorkerThread - 1; i++) {
		PrDxfComputeEntityTask* task = new PrDxfComputeEntityTask();
		task->content = this->m_pImpl->buffers;
		task->database = this->m_pImpl->database;
		task->isMainThread = false;
		task->defaultTextStyle = this->m_pImpl->defaultTextStyle;
		task->blockTableRecordMap = this->m_pImpl->blockTableRecordMap;
		task->layerTableRecordMap = this->m_pImpl->layerTableRecordMap;
		task->textStyleTableRecordMap = this->m_pImpl->textStyleTableRecordMap;
		task->dimStyleTableRecordMap = this->m_pImpl->dimStyleTableRecordMap;
		computeEntityTasks.append(task);
	}
	PrDxfComputeEntityTask* task = new PrDxfComputeEntityTask();
	task->content = this->m_pImpl->buffers;
	task->database = this->m_pImpl->database;
	task->isMainThread = true;
	task->defaultTextStyle = this->m_pImpl->defaultTextStyle;
	task->blockTableRecordMap = this->m_pImpl->blockTableRecordMap;
	task->layerTableRecordMap = this->m_pImpl->layerTableRecordMap;
	task->textStyleTableRecordMap = this->m_pImpl->textStyleTableRecordMap;
	task->dimStyleTableRecordMap = this->m_pImpl->dimStyleTableRecordMap;
	computeEntityTasks.append(task);

	// 分配任务
	int indexThread = 0;
	for (int i = 0; i < objectRecords.size(); i++) {
		computeEntityTasks[indexThread]->records.push_back(new PrDxfRecord(objectRecords[i]->code));
		for (int u = objectRecords[i]->startPos; u < objectRecords[i]->endPos; u++)
		{
			computeEntityTasks[indexThread]->records.push_back(this->m_pImpl->records[u]);
		}
		computeEntityTasks[indexThread]->records.push_back(new PrDxfRecord(PrDxfRecord::EDxfCode::kObjectMark));

		indexThread++;
		if (indexThread >= computeEntityTasks.length()) {
			indexThread = 0;
		}
	}

	// 启动任务
	for (int i = 0; i < computeEntityTasks.length(); i++)
	{
		if (computeEntityTasks[i]->isMainThread == true) {
			PrDxfComputeEntityTask::computeEntity(computeEntityTasks[i]);
		}
		else {
			PrDxfComputeEntityTask* computetask = computeEntityTasks[i];
			::kernel()->acrxWorkerThreadManager()->addTask([computetask, this]()->void {
				PrDxfComputeEntityTask::computeEntity(computetask);
				}, true);
		}
	}

	// 等待任务完成
	for (int i = 0; i < computeEntityTasks.length(); i++)
	{
		if (computeEntityTasks[i]->isMainThread == false && computeEntityTasks[i]->isDone.load() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			i = 0;
		}
	}

	// 设置对象
	for (int i = 0; i < computeEntityTasks.length(); i++)
	{
		for (int u = 0; u < computeEntityTasks[i]->objects.size(); u++) {
			DbHandleTableRecord* record = ::kernel()->acdbObjectManager()->queryHandleTableRecord(this->m_pImpl->database, computeEntityTasks[i]->objects[u]->parentHandle);
			if (record == NULL) {
				continue;
			}
			DbObject* object = ::kernel()->acdbObjectManager()->openDbObject(record->objectId);
			if (object == NULL) {
				continue;
			}

			if (DB_IMP_OBJECT(object->m_pImpl)->isBlockTableRecord()) {
				DbBlockTableRecord* pBlockTableRecord = (DbBlockTableRecord*)object;
				DbObjectId entityId = NULL;
				pBlockTableRecord->appendDbEntity(entityId, (DbEntity*)computeEntityTasks[i]->objects[u]->object);
			}
		}
	}

	// 删除任务
	realloc_arry_all(computeEntityTasks);
}
void PrDXFParser::computeObjectSection() {

	// 获得段开始点
	int startPos = 0;
	for (int i = this->m_pImpl->curZeroRecord; i < this->m_pImpl->zeroRecords.size(); i++)
	{
		::readRecordValue(this->m_pImpl->buffers, this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]);
		if (this->m_pImpl->records[this->m_pImpl->zeroRecords[i]]->value == "SECTION")
		{
			int nextRecord = this->m_pImpl->zeroRecords[i] + 1;
			::readRecordValue(this->m_pImpl->buffers, this->m_pImpl->records[nextRecord]);
			if (this->m_pImpl->records[nextRecord]->key == 2 && this->m_pImpl->records[nextRecord]->value == "OBJECTS")
			{
				this->m_pImpl->curZeroRecord = i + 1;
				startPos = nextRecord;
				break;
			}
		}
	}
	if (startPos == 0)
	{
		return;
	}

	std::vector<PrDxfObjectRecord*> objectRecords;
	for (int i = this->m_pImpl->curZeroRecord; i < this->m_pImpl->zeroRecords.size(); i++)
	{
		// 读取当前记录
		PrDxfRecord* curRecord = this->m_pImpl->records[this->m_pImpl->zeroRecords[i]];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		this->m_pImpl->curZeroRecord = i + 1;
		if (curRecord->value == "ENDSEC")
		{
			break;
		}

		PrDxfObjectRecord* objectRecord = new PrDxfObjectRecord();
		objectRecord->code = PrDxfRecord::EDxfCode::kNone;
		if (curRecord->value == "DICTIONARY")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kDictionary;
		}
		else if (curRecord->value == "XRECORD")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kXdata;
		}
		else if (curRecord->value == "LAYOUT")
		{
			objectRecord->code = PrDxfRecord::EDxfCode::kLayout;
		}

		if (objectRecord->code != PrDxfRecord::EDxfCode::kNone) {
			objectRecord->startPos = this->m_pImpl->zeroRecords[i];
			objectRecord->endPos = this->m_pImpl->zeroRecords[i + 1];
			objectRecords.push_back(objectRecord);
		}
		else {
			delete objectRecord;
		}
	}

	// 创建任务
	AcArray<PrDxfComputeObjectTask*> computeObjectTasks;
	int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
	for (int i = 0; i < numWorkerThread - 1; i++) {
		PrDxfComputeObjectTask* task = new PrDxfComputeObjectTask();
		task->content = this->m_pImpl->buffers;
		task->database = this->m_pImpl->database;
		task->isMainThread = false;
		computeObjectTasks.append(task);
	}
	PrDxfComputeObjectTask* task = new PrDxfComputeObjectTask();
	task->content = this->m_pImpl->buffers;
	task->database = this->m_pImpl->database;
	task->isMainThread = true;
	computeObjectTasks.append(task);

	// 分配任务
	int indexThread = 0;
	for (int i = 0; i < objectRecords.size(); i++) {
		computeObjectTasks[indexThread]->records.push_back(new PrDxfRecord(objectRecords[i]->code));
		for (int u = objectRecords[i]->startPos; u < objectRecords[i]->endPos; u++)
		{
			computeObjectTasks[indexThread]->records.push_back(this->m_pImpl->records[u]);
		}
		computeObjectTasks[indexThread]->records.push_back(new PrDxfRecord(PrDxfRecord::EDxfCode::kObjectMark));

		indexThread++;
		if (indexThread >= computeObjectTasks.length()) {
			indexThread = 0;
		}
	}

	// 启动任务
	for (int i = 0; i < computeObjectTasks.length(); i++)
	{
		if (computeObjectTasks[i]->isMainThread == true) {
			PrDxfComputeObjectTask::computeObject(computeObjectTasks[i]);
		}
		else {
			PrDxfComputeObjectTask* computetask = computeObjectTasks[i];
			::kernel()->acrxWorkerThreadManager()->addTask([computetask, this]()->void {
				PrDxfComputeObjectTask::computeObject(computetask);
				}, true);
		}
	}

	// 等待任务完成
	for (int i = 0; i < computeObjectTasks.length(); i++)
	{
		if (computeObjectTasks[i]->isMainThread == false && computeObjectTasks[i]->isDone.load() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			i = 0;
		}
	}

	// 添加词典
	AcArray<PrDictionaryRecord*> dictionaryIndexs;
	for (int i = 0; i < computeObjectTasks.length(); i++) {
		for (int u = 0; u < computeObjectTasks[i]->dictionaryIndexs.length(); u++) {
			dictionaryIndexs.append(computeObjectTasks[i]->dictionaryIndexs[u]);
		}
	}
	for (int i = 0; i < dictionaryIndexs.length(); i++) {
		if (dictionaryIndexs[i]->parentHandle == 0) {
			this->m_pImpl->database->addDbObject(dictionaryIndexs[i]->dictionary);
			((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pRootDictionary = dictionaryIndexs[i]->dictionary;
			break;
		}
	}
	while (true) {

		// 设置词典
		bool isAppend = false;
		for (int i = 0; i < dictionaryIndexs.length(); i++) {

			if (dictionaryIndexs[i]->isAppend == true) {
				continue;
			}
			if (dictionaryIndexs[i]->parentHandle == 0) {
				continue;
			}

			// 获得父id
			DbObjectId parentId = NULL;
			this->m_pImpl->database->getDbObjectId(parentId, false, dictionaryIndexs[i]->parentHandle);
			if (parentId == NULL) {
				continue;
			}

			// 获得对象
			DbObject* parentObject = ::kernel()->acdbObjectManager()->openDbObject(parentId);
			if (parentObject == NULL) {
				continue;
			}

			// 如果父节点是词典则将对象添加到词典中
			if (DB_IMP_OBJECT(parentObject->m_pImpl)->isDictionary()) {

				// 设置到对应的词典中
				for (int u = 0; u < dictionaryIndexs.length(); u++) {
					if (dictionaryIndexs[u]->dictionary != parentObject) {
						continue;
					}
					AcArray<AcMapRecord<std::string, unsigned int>*> allMapRecords;
					dictionaryIndexs[u]->mapRecord->findAllRecord(allMapRecords);
					for (int j = 0; j < allMapRecords.length(); j++) {
						if (allMapRecords.at(j)->object == dictionaryIndexs[i]->handle) {
							isAppend = true;
							dictionaryIndexs[i]->isAppend = true;
							DbObjectId id;
							((DbDictionary*)parentObject)->setAt(allMapRecords.at(j)->name.c_str(), dictionaryIndexs[i]->dictionary, id);
							break;
						}
					}
					break;
				}
			}
			// 如果父节点是对象则设置对象的扩展词典id
			else {
				isAppend = true;
				dictionaryIndexs[i]->isAppend = true;
				this->m_pImpl->database->addDbObject(dictionaryIndexs[i]->dictionary);
				DB_IMP_OBJECT(parentObject->m_pImpl)->extensionDictionaryId = dictionaryIndexs[i]->dictionary->objectId();
			}
		}
		if (isAppend == false) {
			break;
		}

		// 重新设置列表
		AcArray<PrDictionaryRecord*> tempDictionaryIndexs;
		for (int i = 0; i < dictionaryIndexs.length(); i++) {
			if (dictionaryIndexs[i]->isAppend == false) {
				tempDictionaryIndexs.append(dictionaryIndexs[i]);
			}
			else {
				PrDictionaryRecord* dictionaryIndex = dictionaryIndexs[i];
				delete dictionaryIndex;
			}
		}
		dictionaryIndexs.removeAll();
		for (int i = 0; i < tempDictionaryIndexs.length(); i++) {
			dictionaryIndexs.append(tempDictionaryIndexs[i]);
		}
	};
	for (int i = dictionaryIndexs.length() - 1; i >= 0; i--) {
		PrDictionaryRecord* dictionaryIndex = dictionaryIndexs[i];
		delete dictionaryIndex;
	}

	// 添加对象
	bool isLayout = false;
	DbDictionary* pRootDictionary = ((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pRootDictionary;
	for (int i = 0; i < computeObjectTasks.length(); i++) {
		for (int u = 0; u < computeObjectTasks[i]->objects.size(); u++) {

			// 获得父对象
			DbHandleTableRecord* record = ::kernel()->acdbObjectManager()->queryHandleTableRecord(this->m_pImpl->database, computeObjectTasks[i]->objects[u]->parentHandle);
			if (record == NULL) {
				continue;
			}
			DbObject* parentObject = ::kernel()->acdbObjectManager()->openDbObject(record->objectId);
			if (parentObject == NULL) {
				continue;
			}

			if (DB_IMP_OBJECT(parentObject->m_pImpl)->isDictionary() &&
				DB_IMP_OBJECT(computeObjectTasks[i]->objects[u]->object->m_pImpl)->isLayout()) {

				DbDictionary* pLayoutDictionary = (DbDictionary*)parentObject;
				DbLayout* pLayout = (DbLayout*)computeObjectTasks[i]->objects[u]->object;

				DbObjectId blockTableRecordId = pLayout->getBlockTableRecordId();
				if (blockTableRecordId != NULL) {
					DbBlockTableRecord* pBlockTableRecord = (DbBlockTableRecord*)::kernel()->acdbObjectManager()->openDbObject(blockTableRecordId);
					if (pBlockTableRecord != NULL) {
						DB_IMP_BLOCKTABLERECORD(pBlockTableRecord->m_pImpl)->layoutId = pLayout->objectId();
					}
				}

				char* layoutName = NULL;
				pLayout->getLayoutName(layoutName);
				DbObjectId layoutId = NULL;
				pLayoutDictionary->setAt(layoutName, pLayout, layoutId);
				if (isLayout == false) {
					isLayout = true;
					pLayout->setTabSelected(true);
				}
			}
		}
	}

	// 设置布局和块表记录
	DbDictionary* layoutDictionary = this->m_pImpl->database->getLayoutDictionary();
	DbDictionaryIterator* dictionaryIterator = layoutDictionary->newIterator();
	for (; !dictionaryIterator->done(); dictionaryIterator->next()) {

		DbLayout* layout = NULL;
		dictionaryIterator->getObject(layout);
		if (layout == NULL) {
			continue;
		}

		AcString layouName;
		layout->getName(layouName);

		// 将块表记录和布局关联
		DbObjectId blockTableRecordId = layout->getBlockTableRecordId();
		DbBlockTableRecord* pBlockTableRecord = NULL;
		if (::acdbOpenObject(pBlockTableRecord, blockTableRecordId) == Acad::ErrorStatus::eOk) {
			DB_IMP_BLOCKTABLERECORD(pBlockTableRecord->m_pImpl)->layoutId = layout->objectId();
			pBlockTableRecord->close();
		}

		layout->close();
	}
	delete dictionaryIterator;

	// 删除任务
	realloc_arry_all(computeObjectTasks);


	//int b = 9;
	/*
	第一个父句柄为0的是数据库词典
	第二个ACAD_LAYERSTATES是图层样式表的扩展词典
	*/
}
