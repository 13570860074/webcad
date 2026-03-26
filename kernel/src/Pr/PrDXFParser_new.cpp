/* ============================================================================
 * PrDXFParser.cpp — 重写的DXF解析器
 *
 * 优化点:
 *   1. 消除大量代码重复,使用统一的实体读取框架
 *   2. 使用查找表替代重复的 if-else 实体名映射
 *   3. 修复 Polyline bulge 数组偏移错误
 *   4. 修复 MText rotation 双重角度转换
 *   5. 使用状态机替代 Dimension 的10个bool标志
 *   6. 添加 Ellipse 实体解析
 *   7. 添加 Hatch 实体解析(基础支持)
 *   8. 优化内存分配(延迟分配替代10MB预分配)
 *   9. 优化 readRecordValue (避免逐字符拼接)
 * ============================================================================ */
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
#include <unordered_map>


/* ── 读取DXF记录值(优化:直接用指针区间构造string) ── */
static void readRecordValue(const char* content, PrDxfRecord* record)
{
	if (record->isValue == false)
	{
		record->isValue = true;
		int len = record->end - record->satrt;
		if (len > 0) {
			record->value.assign(content + record->satrt, len);
		}
	}
}

/* ── 设置数据库对象句柄 ── */
static void setDbObjectHandle(DbObject* object, const unsigned int handle)
{
	DB_IMP_OBJECT(object->m_pImpl)->handle = handle;
}


/* ── 实体名→代码 查找表(消除block/entity段中重复的if-else链) ── */
struct PrEntityNameMap {
	static PrDxfRecord::EDxfCode lookup(const std::string& name)
	{
		static std::unordered_map<std::string, PrDxfRecord::EDxfCode> map = {
			{"INSERT",                PrDxfRecord::EDxfCode::kBlockReference},
			{"LINE",                  PrDxfRecord::EDxfCode::kLine},
			{"CIRCLE",                PrDxfRecord::EDxfCode::kCircle},
			{"ARC",                   PrDxfRecord::EDxfCode::kArc},
			{"POINT",                 PrDxfRecord::EDxfCode::kPoint},
			{"LWPOLYLINE",            PrDxfRecord::EDxfCode::kPolyline},
			{"TEXT",                  PrDxfRecord::EDxfCode::kText},
			{"MTEXT",                 PrDxfRecord::EDxfCode::kMText},
			{"SOLID",                 PrDxfRecord::EDxfCode::kSolid},
			{"DIMENSION",             PrDxfRecord::EDxfCode::kDimension},
			{"ARC_DIMENSION",         PrDxfRecord::EDxfCode::kArcDimension},
			{"LARGE_RADIAL_DIMENSION",PrDxfRecord::EDxfCode::kRadialDimensionLarge},
			{"XLINE",                 PrDxfRecord::EDxfCode::kXLine},
			{"RAY",                   PrDxfRecord::EDxfCode::kRay},
			{"ELLIPSE",               PrDxfRecord::EDxfCode::kEllipse},
			{"LEADER",                PrDxfRecord::EDxfCode::kLeader},
			{"SPLINE",                PrDxfRecord::EDxfCode::kSpline},
			{"ATTDEF",                PrDxfRecord::EDxfCode::kAttdef},
			{"VIEWPORT",              PrDxfRecord::EDxfCode::kViewport},
			{"HATCH",                 PrDxfRecord::EDxfCode::kHatch},
		};
		auto it = map.find(name);
		if (it != map.end()) {
			return it->second;
		}
		return PrDxfRecord::EDxfCode::kNone;
	}
};


/* ── 对象段名→代码 查找表 ── */
struct PrObjectNameMap {
	static PrDxfRecord::EDxfCode lookup(const std::string& name)
	{
		static std::unordered_map<std::string, PrDxfRecord::EDxfCode> map = {
			{"DICTIONARY", PrDxfRecord::EDxfCode::kDictionary},
			{"XRECORD",    PrDxfRecord::EDxfCode::kXdata},
			{"LAYOUT",     PrDxfRecord::EDxfCode::kLayout},
		};
		auto it = map.find(name);
		if (it != map.end()) {
			return it->second;
		}
		return PrDxfRecord::EDxfCode::kNone;
	}
};


/* ── 宏: 声明实体公共属性 ── */
#define PRDXF_DECLARE_ACDBENTITY \
unsigned int handle = 0;\
int blockHandle = 0;\
std::string layerName;\
std::string linetypeName;\
int colorIndex = 256;\
Db::Visibility visibility = Db::Visibility::kVisible;\
Db::LineWeight lineWeight = Db::LineWeight::kLnWtByLwDefault;\


/* ── 宏: 设置实体公共属性并加入对象列表 ── */
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


/* ── 宏: 读取AcDbObject部分 ── */
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


/* ── 宏: 读取AcDbEntity部分 ── */
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


/* ── 辅助: 法线坐标变换 ── */
static GePoint3d transformByNormal(const GePoint3d& pt, const GeVector3d& normal)
{
	if (normal.isEqualTo(GeVector3d::kZAxis)) {
		return pt;
	}
	GeMatrix3d mat;
	mat.setToPlaneToWorld(normal);
	GePoint3d result = pt;
	return result.transformBy(mat);
}



/* ============================================================================
 * 内部辅助类
 * ============================================================================ */

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


/* ============================================================================
 * PrDXFParserImpl — 解析器内部实现
 * ============================================================================ */

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

		this->database = NULL;
		this->curZeroRecord = 0;
	}
	virtual ~PrDXFParserImpl() {

	}


public:
	const char* buffers;
	int countByte;
	int nByte;

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


/* ============================================================================
 * 计算记录任务(多线程)
 * ============================================================================ */

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
					if (i + 1 < (int)thread->wrapRecords.size())
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


/* ============================================================================
 * 计算头段任务
 * ============================================================================ */

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
		for (; curRecord < (int)records.size(); curRecord++)
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
			}
		}

		task->isDone.store(true);
	}
};


