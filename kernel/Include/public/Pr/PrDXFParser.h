#ifndef PrDXFParser_H
#define PrDXFParser_H

#include "vector"
#include "AcString.h"
#include "arxHeaders.h"


class PrDxfRecord
{
public:
	PrDxfRecord(int _key = 0, std::string _val = "")
	{
		this->key = _key;
		this->value = _val;
		this->isValue = false;
		this->satrt = 0;
		this->end = 0;
	}
	~PrDxfRecord()
	{
	}

	enum EDxfCode
	{

		kNone = 1048576,

		kHeader = kNone + 1,
		kObjectMark,
		kObjectEnd,

		kViewportTable = kNone + 100,
		kLinetypeTable,
		kLayerTable,
		kTextStyleTable,
		kViewTable,
		kUCSTable,
		kAppidTable,
		kDimStyleTable,
		kBlockTable,

		kViewportTableRecord = kNone + 200,
		kLinetypeTableRecord,
		kLayerTableRecord,
		kTextStyleTableRecord,
		kViewTableRecord,
		kUCSTableRecord,
		kAppidTableRecord,
		kDimStyleTableRecord,
		kBlockTableRecord,

		kBlockReference = kNone + 1000,
		kLine,
		kCircle,
		kArc,
		kPoint,
		kPolyline,
		kText,
		kMText,
		kSolid,
		kDimension,
		kArcDimension,
		kRadialDimensionLarge,
		kXLine,
		kRay,
		kEllipse,
		kLeader,
		kSpline,
		kAttdef,
		kViewport,
		kHatch,

		kDictionary,
		kXdata,
		kLayout,
	};

	int key;
	std::string value;
	bool isValue;
	int satrt;
	int end;
};

class PrDXFParserImpl;
class PrDXFParser
{
public:
	PrDXFParser();
	~PrDXFParser();

private:

	void computeViewTable();
	void computeLinetypeTable();
	void computeLayerTable();
	void computeTextStyleTable();
	void computeViewportTable();
	void computeUCSTable();
	void computeAppidTable();
	void computeDimStyleTable();
	void computeBlockTable();
	void computeTable(PrDxfRecord::EDxfCode code);

	void computeRecords();
	void computeHeaderSection();
	void computeTableSection();
	void computeBlockSection();
	void computeEntitySection();
	void computeObjectSection();

public:
	DbDatabase *load(std::string _fileName);
	DbDatabase *loadBuffer(std::string _buff);
	DbDatabase *loadBuffer(const char *_buff, unsigned int len);

public:
	PrDXFParserImpl* m_pImpl;
};

#endif