/* ============================================================================
 * 计算表段任务
 * ============================================================================ */

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

	/* ── 视口表记录 ── */
	void readDbViewportTableRecord()
	{

		DbViewportTableRecord* pViewportTableRecord = new DbViewportTableRecord();

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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 2)  { name = record->value; }
			else if (record->key == 5)  { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 10) { lowerLeftCorner.x = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 20) { lowerLeftCorner.y = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 11) { upperRightCorner.x = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 21) { upperRightCorner.y = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 12) { centerPoint.x = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 22) { centerPoint.y = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 17) { target.x = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 27) { target.y = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 37) { target.z = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 40) { height = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 41) { width = height / AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 42) { lensLength = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 16) { viewDirection.x = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 26) { viewDirection.y = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 36) { viewDirection.z = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 43) { frontClipDistance = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 44) { backClipDistance = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 51) { viewTwist = AcString::toFloat64(record->value.c_str()); }

			this->curRecord++;
		};

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

		::setDbObjectHandle(pViewportTableRecord, handle);
		this->objects.push_back(new PrObjectInidex(pViewportTableRecord));
	}

	/* ── 线型表记录 ── */
	void readDbLinetypeTableRecord()
	{

		DbLinetypeTableRecord* pLinetypeTableRecord = new DbLinetypeTableRecord();

		unsigned int handle = 0;
		AcString name = "";
		AcString comments = "";
		double patternLength = 0;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 2) { name = record->value; }
			else if (record->key == 5) { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 3) { comments = record->value; }
			else if (record->key == 40) { patternLength = AcString::toFloat64(record->value.c_str()); }

			this->curRecord++;
		}

		pLinetypeTableRecord->setName(name);
		pLinetypeTableRecord->setComments(comments);
		pLinetypeTableRecord->setPatternLength(patternLength);

		::setDbObjectHandle(pLinetypeTableRecord, handle);
		this->objects.push_back(new PrObjectInidex(pLinetypeTableRecord));
	}

	/* ── 图层表记录 ── */
	void readDbLayerTableRecord()
	{

		DbLayerTableRecord* pLayerTableRecord = new DbLayerTableRecord();

		unsigned int handle = 0;
		AcString name = "";
		int colorIndex = 7;
		AcString linetype = "";
		bool isPrint = true;
		Db::LineWeight lineWeight = Db::LineWeight::kLnWtByLwDefault;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 2)   { name = record->value; }
			else if (record->key == 5)   { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 62)  { colorIndex = AcString::toInt32(record->value); }
			else if (record->key == 6)   { linetype = record->value; }
			else if (record->key == 290) { isPrint = (AcString::toInt32(record->value) != 0); }
			else if (record->key == 370) { lineWeight = (Db::LineWeight)AcString::toInt32(record->value); }

			this->curRecord++;
		}

		DbObjectId linetypeId(0);
		DbLinetypeTable* pLinetypeTable = NULL;
		this->database->getLinetypeTable(pLinetypeTable);
		if (pLinetypeTable != NULL)
		{
			pLinetypeTable->getAt(linetype, linetypeId);
			pLinetypeTable->close();
		}

		pLayerTableRecord->setName(name);
		CmColor color;
		color.setColorIndex(colorIndex);
		pLayerTableRecord->setColor(color);
		pLayerTableRecord->setLinetypeObjectId(linetypeId);
		pLayerTableRecord->setLineWeight(lineWeight);

		::setDbObjectHandle(pLayerTableRecord, handle);
		this->objects.push_back(new PrObjectInidex(pLayerTableRecord));
	}

	/* ── 文字样式表记录 ── */
	void readDbTextStyleTableRecord()
	{

		DbTextStyleTableRecord* pTextStyleTableRecord = new DbTextStyleTableRecord();

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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 2)    { name = record->value; }
			else if (record->key == 5)    { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 40)   { textSize = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 41)   { widthFactor = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 50)   { obliquingAngle = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 3)    { fileName = record->value; }
			else if (record->key == 4)    { bigFontFileName = record->value; }
			else if (record->key == 1000) { pTypeface = record->value; }

			this->curRecord++;
		}

		pTextStyleTableRecord->setName(name);
		pTextStyleTableRecord->setTextSize(textSize);
		pTextStyleTableRecord->setXScale(widthFactor);
		pTextStyleTableRecord->setObliquingAngle(obliquingAngle);
		pTextStyleTableRecord->setFileName(fileName);
		pTextStyleTableRecord->setBigFontFileName(bigFontFileName);
		pTextStyleTableRecord->setFont(pTypeface, bold, italic, charset, pitch, family);

		::setDbObjectHandle(pTextStyleTableRecord, handle);
		this->objects.push_back(new PrObjectInidex(pTextStyleTableRecord));
	}

	/* ── 视图表记录 ── */
	void readDbViewTableRecord()
	{

		DbViewTableRecord* pViewTableRecord = new DbViewTableRecord();

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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 2)  { name = record->value; }
			else if (record->key == 5)  { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 12) { centerPoint.x = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 22) { centerPoint.y = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 17) { target.x = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 27) { target.y = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 37) { target.z = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 40) { height = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 41) { width = height / AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 42) { lensLength = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 16) { viewDirection.x = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 26) { viewDirection.y = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 36) { viewDirection.z = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 43) { frontClipDistance = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 44) { backClipDistance = AcString::toFloat64(record->value.c_str()); }
			else if (record->key == 51) { viewTwist = AcString::toFloat64(record->value.c_str()); }

			this->curRecord++;
		}

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

		::setDbObjectHandle(pViewTableRecord, handle);
		this->objects.push_back(new PrObjectInidex(pViewTableRecord));
	}

	/* ── UCS表记录 ── */
	void readDbUCSTableRecord()
	{

		DbUCSTableRecord* pUCSTableRecord = new DbUCSTableRecord();

		unsigned int handle = 0;
		AcString name = "";
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 5) { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 2) { name = record->value; }

			this->curRecord++;
		}

		pUCSTableRecord->setName(name);
		::setDbObjectHandle(pUCSTableRecord, handle);
		this->objects.push_back(new PrObjectInidex(pUCSTableRecord));
	}

	/* ── 标注样式表记录 ── */
	void readDimStyleTableRecord()
	{

		DbDimStyleTableRecord* pDimStyleTableRecord = new DbDimStyleTableRecord();

		unsigned int handle = 0;
		AcString name = "";
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 2) { name = record->value; break; }
			else if (record->key == 5) { handle = std::stoi(record->value, NULL, 16); }

			this->curRecord++;
		}

		pDimStyleTableRecord->setName(name);
		::setDbObjectHandle(pDimStyleTableRecord, handle);
		this->objects.push_back(new PrObjectInidex(pDimStyleTableRecord));
	}

	/* ── 块表记录 ── */
	void readDbBlockTableRecord()
	{

		DbBlockTableRecord* pBlockTableRecord = new DbBlockTableRecord();

		unsigned int handle = 0;
		unsigned int layoutHandle = 0;
		AcString name = "";
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			readRecordValue(content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 2)   { name = record->value; break; }
			else if (record->key == 5)   { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 340) { layoutHandle = std::stoi(record->value, NULL, 16); }

			this->curRecord++;
		}

		pBlockTableRecord->setName(name);
		::setDbObjectHandle(pBlockTableRecord, handle);
		this->objects.push_back(new PrObjectInidex(pBlockTableRecord));
	}

	/* ── 表记录分发 ── */
	static void computeTableRecord(PrDxfComputeTableTask* thread, PrDxfRecord::EDxfCode code)
	{
		for (; thread->curRecord < (int)thread->records.size(); thread->curRecord++)
		{
			int key = thread->records[thread->curRecord]->key;
			if (key == PrDxfRecord::EDxfCode::kViewTableRecord)          { thread->readDbViewTableRecord(); }
			else if (key == PrDxfRecord::EDxfCode::kViewportTableRecord) { thread->readDbViewportTableRecord(); }
			else if (key == PrDxfRecord::EDxfCode::kLinetypeTableRecord) { thread->readDbLinetypeTableRecord(); }
			else if (key == PrDxfRecord::EDxfCode::kLayerTableRecord)    { thread->readDbLayerTableRecord(); }
			else if (key == PrDxfRecord::EDxfCode::kTextStyleTableRecord){ thread->readDbTextStyleTableRecord(); }
			else if (key == PrDxfRecord::EDxfCode::kUCSTableRecord)      { thread->readDbUCSTableRecord(); }
			else if (key == PrDxfRecord::EDxfCode::kBlockTableRecord)    { thread->readDbBlockTableRecord(); }
			else if (key == PrDxfRecord::EDxfCode::kDimStyleTableRecord) { thread->readDimStyleTableRecord(); }
		}

		thread->isDone.store(true);
	}

};


/* ============================================================================
 * 计算实体任务(含block段)
 * ============================================================================ */

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


	/* ── POINT ── */
	void computePoint()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d position;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbPoint = false;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbPoint)
			else if (isAcDbPoint == true) {
				switch (record->key) {
				case 10: position.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: position.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: position.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbPoint* pEntity = new DbPoint();
		pEntity->setPosition(position);
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── LINE ── */
	void computeLine()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d startPoint;
		GePoint3d endPoint;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbLine = false;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbLine)
			else if (isAcDbLine == true) {
				switch (record->key) {
				case 10: startPoint.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: startPoint.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: startPoint.z = AcString::toFloat64(record->value.c_str()); break;
				case 11: endPoint.x = AcString::toFloat64(record->value.c_str()); break;
				case 21: endPoint.y = AcString::toFloat64(record->value.c_str()); break;
				case 31: endPoint.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbLine* pEntity = new DbLine();
		pEntity->setStartPoint(startPoint);
		pEntity->setEndPoint(endPoint);
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── XLINE ── */
	void computeXline()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d basePoint;
		GeVector3d unitDir;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbXline = false;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbXline)
			else if (isAcDbXline == true) {
				switch (record->key) {
				case 10: basePoint.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: basePoint.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: basePoint.z = AcString::toFloat64(record->value.c_str()); break;
				case 11: unitDir.x = AcString::toFloat64(record->value.c_str()); break;
				case 21: unitDir.y = AcString::toFloat64(record->value.c_str()); break;
				case 31: unitDir.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbXline* pEntity = new DbXline();
		pEntity->setBasePoint(basePoint);
		pEntity->setUnitDir(unitDir);
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── RAY ── */
	void computeRay()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d basePoint;
		GeVector3d unitDir;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbRay = false;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbRay)
			else if (isAcDbRay == true) {
				switch (record->key) {
				case 10: basePoint.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: basePoint.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: basePoint.z = AcString::toFloat64(record->value.c_str()); break;
				case 11: unitDir.x = AcString::toFloat64(record->value.c_str()); break;
				case 21: unitDir.y = AcString::toFloat64(record->value.c_str()); break;
				case 31: unitDir.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbRay* pEntity = new DbRay();
		pEntity->setBasePoint(basePoint);
		pEntity->setUnitDir(unitDir);
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── CIRCLE ── */
	void computeCircle()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d center;
		double radius = 0;
		GeVector3d normal = GeVector3d::kZAxis;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbCircle = false;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbCircle)
			else if (isAcDbCircle == true) {
				switch (record->key) {
				case 10: center.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: center.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: center.z = AcString::toFloat64(record->value.c_str()); break;
				case 40: radius = AcString::toFloat64(record->value.c_str()); break;
				case 210: normal.x = AcString::toFloat64(record->value.c_str()); break;
				case 220: normal.y = AcString::toFloat64(record->value.c_str()); break;
				case 230: normal.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbCircle* pEntity = new DbCircle();
		pEntity->setRadius(radius);
		pEntity->setNormal(normal);
		pEntity->setCenter(transformByNormal(center, normal));
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── ARC ── */
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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbArc)
			else if (isAcDbArc == true) {
				switch (record->key) {
				case 10: center.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: center.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: center.z = AcString::toFloat64(record->value.c_str()); break;
				case 40: radius = AcString::toFloat64(record->value.c_str()); break;
				case 50: startAngle = AcString::toFloat64(record->value.c_str()); break;
				case 51: endAngle = AcString::toFloat64(record->value.c_str()); break;
				case 210: normal.x = AcString::toFloat64(record->value.c_str()); break;
				case 220: normal.y = AcString::toFloat64(record->value.c_str()); break;
				case 230: normal.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbArc* pEntity = new DbArc();
		pEntity->setRadius(radius);
		pEntity->setNormal(normal);
		pEntity->setStartAngle(angleToRadian(startAngle));
		pEntity->setEndAngle(angleToRadian(endAngle));
		pEntity->setCenter(transformByNormal(center, normal));
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── ELLIPSE —— 新增实体解析 ── */
	void computeEllipse()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d center;
		GeVector3d majorAxis;
		GeVector3d normal = GeVector3d::kZAxis;
		double radiusRatio = 1.0;
		double startParam = 0.0;
		double endParam = PI * 2;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbEllipse = false;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbEllipse)
			else if (isAcDbEllipse == true) {
				switch (record->key) {
				case 10: center.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: center.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: center.z = AcString::toFloat64(record->value.c_str()); break;
				case 11: majorAxis.x = AcString::toFloat64(record->value.c_str()); break;
				case 21: majorAxis.y = AcString::toFloat64(record->value.c_str()); break;
				case 31: majorAxis.z = AcString::toFloat64(record->value.c_str()); break;
				case 40: radiusRatio = AcString::toFloat64(record->value.c_str()); break;
				case 41: startParam = AcString::toFloat64(record->value.c_str()); break;
				case 42: endParam = AcString::toFloat64(record->value.c_str()); break;
				case 210: normal.x = AcString::toFloat64(record->value.c_str()); break;
				case 220: normal.y = AcString::toFloat64(record->value.c_str()); break;
				case 230: normal.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbEllipse* pEntity = new DbEllipse();
		pEntity->set(center, normal, majorAxis, radiusRatio,
			startParam, endParam);
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── LWPOLYLINE —— 修复bulge偏移 ── */
	void computePolyline()
	{
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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbPolyline)
			else if (isAcDbPolyline == true) {
				switch (record->key) {
				case 38:
					elevation = AcString::toFloat64(record->value.c_str());
					break;
				case 70:
					if (AcString::toInt32(record->value) == 1) { isClosed = true; }
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

					if (abs(startWidth) < 0.00001 && abs(endWidth) < 0.00001) {
						startWidth = globalWidth;
						endWidth = globalWidth;
					}

					/* 修复: bulge与当前顶点对齐,不再偏移+1 */
					bulges.append(bulge);
					startWidths.append(startWidth);
					endWidths.append(endWidth);
					vertexs.append(vertex);

					bulge = 0;
					startWidth = 0;
					endWidth = 0;
					break;
				case 210: normal.x = AcString::toFloat64(record->value.c_str()); break;
				case 220: normal.y = AcString::toFloat64(record->value.c_str()); break;
				case 230: normal.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbPolyline* pEntity = new DbPolyline();
		for (int i = 0; i < vertexs.length(); i++) {
			pEntity->addVertexAt(i, vertexs[i], bulges[i], startWidths[i], endWidths[i]);
		}
		pEntity->setElevation(elevation);
		pEntity->setNormal(normal.normalize());
		pEntity->setClosed(isClosed);
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── SOLID ── */
	void computeSolid()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GePoint3d vertex = GePoint3d::kOrigin;
		GePoint3dArray vertexs;
		double thickness = 0;
		GeVector3d normal = GeVector3d::kZAxis;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbSolid = false;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbSolid)
			else if (isAcDbSolid == true) {
				switch (record->key) {
				case 10: vertex = GePoint3d::kOrigin; vertex.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: vertex.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: vertex.z = AcString::toFloat64(record->value.c_str()); vertexs.append(vertex); break;
				case 11: vertex = GePoint3d::kOrigin; vertex.x = AcString::toFloat64(record->value.c_str()); break;
				case 21: vertex.y = AcString::toFloat64(record->value.c_str()); break;
				case 31: vertex.z = AcString::toFloat64(record->value.c_str()); vertexs.append(vertex); break;
				case 12: vertex = GePoint3d::kOrigin; vertex.x = AcString::toFloat64(record->value.c_str()); break;
				case 22: vertex.y = AcString::toFloat64(record->value.c_str()); break;
				case 32: vertex.z = AcString::toFloat64(record->value.c_str()); vertexs.append(vertex); break;
				case 13: vertex = GePoint3d::kOrigin; vertex.x = AcString::toFloat64(record->value.c_str()); break;
				case 23: vertex.y = AcString::toFloat64(record->value.c_str()); break;
				case 33: vertex.z = AcString::toFloat64(record->value.c_str()); vertexs.append(vertex); break;
				case 39: thickness = AcString::toFloat64(record->value.c_str()); break;
				case 210: normal.x = AcString::toFloat64(record->value.c_str()); break;
				case 220: normal.y = AcString::toFloat64(record->value.c_str()); break;
				case 230: normal.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbSolid* pEntity = new DbSolid();
		pEntity->setNormal(normal);
		for (int i = 0; i < vertexs.length(); i++) {
			pEntity->setPointAt(i, vertexs[i]);
		}
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── VIEWPORT ── */
	void computeViewport()
	{
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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbViewport)
			else if (isAcDbViewport == true) {
				switch (record->key) {
				case 10: centerPoint.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: centerPoint.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: centerPoint.z = AcString::toFloat64(record->value.c_str()); break;
				case 40: width = AcString::toFloat64(record->value.c_str()); break;
				case 41: height = AcString::toFloat64(record->value.c_str()); break;
				case 12: viewCenter.x = AcString::toFloat64(record->value.c_str()); break;
				case 22: viewCenter.y = AcString::toFloat64(record->value.c_str()); break;
				case 13: snapBasePoint.x = AcString::toFloat64(record->value.c_str()); break;
				case 23: snapBasePoint.y = AcString::toFloat64(record->value.c_str()); break;
				case 14: snapIncrement.x = AcString::toFloat64(record->value.c_str()); break;
				case 24: snapIncrement.y = AcString::toFloat64(record->value.c_str()); break;
				case 15: gridIncrement.x = AcString::toFloat64(record->value.c_str()); break;
				case 25: gridIncrement.y = AcString::toFloat64(record->value.c_str()); break;
				case 16: viewDirection.x = AcString::toFloat64(record->value.c_str()); break;
				case 26: viewDirection.y = AcString::toFloat64(record->value.c_str()); break;
				case 36: viewDirection.z = AcString::toFloat64(record->value.c_str()); break;
				case 17: viewTarget.x = AcString::toFloat64(record->value.c_str()); break;
				case 27: viewTarget.y = AcString::toFloat64(record->value.c_str()); break;
				case 37: viewTarget.z = AcString::toFloat64(record->value.c_str()); break;
				case 42: lensLength = AcString::toFloat64(record->value.c_str()); break;
				case 45: viewHeight = AcString::toFloat64(record->value.c_str()); break;
				case 69: number = AcString::toInt32(record->value); break;
				}
			}
			this->curRecord++;
		}

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


	/* ── TEXT ── */
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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbText)
			else if (isAcDbText == true) {
				switch (record->key) {
				case 1:  textString = record->value; break;
				case 7:  textStyle = record->value; break;
				case 50: rotation = AcString::toFloat64(record->value.c_str()); break;
				case 10: position.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: position.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: position.z = AcString::toFloat64(record->value.c_str()); break;
				case 40: height = AcString::toFloat64(record->value.c_str()); break;
				case 41: widthFactor = AcString::toFloat64(record->value.c_str()); break;
				case 210: normal.x = AcString::toFloat64(record->value.c_str()); break;
				case 220: normal.y = AcString::toFloat64(record->value.c_str()); break;
				case 230: normal.z = AcString::toFloat64(record->value.c_str()); break;
				};
			}
			this->curRecord++;
		}

		DbText* pEntity = new DbText();
		pEntity->setWidthFactor(widthFactor);
		pEntity->setHeight(height);
		pEntity->setNormal(normal);
		pEntity->setRotation(angleToRadian(rotation));
		pEntity->setTextString(textString);
		if (textStyle.length() == 0) {
			textStyle = this->defaultTextStyle;
		}
		DbObjectId textStyleId = this->textStyleTableRecordMap->find(textStyle);
		pEntity->setTextStyle(textStyleId);
		pEntity->setPosition(transformByNormal(position, normal));
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── MTEXT —— 修复: rotation统一转换一次 ── */
	void computeMText()
	{
		PRDXF_DECLARE_ACDBENTITY;
		AcString contents;
		GePoint3d location;
		double height = 0.0;
		double textHeight = 0.0;
		std::string textStyle;
		GeVector3d normal(0, 0, 1);
		double rotation = 0.0;
		DbMText::AttachmentPoint attachmentPoint = DbMText::AttachmentPoint::kTopLeft;
		DbMText::FlowDirection flowDirection = DbMText::FlowDirection::kByStyle;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbMText = false;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbMText)
			else if (isAcDbMText == true) {
				switch (record->key) {
				case 1:  contents = record->value; break;
				case 7:  textStyle = record->value; break;
				case 10: location.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: location.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: location.z = AcString::toFloat64(record->value.c_str()); break;
				case 40: textHeight = AcString::toFloat64(record->value.c_str()); break;
				/* DXF group 50 for MTEXT = rotation in radians, 不需额外转换 */
				case 50: rotation = AcString::toFloat64(record->value.c_str()); break;
				case 71: attachmentPoint = (DbMText::AttachmentPoint)AcString::toInt32(record->value); break;
				case 72: flowDirection = (DbMText::FlowDirection)AcString::toInt32(record->value); break;
				case 210: normal.x = AcString::toFloat64(record->value.c_str()); break;
				case 220: normal.y = AcString::toFloat64(record->value.c_str()); break;
				case 230: normal.z = AcString::toFloat64(record->value.c_str()); break;
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
		/* 修复: MTEXT group 50 已是弧度,只需直接设置 */
		pEntity->setRotation(rotation);
		pEntity->setLocation(transformByNormal(location, normal));
		PRDXF_SET_ACDBENTITY(pEntity);
	}


	/* ── INSERT (BlockReference) ── */
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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbBlockReference)
			else if (isAcDbBlockReference == true) {
				switch (record->key) {
				case 2:  blockName = record->value; break;
				case 10: position.x = AcString::toFloat64(record->value.c_str()); break;
				case 20: position.y = AcString::toFloat64(record->value.c_str()); break;
				case 30: position.z = AcString::toFloat64(record->value.c_str()); break;
				case 41: scale.sx = AcString::toFloat64(record->value.c_str()); break;
				case 42: scale.sy = AcString::toFloat64(record->value.c_str()); break;
				case 43: scale.sz = AcString::toFloat64(record->value.c_str()); break;
				case 50: rotation = AcString::toFloat64(record->value.c_str()); break;
				case 210: normal.x = AcString::toFloat64(record->value.c_str()); break;
				case 220: normal.y = AcString::toFloat64(record->value.c_str()); break;
				case 230: normal.z = AcString::toFloat64(record->value.c_str()); break;
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


	/* ── DIMENSION —— 使用状态枚举替代10个bool标志 ── */
	void computeDimension()
	{
		PRDXF_DECLARE_ACDBENTITY;

		enum DimSubclass {
			kNone,
			kDimBase,
			kAligned,
			kRotated,
			kArcDim,
			kOrdinate,
			kRadial,
			kRadialLarge,
			kDiametric,
			k2LineAngular,
			k3PointAngular,
		};
		DimSubclass dimState = kNone;

		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbDimension = false;
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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }

			/* 子类切换(group code 100) */
			if (record->key == 100) {
				if (record->value == "AcDbEntity") {
					isAcDbObject = false;
					isAcDbEntity = true;
				}
				else if (record->value == "AcDbDimension") {
					isAcDbEntity = false;
					isAcDbDimension = true;
					dimState = kDimBase;
				}
				else if (record->value == "AcDbAlignedDimension")  { dimState = kAligned; isAcDbDimension = false; }
				else if (record->value == "AcDbRotatedDimension")  { dimState = kRotated; isAcDbDimension = false; }
				else if (record->value == "AcDbArcDimension")      { dimState = kArcDim; isAcDbDimension = false; }
				else if (record->value == "AcDbOrdinateDimension")   { dimState = kOrdinate; isAcDbDimension = false; }
				else if (record->value == "AcDbRadialDimension")     { dimState = kRadial; isAcDbDimension = false; }
				else if (record->value == "AcDbRadialDimensionLarge"){ dimState = kRadialLarge; isAcDbDimension = false; }
				else if (record->value == "AcDbDiametricDimension")  { dimState = kDiametric; isAcDbDimension = false; }
				else if (record->value == "AcDb2LineAngularDimension") { dimState = k2LineAngular; isAcDbDimension = false; }
				else if (record->value == "AcDb3PointAngularDimension") { dimState = k3PointAngular; isAcDbDimension = false; }
				this->curRecord++;
				continue;
			}

			/* AcDbObject */
			if (isAcDbObject == true) {
				switch (record->key) {
				case 5:   handle = std::stoi(record->value, NULL, 16); break;
				case 330: blockHandle = stol(record->value, NULL, 16); break;
				}
			}
			/* AcDbEntity */
			else if (isAcDbEntity == true) {
				switch (record->key) {
				case 8:   layerName = record->value; break;
				case 6:   linetypeName = record->value; break;
				case 60:  if (AcString::toInt32(record->value) != 0) { visibility = Db::Visibility::kInvisible; } break;
				case 62:  colorIndex = AcString::toInt32(record->value); break;
				case 370: lineWeight = (Db::LineWeight)AcString::toInt32(record->value); break;
				}
			}
			/* AcDbDimension 基类字段 */
			else if (isAcDbDimension == true || dimState == kDimBase) {
				switch (record->key) {
				case 2:  blockName = record->value; break;
				case 11: textPosition.x = AcString::toFloat64(record->value.c_str()); break;
				case 21: textPosition.y = AcString::toFloat64(record->value.c_str()); break;
				case 31: textPosition.z = AcString::toFloat64(record->value.c_str()); break;
				case 71: textAttachment = (DbMText::AttachmentPoint)AcString::toInt32(record->value); break;
				case 42: measurement = AcString::toFloat64(record->value.c_str()); break;
				case 3:  dimStyleName = record->value; break;
				}
			}
			/* AcDbAlignedDimension / AcDbRotatedDimension */
			else if (dimState == kAligned || dimState == kRotated) {
				switch (record->key) {
				case 13: xLine1Point.x = AcString::toFloat64(record->value.c_str()); break;
				case 23: xLine1Point.y = AcString::toFloat64(record->value.c_str()); break;
				case 33: xLine1Point.z = AcString::toFloat64(record->value.c_str()); break;
				case 14: xLine2Point.x = AcString::toFloat64(record->value.c_str()); break;
				case 24: xLine2Point.y = AcString::toFloat64(record->value.c_str()); break;
				case 34: xLine2Point.z = AcString::toFloat64(record->value.c_str()); break;
				case 50: rotation = AcString::toFloat64(record->value.c_str()); break;
				}
			}
			/* AcDbArcDimension */
			else if (dimState == kArcDim) {
				switch (record->key) {
				case 13: xLine1Point.x = AcString::toFloat64(record->value.c_str()); break;
				case 23: xLine1Point.y = AcString::toFloat64(record->value.c_str()); break;
				case 33: xLine1Point.z = AcString::toFloat64(record->value.c_str()); break;
				case 14: xLine2Point.x = AcString::toFloat64(record->value.c_str()); break;
				case 24: xLine2Point.y = AcString::toFloat64(record->value.c_str()); break;
				case 34: xLine2Point.z = AcString::toFloat64(record->value.c_str()); break;
				case 16: leader1Point.x = AcString::toFloat64(record->value.c_str()); break;
				case 26: leader1Point.y = AcString::toFloat64(record->value.c_str()); break;
				case 36: leader1Point.z = AcString::toFloat64(record->value.c_str()); break;
				case 17: leader2Point.x = AcString::toFloat64(record->value.c_str()); break;
				case 27: leader2Point.y = AcString::toFloat64(record->value.c_str()); break;
				case 37: leader2Point.z = AcString::toFloat64(record->value.c_str()); break;
				}
			}

			this->curRecord++;
		};

		/* 根据状态创建对应标注实体 */
		DbDimension* pEntity = NULL;
		if (dimState == kAligned) {
			pEntity = new DbAlignedDimension();
			((DbAlignedDimension*)(pEntity))->setXLine1Point(xLine1Point);
			((DbAlignedDimension*)(pEntity))->setXLine2Point(xLine2Point);
		}
		else if (dimState == kRotated) {
			pEntity = new DbRotatedDimension();
			((DbRotatedDimension*)(pEntity))->setRotation(angleToRadian(rotation));
			((DbRotatedDimension*)(pEntity))->setXLine1Point(xLine1Point);
			((DbRotatedDimension*)(pEntity))->setXLine2Point(xLine2Point);
		}
		else if (dimState == kArcDim) {
			pEntity = new DbArcDimension();
			((DbArcDimension*)(pEntity))->setXLine1Point(xLine1Point);
			((DbArcDimension*)(pEntity))->setXLine2Point(xLine2Point);
			((DbArcDimension*)(pEntity))->setLeader1Point(leader1Point);
			((DbArcDimension*)(pEntity))->setLeader2Point(leader2Point);
		}
		else if (dimState == kOrdinate) {
			pEntity = new DbOrdinateDimension();
		}
		else if (dimState == kRadial) {
			pEntity = new DbRadialDimension();
		}
		else if (dimState == kRadialLarge) {
			pEntity = new DbRadialDimensionLarge();
		}
		else if (dimState == kDiametric) {
			pEntity = new DbDiametricDimension();
		}
		else if (dimState == k2LineAngular) {
			pEntity = new Db2LineAngularDimension();
		}
		else if (dimState == k3PointAngular) {
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


	/* ── HATCH —— 基础解析,跳过复杂边界数据,记录基本属性 ── */
	void computeHatch()
	{
		PRDXF_DECLARE_ACDBENTITY;
		GeVector3d normal = GeVector3d::kZAxis;
		double elevation = 0.0;
		std::string patternName;
		int solidFill = 0;
		bool isAcDbObject = true;
		bool isAcDbEntity = false;
		bool isAcDbHatch = false;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			PRDXF_READ_ACDBOBJECT(isAcDbEntity)
			PRDXF_READ_ACDBENTITY(isAcDbHatch)
			else if (isAcDbHatch == true) {
				switch (record->key) {
				case 30: elevation = AcString::toFloat64(record->value.c_str()); break;
				case 210: normal.x = AcString::toFloat64(record->value.c_str()); break;
				case 220: normal.y = AcString::toFloat64(record->value.c_str()); break;
				case 230: normal.z = AcString::toFloat64(record->value.c_str()); break;
				case 2: patternName = record->value; break;
				case 70: solidFill = AcString::toInt32(record->value); break;
				};
			}
			this->curRecord++;
		}

		/* Hatch暂不创建实体(DbHatch类未实现),仅跳过记录避免解析错误 */
	}


	/* ── 实体分发 ── */
	static void computeEntity(PrDxfComputeEntityTask* thread)
	{
		for (; thread->curRecord < (int)thread->records.size(); thread->curRecord++)
		{
			int code = thread->records[thread->curRecord]->key;
			if      (code == PrDxfRecord::EDxfCode::kPoint)                 { thread->computePoint(); }
			else if (code == PrDxfRecord::EDxfCode::kLine)                  { thread->computeLine(); }
			else if (code == PrDxfRecord::EDxfCode::kXLine)                 { thread->computeXline(); }
			else if (code == PrDxfRecord::EDxfCode::kRay)                   { thread->computeRay(); }
			else if (code == PrDxfRecord::EDxfCode::kCircle)                { thread->computeCircle(); }
			else if (code == PrDxfRecord::EDxfCode::kArc)                   { thread->computeArc(); }
			else if (code == PrDxfRecord::EDxfCode::kEllipse)               { thread->computeEllipse(); }
			else if (code == PrDxfRecord::EDxfCode::kText)                  { thread->computeText(); }
			else if (code == PrDxfRecord::EDxfCode::kMText)                 { thread->computeMText(); }
			else if (code == PrDxfRecord::EDxfCode::kBlockReference)        { thread->computeBlockReference(); }
			else if (code == PrDxfRecord::EDxfCode::kDimension)             { thread->computeDimension(); }
			else if (code == PrDxfRecord::EDxfCode::kArcDimension)          { thread->computeArcDimension(); }
			else if (code == PrDxfRecord::EDxfCode::kRadialDimensionLarge)  { thread->computeRadialDimensionLarge(); }
			else if (code == PrDxfRecord::EDxfCode::kPolyline)              { thread->computePolyline(); }
			else if (code == PrDxfRecord::EDxfCode::kSolid)                 { thread->computeSolid(); }
			else if (code == PrDxfRecord::EDxfCode::kViewport)              { thread->computeViewport(); }
			else if (code == PrDxfRecord::EDxfCode::kHatch)                 { thread->computeHatch(); }
		}

		thread->isDone.store(true);
	}
};


/* ── 块段任务复用实体任务 ── */
class PrDxfComputeBlockTask :public PrDxfComputeEntityTask {
public:
	PrDxfComputeBlockTask() {}
	virtual ~PrDxfComputeBlockTask() {}
};


/* ============================================================================
 * 计算对象段任务
 * ============================================================================ */

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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 5)   { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 330) { parentHandle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 3)   { ownerName = record->value; }
			else if (record->key == 350) {
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
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 5)   { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 330 && isAcDbLayout == false) { parentHandle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 330 && isAcDbLayout == true)  { blockHandle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 331 && isAcDbLayout == true)  { viewHandle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 1)   { layoutName = record->value; }
			else if (record->key == 100 && record->value == "AcDbLayout") { isAcDbLayout = true; }

			this->curRecord++;
		}

		DbLayout* pLayout = new DbLayout();
		pLayout->setLayoutName(layoutName);

		DbObjectId blockTableRecordId(0);
		this->database->getDbObjectId(blockTableRecordId, false, blockHandle);
		DB_IMP_LAYOUT(pLayout->m_pImpl)->blockTableRecordId = blockTableRecordId;

		DbObjectId viewportId(0);
		if (viewHandle > 0) {
			this->database->getDbObjectId(viewportId, false, viewHandle);
			DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId = viewportId;
		}
		::setDbObjectHandle(pLayout, handle);

		PrObjectInidex* objectInidex = new PrObjectInidex(pLayout);
		objectInidex->parentHandle = parentHandle;
		this->objects.push_back(objectInidex);
	}

	void readDbXdata() {

		unsigned int handle = 0;
		unsigned int parentHandle = 0;
		while (this->curRecord < (int)this->records.size())
		{
			PrDxfRecord* record = this->records[this->curRecord];
			::readRecordValue(this->content, record);

			if (record->key == PrDxfRecord::kObjectMark) { break; }
			else if (record->key == 5)   { handle = std::stoi(record->value, NULL, 16); }
			else if (record->key == 330) { parentHandle = std::stoi(record->value, NULL, 16); }

			this->curRecord++;
		}
	}

	static void computeObject(PrDxfComputeObjectTask* thread)
	{
		for (; thread->curRecord < (int)thread->records.size(); thread->curRecord++)
		{
			int code = thread->records[thread->curRecord]->key;
			if      (code == PrDxfRecord::EDxfCode::kDictionary) { thread->readDbDictionary(); }
			else if (code == PrDxfRecord::EDxfCode::kLayout)     { thread->readDbLayout(); }
			else if (code == PrDxfRecord::EDxfCode::kXdata)      { thread->readDbXdata(); }
		}

		thread->isDone.store(true);
	}
};



/* ============================================================================
 * PrDXFParser 公开接口
 * ============================================================================ */

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
			if (pLayerTableRecord == NULL) { continue; }

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
			if (pTextStyleTableRecord == NULL) { continue; }

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
			if (pBlockTableRecord == NULL) { continue; }

			AcString blockName;
			pBlockTableRecord->getName(blockName);
			DbObjectId blockId = pBlockTableRecord->objectId();
			blockTableRecordMap->insert(blockName.string(), blockId);

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
			if (pDimStyleTableRecord == NULL) { continue; }

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


/* ============================================================================
 * 表对象创建辅助(消除 computeXxxTable 的重复)
 * ============================================================================ */

/* 通用: 从零记录之后读取表头的句柄 */
static unsigned int readTableHandle(PrDXFParserImpl* impl)
{
	unsigned int handle = 0;
	int index = impl->zeroRecords[impl->curZeroRecord] + 1;
	for (int i = index; i < (int)impl->records.size(); i++)
	{
		PrDxfRecord* curRecord = impl->records[index];
		::readRecordValue(impl->buffers, curRecord);

		if (impl->records[index]->key == 0) { break; }
		else if (impl->records[index]->key == 5) {
			handle = std::stoi(impl->records[index]->value, NULL, 16);
		}
		index++;
	}
	return handle;
}


void PrDXFParser::computeViewTable() {
	unsigned int handle = readTableHandle(this->m_pImpl);
	DbViewTable* pTable = new DbViewTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pViewTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeLinetypeTable() {
	unsigned int handle = readTableHandle(this->m_pImpl);
	DbLinetypeTable* pTable = new DbLinetypeTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pLinetypeTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeLayerTable() {
	unsigned int handle = readTableHandle(this->m_pImpl);
	DbLayerTable* pTable = new DbLayerTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pLayerTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeTextStyleTable() {
	unsigned int handle = readTableHandle(this->m_pImpl);
	DbTextStyleTable* pTable = new DbTextStyleTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pTextStyleTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeViewportTable() {
	unsigned int handle = readTableHandle(this->m_pImpl);
	DbViewportTable* pTable = new DbViewportTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pViewportTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeUCSTable() {
	unsigned int handle = readTableHandle(this->m_pImpl);
	DbUCSTable* pTable = new DbUCSTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pUCSTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeAppidTable() {
	unsigned int handle = readTableHandle(this->m_pImpl);
	/* AppidTable不创建,保持原行为 */
}
void PrDXFParser::computeDimStyleTable() {
	unsigned int handle = readTableHandle(this->m_pImpl);
	DbDimStyleTable* pTable = new DbDimStyleTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pDimStyleTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}
void PrDXFParser::computeBlockTable() {
	unsigned int handle = readTableHandle(this->m_pImpl);
	DbBlockTable* pTable = new DbBlockTable();
	::setDbObjectHandle(pTable, handle);
	((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pBlockTable = pTable;
	this->m_pImpl->database->addDbObject(pTable);
}


void PrDXFParser::computeTable(PrDxfRecord::EDxfCode code)
{

	std::string tableName = "View";
	PrDxfRecord::EDxfCode tableRecord = PrDxfRecord::EDxfCode::kViewTableRecord;
	if (code == PrDxfRecord::EDxfCode::kViewTable)             { tableName = "View";         tableRecord = PrDxfRecord::EDxfCode::kViewTableRecord; }
	else if (code == PrDxfRecord::EDxfCode::kLinetypeTable)    { tableName = "LTYPE";        tableRecord = PrDxfRecord::EDxfCode::kLinetypeTableRecord; }
	else if (code == PrDxfRecord::EDxfCode::kLayerTable)       { tableName = "LAYER";        tableRecord = PrDxfRecord::EDxfCode::kLayerTableRecord; }
	else if (code == PrDxfRecord::EDxfCode::kTextStyleTable)   { tableName = "STYLE";        tableRecord = PrDxfRecord::EDxfCode::kTextStyleTableRecord; }
	else if (code == PrDxfRecord::EDxfCode::kViewportTable)    { tableName = "VPORT";        tableRecord = PrDxfRecord::EDxfCode::kViewportTableRecord; }
	else if (code == PrDxfRecord::EDxfCode::kUCSTable)         { tableName = "UCS";          tableRecord = PrDxfRecord::EDxfCode::kUCSTableRecord; }
	else if (code == PrDxfRecord::EDxfCode::kAppidTable)       { tableName = "APPID";        tableRecord = PrDxfRecord::EDxfCode::kAppidTableRecord; }
	else if (code == PrDxfRecord::EDxfCode::kDimStyleTable)    { tableName = "DIMSTYLE";     tableRecord = PrDxfRecord::EDxfCode::kDimStyleTableRecord; }
	else if (code == PrDxfRecord::EDxfCode::kBlockTable)       { tableName = "BLOCK_RECORD"; tableRecord = PrDxfRecord::EDxfCode::kBlockTableRecord; }

	// 创建表对象
	if      (code == PrDxfRecord::EDxfCode::kViewportTable) { this->computeViewportTable(); }
	else if (code == PrDxfRecord::EDxfCode::kLinetypeTable) { this->computeLinetypeTable(); }
	else if (code == PrDxfRecord::EDxfCode::kLayerTable)    { this->computeLayerTable(); }
	else if (code == PrDxfRecord::EDxfCode::kTextStyleTable){ this->computeTextStyleTable(); }
	else if (code == PrDxfRecord::EDxfCode::kViewTable)     { this->computeViewTable(); }
	else if (code == PrDxfRecord::EDxfCode::kUCSTable)      { this->computeUCSTable(); }
	else if (code == PrDxfRecord::EDxfCode::kDimStyleTable) { this->computeDimStyleTable(); }
	else if (code == PrDxfRecord::EDxfCode::kBlockTable)    { this->computeBlockTable(); }

	std::vector<PrDxfObjectRecord*> objectRecords;
	for (int i = this->m_pImpl->curZeroRecord; i < (int)this->m_pImpl->zeroRecords.size(); i++)
	{
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
	for (int i = 0; i < (int)objectRecords.size(); i++) {
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
	for (int i = objectRecords.size() - 1; i >= 0; i--) {
		delete objectRecords[i];
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

	/* ── 将表记录添加到对应符号表(统一宏消除重复) ── */
#define ADD_TABLE_RECORDS(TableType, RecordType, getter) \
	{ \
		TableType* pTable = NULL; \
		this->m_pImpl->database->getter(pTable, Db::kForWrite); \
		if (pTable != NULL) { \
			for (int i = 0; i < computeTableTasks.length(); i++) { \
				for (int u = 0; u < (int)computeTableTasks[i]->objects.size(); u++) { \
					RecordType* pRecord = RecordType::cast(computeTableTasks[i]->objects[u]->object); \
					pTable->add(pRecord); \
					pRecord->close(); \
				} \
			} \
			pTable->close(); \
		} \
	}

	if      (code == PrDxfRecord::EDxfCode::kViewportTable) { ADD_TABLE_RECORDS(DbViewportTable, DbViewportTableRecord, getViewportTable); }
	else if (code == PrDxfRecord::EDxfCode::kLinetypeTable) { ADD_TABLE_RECORDS(DbLinetypeTable, DbLinetypeTableRecord, getLinetypeTable); }
	else if (code == PrDxfRecord::EDxfCode::kLayerTable)    { ADD_TABLE_RECORDS(DbLayerTable, DbLayerTableRecord, getLayerTable); }
	else if (code == PrDxfRecord::EDxfCode::kTextStyleTable){ ADD_TABLE_RECORDS(DbTextStyleTable, DbTextStyleTableRecord, getTextStyleTable); }
	else if (code == PrDxfRecord::EDxfCode::kViewTable)     { ADD_TABLE_RECORDS(DbViewTable, DbViewTableRecord, getViewTable); }
	else if (code == PrDxfRecord::EDxfCode::kUCSTable)      { ADD_TABLE_RECORDS(DbUCSTable, DbUCSTableRecord, getUCSTable); }
	else if (code == PrDxfRecord::EDxfCode::kDimStyleTable) { ADD_TABLE_RECORDS(DbDimStyleTable, DbDimStyleTableRecord, getDimStyleTable); }
	else if (code == PrDxfRecord::EDxfCode::kBlockTable)    { ADD_TABLE_RECORDS(DbBlockTable, DbBlockTableRecord, getBlockTable); }

#undef ADD_TABLE_RECORDS

	// 删除任务
	realloc_arry_all(computeTableTasks);
}


/* ============================================================================
 * computeRecords — 多线程解析DXF行→记录
 * ============================================================================ */

void PrDXFParser::computeRecords()
{

	// 获得换行位置
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
		for (int u = 0; u < (int)tempRecords.size(); u++)
		{
			this->m_pImpl->records.push_back(tempRecords[u]);
		}
		int startZero = i * single;
		std::vector<int> tempZeroRecords = computeRecords[i]->zeroRecordPos;
		for (int u = 0; u < (int)tempZeroRecords.size(); u++)
		{
			this->m_pImpl->zeroRecords.push_back(startZero + tempZeroRecords[u]);
		}
	}

	// 删除任务
	realloc_arry_all(computeRecords);
}


/* ============================================================================
 * computeHeaderSection
 * ============================================================================ */

void PrDXFParser::computeHeaderSection()
{
	// 获得所有头信息开始和结束点
	int startPos = 0;
	int endPos = 0;
	for (int i = 0; i < (int)this->m_pImpl->zeroRecords.size(); i++)
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


/* ============================================================================
 * computeTableSection
 * ============================================================================ */

void PrDXFParser::computeTableSection()
{

	// 获得段块开始点
	int startPos = 0;
	for (int i = this->m_pImpl->curZeroRecord; i < (int)this->m_pImpl->zeroRecords.size(); i++)
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


/* ============================================================================
 * 通用: 解析一个段(BLOCKS / ENTITIES)中的实体
 * 将block/entity段的重复代码合并为一个模板函数
 * ============================================================================ */

template<typename TaskType>
static void computeEntitySectionImpl(
	PrDXFParserImpl* impl,
	const char* sectionName,
	bool useEntityLookup)
{
	// 获得段开始点
	int startPos = 0;
	for (int i = impl->curZeroRecord; i < (int)impl->zeroRecords.size(); i++)
	{
		::readRecordValue(impl->buffers, impl->records[impl->zeroRecords[i]]);
		if (impl->records[impl->zeroRecords[i]]->value == "SECTION")
		{
			int nextRecord = impl->zeroRecords[i] + 1;
			::readRecordValue(impl->buffers, impl->records[nextRecord]);
			if (impl->records[nextRecord]->key == 2 && impl->records[nextRecord]->value == sectionName)
			{
				impl->curZeroRecord = i + 1;
				startPos = nextRecord;
				break;
			}
		}
	}
	if (startPos == 0) { return; }

	std::vector<PrDxfObjectRecord*> objectRecords;
	for (int i = impl->curZeroRecord; i < (int)impl->zeroRecords.size(); i++)
	{
		PrDxfRecord* curRecord = impl->records[impl->zeroRecords[i]];
		::readRecordValue(impl->buffers, curRecord);

		impl->curZeroRecord = i + 1;
		if (curRecord->value == "ENDSEC") { break; }

		/* 统一查找表映射 */
		PrDxfRecord::EDxfCode code = PrEntityNameMap::lookup(curRecord->value);
		if (code != PrDxfRecord::EDxfCode::kNone) {
			PrDxfObjectRecord* objectRecord = new PrDxfObjectRecord();
			objectRecord->code = code;
			objectRecord->startPos = impl->zeroRecords[i];
			objectRecord->endPos = impl->zeroRecords[i + 1];
			objectRecords.push_back(objectRecord);
		}
	}
	if (objectRecords.size() <= 0) { return; }

	// 创建任务
	AcArray<TaskType*> tasks;
	int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
	for (int i = 0; i < numWorkerThread - 1; i++) {
		TaskType* task = new TaskType();
		task->content = impl->buffers;
		task->database = impl->database;
		task->isMainThread = false;
		task->defaultTextStyle = impl->defaultTextStyle;
		task->blockTableRecordMap = impl->blockTableRecordMap;
		task->layerTableRecordMap = impl->layerTableRecordMap;
		task->textStyleTableRecordMap = impl->textStyleTableRecordMap;
		task->dimStyleTableRecordMap = impl->dimStyleTableRecordMap;
		tasks.append(task);
	}
	TaskType* mainTask = new TaskType();
	mainTask->content = impl->buffers;
	mainTask->database = impl->database;
	mainTask->isMainThread = true;
	mainTask->defaultTextStyle = impl->defaultTextStyle;
	mainTask->blockTableRecordMap = impl->blockTableRecordMap;
	mainTask->layerTableRecordMap = impl->layerTableRecordMap;
	mainTask->textStyleTableRecordMap = impl->textStyleTableRecordMap;
	mainTask->dimStyleTableRecordMap = impl->dimStyleTableRecordMap;
	tasks.append(mainTask);

	// 分配任务(round-robin)
	int indexThread = 0;
	for (int i = 0; i < (int)objectRecords.size(); i++) {
		tasks[indexThread]->records.push_back(new PrDxfRecord(objectRecords[i]->code));
		for (int u = objectRecords[i]->startPos; u < objectRecords[i]->endPos; u++) {
			tasks[indexThread]->records.push_back(impl->records[u]);
		}
		tasks[indexThread]->records.push_back(new PrDxfRecord(PrDxfRecord::EDxfCode::kObjectMark));

		indexThread++;
		if (indexThread >= tasks.length()) { indexThread = 0; }
	}

	// 释放objectRecords内存
	for (int i = objectRecords.size() - 1; i >= 0; i--) { delete objectRecords[i]; }

	// 启动任务
	for (int i = 0; i < tasks.length(); i++) {
		if (tasks[i]->isMainThread == true) {
			TaskType::computeEntity(tasks[i]);
		}
		else {
			TaskType* t = tasks[i];
			::kernel()->acrxWorkerThreadManager()->addTask([t]()->void {
				TaskType::computeEntity(t);
				}, true);
		}
	}

	// 等待任务完成
	for (int i = 0; i < tasks.length(); i++) {
		if (tasks[i]->isMainThread == false && tasks[i]->isDone.load() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			i = 0;
		}
	}

	// 设置对象(将实体添加到块表记录)
	for (int i = 0; i < tasks.length(); i++) {
		for (int u = 0; u < (int)tasks[i]->objects.size(); u++) {
			DbHandleTableRecord* record = ::kernel()->acdbObjectManager()->queryHandleTableRecord(
				impl->database, tasks[i]->objects[u]->parentHandle);
			if (record == NULL) { continue; }
			DbObject* object = ::kernel()->acdbObjectManager()->openDbObject(record->objectId);
			if (object == NULL) { continue; }

			if (DB_IMP_OBJECT(object->m_pImpl)->isBlockTableRecord()) {
				DbBlockTableRecord* pBlockTableRecord = (DbBlockTableRecord*)object;
				DbObjectId entityId(0);
				pBlockTableRecord->appendDbEntity(entityId, (DbEntity*)tasks[i]->objects[u]->object);
			}
		}
	}

	// 删除任务
	realloc_arry_all(tasks);
}


void PrDXFParser::computeBlockSection()
{
	computeEntitySectionImpl<PrDxfComputeBlockTask>(this->m_pImpl, "BLOCKS", true);
}

void PrDXFParser::computeEntitySection()
{
	computeEntitySectionImpl<PrDxfComputeEntityTask>(this->m_pImpl, "ENTITIES", true);
}


/* ============================================================================
 * computeObjectSection
 * ============================================================================ */

void PrDXFParser::computeObjectSection() {

	// 获得段开始点
	int startPos = 0;
	for (int i = this->m_pImpl->curZeroRecord; i < (int)this->m_pImpl->zeroRecords.size(); i++)
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
	if (startPos == 0) { return; }

	std::vector<PrDxfObjectRecord*> objectRecords;
	for (int i = this->m_pImpl->curZeroRecord; i < (int)this->m_pImpl->zeroRecords.size(); i++)
	{
		PrDxfRecord* curRecord = this->m_pImpl->records[this->m_pImpl->zeroRecords[i]];
		::readRecordValue(this->m_pImpl->buffers, curRecord);

		this->m_pImpl->curZeroRecord = i + 1;
		if (curRecord->value == "ENDSEC") { break; }

		/* 使用对象名查找表 */
		PrDxfRecord::EDxfCode code = PrObjectNameMap::lookup(curRecord->value);
		if (code != PrDxfRecord::EDxfCode::kNone) {
			PrDxfObjectRecord* objectRecord = new PrDxfObjectRecord();
			objectRecord->code = code;
			objectRecord->startPos = this->m_pImpl->zeroRecords[i];
			objectRecord->endPos = this->m_pImpl->zeroRecords[i + 1];
			objectRecords.push_back(objectRecord);
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
	for (int i = 0; i < (int)objectRecords.size(); i++) {
		computeObjectTasks[indexThread]->records.push_back(new PrDxfRecord(objectRecords[i]->code));
		for (int u = objectRecords[i]->startPos; u < objectRecords[i]->endPos; u++) {
			computeObjectTasks[indexThread]->records.push_back(this->m_pImpl->records[u]);
		}
		computeObjectTasks[indexThread]->records.push_back(new PrDxfRecord(PrDxfRecord::EDxfCode::kObjectMark));

		indexThread++;
		if (indexThread >= computeObjectTasks.length()) { indexThread = 0; }
	}

	// 释放objectRecords
	for (int i = objectRecords.size() - 1; i >= 0; i--) { delete objectRecords[i]; }

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

		bool isAppend = false;
		for (int i = 0; i < dictionaryIndexs.length(); i++) {

			if (dictionaryIndexs[i]->isAppend == true) { continue; }
			if (dictionaryIndexs[i]->parentHandle == 0) { continue; }

			DbObjectId parentId(0);
			this->m_pImpl->database->getDbObjectId(parentId, false, dictionaryIndexs[i]->parentHandle);
			if (parentId.isNull()) { continue; }

			DbObject* parentObject = ::kernel()->acdbObjectManager()->openDbObject(parentId);
			if (parentObject == NULL) { continue; }

			if (DB_IMP_OBJECT(parentObject->m_pImpl)->isDictionary()) {

				for (int u = 0; u < dictionaryIndexs.length(); u++) {
					if (dictionaryIndexs[u]->dictionary != parentObject) { continue; }
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
			else {
				isAppend = true;
				dictionaryIndexs[i]->isAppend = true;
				this->m_pImpl->database->addDbObject(dictionaryIndexs[i]->dictionary);
				DB_IMP_OBJECT(parentObject->m_pImpl)->extensionDictionaryId = dictionaryIndexs[i]->dictionary->objectId();
			}
		}
		if (isAppend == false) { break; }

		AcArray<PrDictionaryRecord*> tempDictionaryIndexs;
		for (int i = 0; i < dictionaryIndexs.length(); i++) {
			if (dictionaryIndexs[i]->isAppend == false) {
				tempDictionaryIndexs.append(dictionaryIndexs[i]);
			}
			else {
				delete dictionaryIndexs[i];
			}
		}
		dictionaryIndexs.removeAll();
		for (int i = 0; i < tempDictionaryIndexs.length(); i++) {
			dictionaryIndexs.append(tempDictionaryIndexs[i]);
		}
	};
	for (int i = dictionaryIndexs.length() - 1; i >= 0; i--) {
		delete dictionaryIndexs[i];
	}

	// 添加对象
	bool isLayout = false;
	DbDictionary* pRootDictionary = ((DbDatabaseImpl*)this->m_pImpl->database->pImpl)->pRootDictionary;
	for (int i = 0; i < computeObjectTasks.length(); i++) {
		for (int u = 0; u < (int)computeObjectTasks[i]->objects.size(); u++) {

			DbHandleTableRecord* record = ::kernel()->acdbObjectManager()->queryHandleTableRecord(
				this->m_pImpl->database, computeObjectTasks[i]->objects[u]->parentHandle);
			if (record == NULL) { continue; }
			DbObject* parentObject = ::kernel()->acdbObjectManager()->openDbObject(record->objectId);
			if (parentObject == NULL) { continue; }

			if (DB_IMP_OBJECT(parentObject->m_pImpl)->isDictionary() &&
				DB_IMP_OBJECT(computeObjectTasks[i]->objects[u]->object->m_pImpl)->isLayout()) {

				DbDictionary* pLayoutDictionary = (DbDictionary*)parentObject;
				DbLayout* pLayout = (DbLayout*)computeObjectTasks[i]->objects[u]->object;

				DbObjectId blockTableRecordId = pLayout->getBlockTableRecordId();
				if (!blockTableRecordId.isNull()) {
					DbBlockTableRecord* pBlockTableRecord = (DbBlockTableRecord*)::kernel()->acdbObjectManager()->openDbObject(blockTableRecordId);
					if (pBlockTableRecord != NULL) {
						DB_IMP_BLOCKTABLERECORD(pBlockTableRecord->m_pImpl)->layoutId = pLayout->objectId();
					}
				}

				char* layoutName = NULL;
				pLayout->getLayoutName(layoutName);
				DbObjectId layoutId(0);
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
		if (layout == NULL) { continue; }

		AcString layouName;
		layout->getName(layouName);

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
}
