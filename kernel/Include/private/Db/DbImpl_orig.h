#ifndef DBIMPL_H
#define DBIMPL_H

#include "Db.h"
#include "AcString.h"
#include "AcArray.h"
#include "AcMap.h"
#include "AcEvent.h"
#include "AcEmsdkEvent.h"
#include "AcBinary.h"
#include "AcBinaryFiler.h"
#include "AcIterator.h"
#include "RxImpl.h"
#include "RxObjectImpl.h"
#include "CmColor.h"
#include "CmTransparency.h"
#include "GePoint3d.h"
#include "GeVector3d.h"
#include "GeMatrix3d.h"
#include "GiTextStyle.h"
#include "DbHandle.h"
#include "DbObjectId.h"
#include "DbText.h"
#include "DbMText.h"
#include "DbEntityStake.h"
#include "DbHandleManage.h"
#include "DbLinetypeDashe.h"
#include "DbObjectIdArray.h"
#include "DbDictionaryRecord.h"
#include "DbLayoutManagerReactor.h"
#include "acutmem.h"
#include "adsdef.h"
#include "acutads.h"

#define DB_IMP_OBJECT(x) ((DbObjectImpl *)x)

#define DB_IMP_ENTITY(x) ((DbEntityImpl *)x)
#define DB_IMP_BLOCKREFERENCE(x) ((DbBlockReferenceImpl *)x)
#define DB_IMP_POINT(x) ((DbPointImpl *)x)
#define DB_IMP_CURVE(x) ((DbCurveImpl *)x)
#define DB_IMP_LINE(x) ((DbLineImpl *)x)
#define DB_IMP_XLINE(x) ((DbXlineImpl *)x)
#define DB_IMP_RAY(x) ((DbRayImpl *)x)
#define DB_IMP_CIRCLE(x) ((DbCircleImpl *)x)
#define DB_IMP_ARC(x) ((DbArcImpl *)x)
#define DB_IMP_ELLIPSE(x) ((DbEllipseImpl *)x)
#define DB_IMP_POLYLINE(x) ((DbPolylineImpl *)x)
#define DB_IMP_TEXT(x) ((DbTextImpl *)x)
#define DB_IMP_MTEXT(x) ((DbMTextImpl *)x)
#define DB_IMP_SOLID(x) ((DbSolidImpl *)x)
#define DB_IMP_VIEWPORT(x) ((DbViewportImpl *)x)

#define DB_IMP_SYMBOLTABLEITERATOR(x) ((DbSymbolTableIteratorImpl *)x)
#define DB_IMP_BLOCKTABLERECORDITERATOR(x) ((DbBlockTableRecordIteratorImpl *)x)
#define DB_IMP_DICTIONARYITERATOR(x) ((DbDictionaryIteratorImpl *)x)

#define DB_IMP_SYMBOLTABLE(x) ((DbSymbolTableImpl *)x)
#define DB_IMP_LAYERTABLE(x) ((DbLayerTableImpl *)x)
#define DB_IMP_BLOCKTABLE(x) ((DbBlockTableImpl *)x)
#define DB_IMP_DIMSTYLETABLE(x) ((DbDimStyleTableImpl *)x)
#define DB_IMP_LINETYPETABLE(x) ((DbLinetypeTableImpl *)x)
#define DB_IMP_REGAPPTABLE(x) ((DbRegAppTableImpl *)x)
#define DB_IMP_TEXTSTYLETABLE(x) ((DbTextStyleTableImpl *)x)
#define DB_IMP_UCSTABLE(x) ((DbUCSTableImpl *)x)
#define DB_IMP_VIEWPORTTABLE(x) ((DbViewportTableImpl *)x)
#define DB_IMP_VIEWTABLE(x) ((DbViewTableImpl *)x)
#define DB_IMP_DRAGENTITY(x) ((DbDragEntityImpl *)x)

#define DB_IMP_SYMBOLTABLERECORD(x) ((DbSymbolTableRecordImpl *)x)
#define DB_IMP_LAYERTABLERECORD(x) ((DbLayerTableRecordImpl *)x)
#define DB_IMP_BLOCKTABLERECORD(x) ((DbBlockTableRecordImpl *)x)
#define DB_IMP_DIMSTYLETABLERECORD(x) ((DbDimStyleTableRecordImpl *)x)
#define DB_IMP_LINETYPEDASHE(x) ((DbLinetypeDasheImpl *)x)
#define DB_IMP_LINETYPETABLERECORD(x) ((DbLinetypeTableRecordImpl *)x)
#define DB_IMP_REGAPPTABLERECORD(x) ((DbRegAppTableRecordImpl *)x)
#define DB_IMP_TEXTSTYLETABLERECORD(x) ((DbTextStyleTableRecordImpl *)x)
#define DB_IMP_UCSTABLERECORD(x) ((DbUCSTableRecordImpl *)x)
#define DB_IMP_ABSTRACTVIEWTABLERECORD(x) ((DbAbstractViewTableRecordImpl *)x)
#define DB_IMP_VIEWPORTTABLERECORD(x) ((DbViewportTableRecordImpl *)x)
#define DB_IMP_VIEWTABLERECORD(x) ((DbViewTableRecordImpl *)x)

#define DB_IMP_DICTIONARY(x) ((DbDictionaryImpl *)x)
#define DB_IMP_PLOTSETTINGS(x) ((DbPlotSettingsImpl *)x)
#define DB_IMP_LAYOUT(x) ((DbLayoutImpl *)x)
#define DB_IMP_LAYOUTMANAGER(x) ((DbLayoutManagerImpl *)x)
#define DB_IMP_LAYOUTMANAGERREACTOR(x) ((DbLayoutManagerReactorImpl *)x)

#define DB_IMP_OBJECTMANAGER(x) ((DbObjectManagerImpl *)x)
#define DB_IMP_HOSTAPPLICATIONSERVICES(x) ((DbHostApplicationServicesImpl *)x)

#define DB_IMP_FWGFILER(x) ((DbDwgFilerImpl *)x)
#define DB_IMP_FWGFILEFILER(x) ((DbDwgFileFilerImpl *)x)
#define DB_IMP_FWGUNDOFILER(x) ((DbDwgUndoFilerImpl *)x)
#define DB_IMP_FWGCOPYFILER(x) ((DbDwgCopyFilerImpl *)x)

#define DB_IMP_OBJECTREACTOR(x) ((DbObjectReactorImpl *)x)
#define DB_IMP_ENTITYREACTOR(x) ((DbEntityReactorImpl *)x)
#define DB_IMP_DATABASEREACTOR(x) ((DbDatabaseReactorImpl *)x)

#define DB_IMP_XRECORD(x) ((DbXrecordImpl *)x)

#define DB_IMP_DIMENSION(x) ((DbDimensionImpl *)x)
#define DB_IMP_ROTATEDDIMENSION(x) ((DbRotatedDimensionImpl *)x)
#define DB_IMP_ALIGNEDDIMENSION(x) ((DbAlignedDimensionImpl *)x)
#define DB_IMP_DIAMETRICDIMENSION(x) ((DbDiametricDimensionImpl *)x)
#define DB_IMP_RADIALDIMENSION(x) ((DbRadialDimensionImpl *)x)
#define DB_IMP_3POINTANGULARDIMENSION(x) ((Db3PointAngularDimensionImpl *)x)
#define DB_IMP_ORDINATEDIMENSION(x) ((DbOrdinateDimensionImpl *)x)
#define DB_IMP_ARCDIMENSION(x) ((DbArcDimensionImpl *)x)
#define DB_IMP_RADIALDIMENSIONLARGE(x) ((DbRadialDimensionLargeImpl *)x)
#define DB_IMP_2LINEANGULARDIMENSION(x) ((Db2LineAngularDimensionImpl *)x)

#define DB_IMP_OBJECTSTACKREACTOR(x) ((DbObjectStackReactorImpl *)x)
#define DB_IMP_OBJECTSTACKMANAGER(x) ((DbObjectStackManagerImpl *)x)


class DbObjectManagerImpl : public RxObjectImpl
{
public:
	DbObjectManagerImpl()
	{
		this->overallId = 0;
		this->handleManage = new DbHandleManage();
	}
	virtual ~DbObjectManagerImpl()
	{

	}

	unsigned int overallId;
	AcArray<DbObject*> objects;
	DbHandleManage *handleManage;
};


class DbObjectImpl : public RxObjectImpl
{
public:
	DbObjectImpl()
	{
		this->isWrite = false;
		this->handle = 0;
		this->mode = Db::OpenMode::kForNotify;
		this->isErase = false;
		this->database = NULL;

		this->extensionDictionaryId = NULL;
		this->mergeStyle = Db::DuplicateRecordCloning::kDrcIgnore;
	}
	virtual ~DbObjectImpl()
	{

	}

	bool isWrite;

	Db::OpenMode mode;
	DbHandle handle;
	DbObjectId objectId;
	DbObjectId ownerId;
	DbDatabase* database;
	bool isErase;
	AcArray<resbuf *> xDatas;
	Db::DuplicateRecordCloning mergeStyle;
	DbObjectId extensionDictionaryId;


	virtual bool isEntity() { return false; }
	virtual bool isBlockReference() { return false; }
	virtual bool isPoint() { return false; }
	virtual bool isCurve() { return false; }
	virtual bool isLine() { return false; }
	virtual bool isXline() { return false; }
	virtual bool isRay() { return false; }
	virtual bool isCircle() { return false; }
	virtual bool isArc() { return false; }
	virtual bool isPolyline() { return false; }
	virtual bool isViewport() { return false; }
	virtual bool isDimension() { return false; }
	virtual bool isDragEntity() { return false; }

	virtual bool isSymbolTable() { return false; }
	virtual bool isLayerTable() { return false; }
	virtual bool isBlockTable() { return false; }
	virtual bool isDimStyleTable() { return false; }
	virtual bool isLinetypeTable() { return false; }
	virtual bool isRegAppTable() { return false; }
	virtual bool isTextStyleTable() { return false; }
	virtual bool isUCSTable() { return false; }
	virtual bool isViewportTable() { return false; }
	virtual bool isViewTable() { return false; }

	virtual bool isSymbolTableRecord() { return false; }
	virtual bool isLayerTableRecord() { return false; }
	virtual bool isBlockTableRecord() { return false; }
	virtual bool isDimStyleTableRecord() { return false; }
	virtual bool isLinetypeTableRecord() { return false; }
	virtual bool isRegAppTableRecord() { return false; }
	virtual bool isTextStyleTableRecord() { return false; }
	virtual bool isUCSTableRecord() { return false; }
	virtual bool isAbstractViewTableRecordImpl() { return false; }
	virtual bool isViewportTableRecord() { return false; }
	virtual bool isViewTableRecord() { return false; }
	virtual bool isDictionary() { return false; }

	virtual bool isPlotSettings() { return false; }
	virtual bool isLayout() { return false; }
	virtual bool isLayoutManager() { return false; }
	virtual bool isLayoutManagerReactor() { return false; }

	virtual bool isXrecord() { return false; }
};


class DbDwgFilerImpl : public RxObjectImpl
{
public:
	DbDwgFilerImpl()
	{
		buff = NULL;
	}
	virtual ~DbDwgFilerImpl()
	{
	}

	IAcBinary* buff;
};
class DbDwgFileFilerImpl : public DbDwgFilerImpl
{
public:
	DbDwgFileFilerImpl()
	{
		this->buff = new AcBinary();
	}
	virtual ~DbDwgFileFilerImpl()
	{
		delete this->buff;
	}
};
class DbDwgUndoFilerImpl : public DbDwgFilerImpl
{
public:
	DbDwgUndoFilerImpl()
	{
		this->buff = new AcBinaryFiler();
	}
	virtual ~DbDwgUndoFilerImpl()
	{
		this->buff = NULL;
	}
};
class DbDwgCopyFilerImpl : public DbDwgFilerImpl
{
public:
	DbDwgCopyFilerImpl()
	{
		this->buff = new AcBinaryFiler();
	}
	virtual ~DbDwgCopyFilerImpl()
	{
		delete this->buff;
	}
};


class DbDatabaseReactorImpl : public RxObjectReactorImpl
{
public:
	DbDatabaseReactorImpl()
	{
	}
	virtual ~DbDatabaseReactorImpl()
	{
	}
};
class DbHostApplicationServicesImpl : public RxObjectImpl
{
public:
	DbHostApplicationServicesImpl()
	{
		this->workingDatabase = NULL;
	}
	virtual ~DbHostApplicationServicesImpl()
	{
	}

	DbDatabase* workingDatabase;
};


class DbEntityImpl : public DbObjectImpl
{
public:
	DbEntityImpl()
	{
		this->mark = false;
		this->markDrag = false;
		this->highlight = false;
		this->markSelectionSet = false;
		this->linetypeScale = 1;
		this->visibility = Db::Visibility::kVisible;
		this->lineWeight = Db::LineWeight::kLnWtByLayer;
		this->color.setColorMethod(CmEntityColor::ColorMethod::kByLayer);
		this->PlotStyleName = Db::PlotStyleNameType::kPlotStyleNameIsDictDefault;

		this->stake = NULL;
	}
	virtual ~DbEntityImpl()
	{

	}

	DbObjectId blockId;
	DbObjectId layerId;
	DbObjectId linetypeId;
	CmColor color;
	CmTransparency Transparency;
	double linetypeScale;
	Db::Visibility visibility;
	Db::LineWeight lineWeight;
	bool highlight;
	bool mark;
	bool markSelectionSet;
	bool markDrag;
	Db::PlotStyleNameType PlotStyleName;

	DbEntityStake* stake;
	AcArray<GiEntityGeometry*> entitys;
	AcArray<GiEntityGeometry*> highlightEntitys;

	virtual bool isEntity() { return true; }
};
class DbViewportImpl : public DbEntityImpl
{
public:
	DbViewportImpl()
	{
		this->makeChange = false;
		this->bOn = true;
		this->height = 6.4;
		this->width = 8.4;
		this->centerPoint = GePoint3d(5.25, 4.0, 0.0);
		this->number = 0;
		this->target = GePoint3d(0.0, 0.0, 0.0);
		this->viewDirection = GeVector3d(0.0, 0.0, 1.0);
		this->viewHeight = 9.226;
		this->viewCenter = GePoint2d(6.0, 4.5);
		this->twistAngle = 0.0;
		this->lensLength = 50.0;
		this->isFrontClipOn = false;
		this->isBackClipOn = false;
		this->isFrontClipAtEyeOn = true;
		this->frontClipDistance = 0.0;
		this->backClipDistance = 0.0;
		this->isPerspectiveOn = false;
		this->isUcsFollowModeOn = false;
		this->isUcsIconVisible = true;
		this->isUcsIconAtOrigin = true;
		this->circleSides = 100;
		this->isSnapOn = false;
		this->isSnapIsometric = false;
		this->isLocked = false;
		this->gridMajor = 0;
		this->gridIncrement = GeVector2d(0, 0);
		this->isGridFollow = true;
		this->isGridSubdivisionRestricted = true;
		this->isGridAdaptive = true;
		this->isGridBoundToLimits = true;
		this->isGridOn = false;
		this->snapIsoPair = 0;
		this->snapIncrement = GeVector2d(1, 1);
		this->snapBasePoint = GePoint2d(0, 0);
		this->snapAngle = 0.0;
		this->origin = GePoint3d::kOrigin;
		this->xAxis = GeVector3d::kXAxis;
		this->yAxis = GeVector3d::kYAxis;
		this->elevation = 0.0;
		this->ucsId = NULL;
		this->orthographicView = 1;
		this->isUcsSavedWithViewport = true;
	}
	virtual ~DbViewportImpl()
	{
	}

	bool makeChange;

	double height;
	double width;
	GePoint3d centerPoint;
	short number;
	bool bOn;
	GePoint3d target;
	GeVector3d viewDirection;
	double viewHeight;
	GePoint2d viewCenter;
	double twistAngle;
	double lensLength;
	bool isFrontClipOn;
	bool isBackClipOn;
	bool isFrontClipAtEyeOn;
	double frontClipDistance;
	double backClipDistance;
	bool isPerspectiveOn;
	bool isUcsFollowModeOn;
	bool isUcsIconVisible;
	bool isUcsIconAtOrigin;
	unsigned short circleSides;
	bool isSnapOn;
	bool isSnapIsometric;
	bool isLocked;
	DbObjectIdArray frozenLayers;
	unsigned short gridMajor;
	GeVector2d gridIncrement;
	bool isGridFollow;
	bool isGridSubdivisionRestricted;
	bool isGridAdaptive;
	bool isGridBoundToLimits;
	bool isGridOn;
	unsigned short snapIsoPair;
	GeVector2d snapIncrement;
	GePoint2d snapBasePoint;
	double snapAngle;
	GePoint3d origin;
	GeVector3d xAxis;
	GeVector3d yAxis;
	double elevation;
	DbObjectId ucsId;
	int orthographicView;
	bool isUcsSavedWithViewport;

	GeMatrix3d displayMatrix;

	virtual bool isViewport() { return true; }
};
class DbBlockReferenceImpl : public DbEntityImpl
{
public:
	DbBlockReferenceImpl()
	{
		this->position = GePoint3d::kOrigin;
		this->normal = GeVector3d::kZAxis;
		this->scale = GeScale3d::kIdentity;
		this->rotation = 0.0;
	}
	virtual ~DbBlockReferenceImpl()
	{

	}

	GePoint3d position;
	GeVector3d normal;
	GeScale3d scale;
	double rotation;
	DbObjectId blockTableRecordId;

	virtual bool isBlockReference() { return true; }
};
class DbPointImpl : public DbEntityImpl
{
public:
	DbPointImpl()
	{
		this->thickness = 0.0;
		this->normal = GeVector3d::kZAxis;
		this->ecsRotation = 0.0;
	}
	virtual ~DbPointImpl() {}

	GePoint3d position;
	double thickness;
	GeVector3d normal;
	double ecsRotation;

	virtual bool isPoint() { return true; }
};
class DbCurveImpl : public DbEntityImpl
{
public:
	DbCurveImpl() {}
	virtual ~DbCurveImpl() {}

	virtual bool isCurve() { return true; }
};
class DbLineImpl : public DbCurveImpl
{
public:
	DbLineImpl()
	{
		this->thickness = 0;
	}
	virtual ~DbLineImpl() {}

	GePoint3d startPoint;
	GePoint3d endPoint;
	double thickness;

	virtual bool isLine() { return true; }
};
class DbXlineImpl : public DbCurveImpl
{
public:
	DbXlineImpl()
	{
		this->direction = GeVector3d::kXAxis;
	}
	virtual ~DbXlineImpl()
	{
	}

	GeVector3d direction;
	GePoint3d basePoint;

	virtual bool isXline() { return true; }
};
class DbRayImpl : public DbCurveImpl
{
public:
	DbRayImpl()
	{
		this->direction = GeVector3d::kXAxis;
	}
	virtual ~DbRayImpl()
	{
	}

	GeVector3d direction;
	GePoint3d basePoint;

	virtual bool isRay() { return true; }
};
class DbCircleImpl : public DbCurveImpl
{
public:
	DbCircleImpl()
	{
		this->bInitPoint = false;
		this->radius = 0.0;
		this->thickness = 0.0;
		this->normal = GeVector3d::kZAxis;
	}
	virtual ~DbCircleImpl() {}

	GePoint3d center;
	double radius;
	double thickness;
	GeVector3d normal;

	GePoint3d initPoint;
	bool bInitPoint;

	virtual bool isCircle() { return true; }
};
class DbArcImpl : public DbCurveImpl
{
public:
	DbArcImpl()
	{
		this->bClockWise = false;
		this->bInitPoint = false;
		this->radius = 0.0;
		this->thickness = 0.0;
		this->startAngle = 0.0;
		this->endAngle = 0.0;
	}
	virtual ~DbArcImpl() {}

	GePoint3d center;
	double radius;
	double thickness;
	GeVector3d normal;
	double startAngle;
	double endAngle;

	GePoint3d initVertex1;
	GePoint3d initVertex2;
	GePoint3d initVertex3;
	bool bInitPoint;
	bool bClockWise;

	virtual bool isArc() { return true; }
};
class DbEllipseImpl : public DbCurveImpl
{
public:
	DbEllipseImpl()
	{
		this->center = GePoint3d::kOrigin;
		this->normal = GeVector3d::kZAxis;
		this->majorAxis = GeVector3d::kXAxis;
		this->minorAxis = GeVector3d::kYAxis;
		this->radiusRatio = 1.0;
		this->startAngle = 0.0;
		this->endAngle = PI * 2;
	}
	virtual ~DbEllipseImpl()
	{
	}

	GePoint3d center;
	GeVector3d normal;
	GeVector3d majorAxis;
	GeVector3d minorAxis;
	double radiusRatio;
	double startAngle;
	double endAngle;
};
class DbPolylineSegImpl
{
public:
	DbPolylineSegImpl()
	{
		this->bulge = 0.0;
		this->startWidth = 0.0;
		this->endWidth = 0.0;
		this->vertexIdentifier = 0;
	}
	virtual ~DbPolylineSegImpl()
	{
	}

	double bulge;
	double startWidth;
	double endWidth;
	int vertexIdentifier;
};
class DbPolylineImpl : public DbCurveImpl
{
public:
	DbPolylineImpl()
	{
		this->isClosed = false;
		this->thickness = 0.0;
		this->elevation = 0.0;
		this->normal = GeVector3d::kZAxis;

		this->initPoint = GePoint3d::kOrigin;
		this->isInitPoint = false;
	}
	virtual ~DbPolylineImpl()
	{
		for (int i = this->segments.length() - 1; i >= 0; i--)
		{
			DbPolylineSegImpl* temp = this->segments.at(i);
			delete temp;
		}
		this->segments.removeAll();
	}

public:
	double thickness;
	double elevation;
	bool isClosed;
	GeVector3d normal;
	AcArray<GePoint2d> vertexs;
	AcArray<DbPolylineSegImpl*> segments;

	GePoint3d initPoint;
	bool isInitPoint;

	virtual bool isPolyline() { return true; }
	unsigned int numSegments()
	{
		if (this->isClosed == true)
		{
			return this->vertexs.length();
		}
		return this->vertexs.length() - 1;
	}
};
class DbSolidImpl : public DbEntityImpl
{
public:
	DbSolidImpl()
	{
		this->countPoint = 3;
		this->thickness = 0.0;
		this->normal = GeVector3d::kZAxis;
		this->elevation = 0.0;
	}
	virtual ~DbSolidImpl()
	{
	}

	GePoint3d p0;
	GePoint3d p1;
	GePoint3d p2;
	GePoint3d p3;
	char countPoint;
	double thickness;
	GeVector3d normal;
	double elevation;
};
class DbTextImpl : public DbEntityImpl
{
public:
	DbTextImpl()
	{
		this->normal = GeVector3d::kZAxis;
		this->thickness = 0;
		this->oblique = 0;
		this->rotation = 0;
		this->height = 2.5;
		this->widthFactor = 1;
		this->textString = "";
		this->textStyleId = NULL;
		this->mirroredInX = false;
		this->mirroredInY = false;
		this->textAlignment = DbText::AcTextAlignment::kTextAlignmentLeft;
	}
	virtual ~DbTextImpl()
	{
	}

	GePoint3d position;
	GePoint3d alignmentPoint;
	GeVector3d normal;
	double thickness;
	double oblique;
	double rotation;
	double height;
	double widthFactor;
	AcString textString;
	DbObjectId textStyleId;
	bool mirroredInX;
	bool mirroredInY;
	DbText::AcTextAlignment textAlignment;
};
class DbMTextImpl : public DbEntityImpl
{
public:
	DbMTextImpl()
	{
		this->location = GePoint3d::kOrigin;
		this->normal = GeVector3d::kZAxis;
		this->direction = GeVector3d::kXAxis;
		this->rotation = 0;
		this->textHeight = 0;
		this->contents = "";
		this->textStyleId = NULL;
		this->attachment = DbMText::AttachmentPoint::kTopLeft;
		this->flowDirection = DbMText::FlowDirection::kByStyle;
		this->actualWidth = 0;
		this->actualHeight = 0;
		this->lineSpacingStyle = Db::LineSpacingStyle::kAtLeast;
		this->lineSpacingFactor = 1;
		this->backgroundFill = false;
		this->backgroundFillColor = CmColor();
		this->backgroundScaleFactor = 1.5;
		this->useBackgroundColor = false;
		this->columnType = DbMText::ColumnType::kNoColumns;
		this->columnAutoHeight = false;
		this->columnFlowReversed = false;
		this->columnWidth = 0;
		this->columnGutterWidth = 0;
		this->showBorders = false;
	}
	virtual ~DbMTextImpl()
	{
	}

	GePoint3d location;
	GeVector3d normal;
	GeVector3d direction;
	double rotation;
	double textHeight;
	AcString contents;
	DbObjectId textStyleId;
	DbMText::AttachmentPoint attachment;
	DbMText::FlowDirection flowDirection;
	double actualWidth;
	double actualHeight;
	Db::LineSpacingStyle lineSpacingStyle;
	double lineSpacingFactor;
	bool backgroundFill;
	CmColor backgroundFillColor;
	double backgroundScaleFactor;
	CmTransparency backgroundTransparency;
	bool useBackgroundColor;
	DbMText::ColumnType columnType;
	bool columnAutoHeight;
	bool columnFlowReversed;
	AcArray<double> columnHeights;
	double columnWidth;
	double columnGutterWidth;
	bool showBorders;
};


class DbXrecordImpl : public DbObjectImpl
{
public:
	DbXrecordImpl()
	{
		this->isXlateReferences = false;
	}
	virtual ~DbXrecordImpl()
	{
		for (int i = this->resbufs.length() - 1; i >= 0; i--)
		{
			resbuf* p = this->resbufs.at(i);
			acutRelRb(p);
		}
		this->resbufs.removeAll();
	}

	AcArray<resbuf*> resbufs;
	bool isXlateReferences;

	virtual bool isXrecord() { return true; }
};


class DbDimensionImpl : public DbEntityImpl
{
public:
	DbDimensionImpl()
	{
		this->normal = GeVector3d::kZAxis;
		this->elevation = 0.0;
		this->textRotation = 0.0;
		this->textAttachment = DbMText::AttachmentPoint::kMiddleCenter;
		this->textLineSpacingStyle = Db::LineSpacingStyle::kAtLeast;
		this->textLineSpacingFactor = 1.0;
		this->horizontalRotation = 0.0;
		this->isDynamicDimension = false;
		this->dimfit = 0;
		this->dimunit = 0;
		this->isHorizontalRefTextRotation = false;
		this->arrowFirstIsFlipped = false;
		this->arrowSecondIsFlipped = false;
		this->inspection = false;
		this->inspectionFrame = false;
		this->suppressAngularLeadingZeros = false;

		this->suppressAngularTrailingZeros = false;

		this->altSuppressZeroInches = false;
		this->altSuppressZeroFeet = false;
		this->altSuppressTrailingZeros = false;

		this->altToleranceSuppressLeadingZeros = false;
		this->altToleranceSuppressZeroInches = false;

		this->altToleranceSuppressZeroFeet = false;
		this->altToleranceSuppressTrailingZeros = false;

		this->suppressZeroFeet = false;
		this->suppressTrailingZeros = false;
		this->suppressLeadingZeros = false;
		this->suppressZeroInches = false;

		this->altSuppressLeadingZeros = false;
		this->toleranceSuppressZeroFeet = false;

		this->toleranceSuppressTrailingZeros = false;
		this->toleranceSuppressLeadingZeros = false;

		this->toleranceSuppressZeroInches = false;

		this->measurement = 0.0;

		this->dimadec = 0;
		this->dimalt = false;
		this->dimaltd = 3;
		this->dimaltf = 0.0394;
		this->dimaltrnd = 0;
		this->dimalttd = 3;
		this->dimalttz = 0;
		this->dimaltu = 2;
		this->dimaltz = 0;
		this->dimapost = "";
		this->dimasz = 2.5;
		this->dimatfit = 3;
		this->dimaunit = 0;
		this->dimazin = 0;
		this->dimblk = NULL;
		this->dimblk1 = NULL;
		this->dimblk2 = NULL;
		this->dimcen = 2.5;
		CmColor color;
		color.setColorIndex(0);
		this->dimclrd = color;
		this->dimclre = color;
		this->dimclrt = color;
		this->dimdec = 2;
		this->dimdle = 0;
		this->dimdli = 3.75;
		this->dimdsep = ',';
		this->dimexe = 1.25;
		this->dimexo = 0.625;
		this->dimfrac = 0;
		this->dimgap = 0.625;
		this->dimjust = 0;
		this->dimldrblk = NULL;
		this->dimlfac = 1;
		this->dimlim = false;
		this->dimlunit = 2;
		this->dimlwd = Db::LineWeight::kLnWtByBlock;
		this->dimlwe = Db::LineWeight::kLnWtByBlock;
		this->dimpost = "";
		this->dimrnd = 0;
		this->dimsah = false;
		this->dimscale = 1;
		this->dimsd1 = false;
		this->dimsd2 = false;
		this->dimse1 = false;
		this->dimse2 = false;
		this->dimsoxd = false;
		this->dimtad = 1;
		this->dimtdec = 2;
		this->dimtfac = 1;
		this->dimtih = false;
		this->dimtix = false;
		this->dimtm = 0;
		this->dimtmove = 0;
		this->dimtofl = true;
		this->dimtoh = false;
		this->dimtol = false;
		this->dimtolj = 0;
		this->dimtp = 0;
		this->dimtsz = 0;
		this->dimtvp = 0;
		this->dimtxsty = NULL;
		this->dimtxt = 2.5;
		this->dimtzin = 8;
		this->dimupt = false;
		this->dimzin = 8;
	}
	virtual ~DbDimensionImpl()
	{
	}

	GePoint3d textPosition;
	GeVector3d normal;
	double elevation;
	double textRotation;
	AcString dimensionText;
	DbObjectId dimensionStyle;
	DbMText::AttachmentPoint textAttachment;
	Db::LineSpacingStyle textLineSpacingStyle;
	double textLineSpacingFactor;
	double horizontalRotation;
	DbObjectId dimBlockId;
	bool isDynamicDimension;
	int dimfit;
	int dimunit;
	bool isHorizontalRefTextRotation;
	bool arrowFirstIsFlipped;
	bool arrowSecondIsFlipped;
	bool inspection;
	bool inspectionFrame;
	AcString inspectionLabel;
	AcString inspectionRate;
	AcString prefix;
	AcString suffix;
	AcString alternateSuffix;
	AcString alternatePrefix;
	bool suppressAngularLeadingZeros;

	bool suppressAngularTrailingZeros;

	bool altSuppressZeroInches;
	bool altSuppressZeroFeet;
	bool altSuppressTrailingZeros;

	bool altToleranceSuppressLeadingZeros;
	bool altToleranceSuppressZeroInches;

	bool altToleranceSuppressZeroFeet;
	bool altToleranceSuppressTrailingZeros;

	bool suppressZeroFeet;
	bool suppressTrailingZeros;
	bool suppressLeadingZeros;
	bool suppressZeroInches;

	bool altSuppressLeadingZeros;
	bool toleranceSuppressZeroFeet;

	bool toleranceSuppressTrailingZeros;
	bool toleranceSuppressLeadingZeros;

	bool toleranceSuppressZeroInches;

	double measurement;

	bool dimadec;
	bool dimalt;
	int dimaltd;
	double dimaltf;
	double dimaltrnd;
	int dimalttd;
	int dimalttz;
	int dimaltu;
	int dimaltz;
	AcString dimapost;
	double dimasz;
	int dimatfit;
	int dimaunit;
	int dimazin;
	DbObjectId dimblk;
	DbObjectId dimblk1;
	DbObjectId dimblk2;
	double dimcen;
	CmColor dimclrd;
	CmColor dimclre;
	CmColor dimclrt;
	int dimdec;
	double dimdle;
	double dimdli;
	wchar_t dimdsep;
	double dimexe;
	double dimexo;
	int dimfrac;
	double dimgap;
	int dimjust;
	DbObjectId dimldrblk;
	double dimlfac;
	bool dimlim;
	int dimlunit;
	Db::LineWeight dimlwd;
	Db::LineWeight dimlwe;
	AcString dimpost;
	double dimrnd;
	bool dimsah;
	double dimscale;
	bool dimsd1;
	bool dimsd2;
	bool dimse1;
	bool dimse2;
	bool dimsoxd;
	int dimtad;
	int dimtdec;
	double dimtfac;
	bool dimtih;
	bool dimtix;
	double dimtm;
	int dimtmove;
	bool dimtofl;
	bool dimtoh;
	bool dimtol;
	int dimtolj;
	double dimtp;
	double dimtsz;
	double dimtvp;
	DbObjectId dimtxsty;
	double dimtxt;
	int dimtzin;
	bool dimupt;
	int dimzin;

	virtual bool isDimension() { return true; }
};
class DbRotatedDimensionImpl : public DbDimensionImpl
{
public:
	DbRotatedDimensionImpl()
	{
		this->oblique = 0.0;
		this->rotation = 0.0;
		this->jogSymbolOn = false;
	}
	virtual ~DbRotatedDimensionImpl()
	{
	}

	GePoint3d xLine1Point;
	GePoint3d xLine2Point;
	GePoint3d dimLinePoint;
	double oblique;
	double rotation;
	bool jogSymbolOn;
	GePoint3d jogSymbolPosition;
};
class DbAlignedDimensionImpl : public DbDimensionImpl
{
public:
	DbAlignedDimensionImpl()
	{
		this->oblique = 0.0;
		this->jogSymbolOn = false;
	}
	virtual ~DbAlignedDimensionImpl()
	{
	}

	GePoint3d xLine1Point;
	GePoint3d xLine2Point;
	GePoint3d dimLinePoint;
	double oblique;
	bool jogSymbolOn;
	GePoint3d jogSymbolPosition;
};
class DbDiametricDimensionImpl : public DbDimensionImpl
{
public:
	DbDiametricDimensionImpl()
	{
		this->leaderLength = 0.0;
		this->extArcStartAngle = 0.0;
		this->extArcEndAngle = 0.0;
	}
	virtual ~DbDiametricDimensionImpl()
	{
	}

	double leaderLength;
	GePoint3d chordPoint;
	GePoint3d farChordPoint;
	double extArcStartAngle;
	double extArcEndAngle;
};
class DbRadialDimensionImpl : public DbDimensionImpl
{
public:
	DbRadialDimensionImpl()
	{
		this->leaderLength = 0.0;
		this->extArcStartAngle = 0.0;
		this->extArcEndAngle = 0.0;
	}
	virtual ~DbRadialDimensionImpl()
	{
	}

	double leaderLength;
	GePoint3d center;
	GePoint3d chordPoint;
	double extArcStartAngle;
	double extArcEndAngle;
};
class DbJigRealDimensionImpl : public DbDimensionImpl
{
public:
	DbJigRealDimensionImpl()
	{
		this->value = 0.0;
	}
	virtual ~DbJigRealDimensionImpl()
	{
	}

	double value;
};
class DbJigStringDimensionImpl : public DbDimensionImpl
{
public:
	DbJigStringDimensionImpl()
	{
	}
	virtual ~DbJigStringDimensionImpl()
	{
	}

	AcString value;
};
class Db3PointAngularDimensionImpl : public DbDimensionImpl
{
public:
	Db3PointAngularDimensionImpl()
	{
		this->extArcOn = false;
	}
	virtual ~Db3PointAngularDimensionImpl()
	{
	}

	GePoint3d arcPoint;
	GePoint3d xLine1Point;
	GePoint3d xLine2Point;
	GePoint3d centerPoint;
	bool extArcOn;
};
class DbOrdinateDimensionImpl : public DbDimensionImpl
{
public:
	DbOrdinateDimensionImpl()
	{
		this->isUsingXAxis = true;
		this->isUsingYAxis = true;
	}
	virtual ~DbOrdinateDimensionImpl()
	{
	}

	GePoint3d origin;
	GePoint3d definingPoint;
	GePoint3d leaderEndPoint;
	bool isUsingXAxis;
	bool isUsingYAxis;
};
class DbArcDimensionImpl : public DbDimensionImpl
{
public:
	DbArcDimensionImpl()
	{
		this->isPartial = false;
		this->arcStartParam = 0.0;
		this->arcEndParam = 0.0;
		this->hasLeader = false;
		this->arcSymbolType = 0;
	}
	virtual ~DbArcDimensionImpl()
	{
	}

	GePoint3d arcPoint;
	GePoint3d xLine1Point;
	GePoint3d xLine2Point;
	GePoint3d centerPoint;
	bool isPartial;
	double arcStartParam;
	double arcEndParam;
	bool hasLeader;
	GePoint3d leader1Point;
	GePoint3d leader2Point;
	int arcSymbolType;
};
class DbRadialDimensionLargeImpl : public DbDimensionImpl
{
public:
	DbRadialDimensionLargeImpl()
	{
		this->jogAngle = 0.0;
		this->extArcStartAngle = 0.0;
		this->extArcEndAngle = 0.0;
	}
	virtual ~DbRadialDimensionLargeImpl()
	{
	}

	GePoint3d center;
	GePoint3d chordPoint;
	GePoint3d overrideCenter;
	GePoint3d jogPoint;
	double jogAngle;
	GePoint3d jogPointPP;
	GePoint3d textPositionPP;
	double extArcStartAngle;
	double extArcEndAngle;
};
class Db2LineAngularDimensionImpl : public DbDimensionImpl
{
public:
	Db2LineAngularDimensionImpl()
	{
		this->extArcOn = true;
	}
	virtual ~Db2LineAngularDimensionImpl()
	{
	}

	GePoint3d arcPoint;
	GePoint3d xLine1Start;
	GePoint3d xLine1End;
	GePoint3d xLine2Start;
	GePoint3d xLine2End;
	bool extArcOn;
};



class DbSymbolTableImpl : public DbObjectImpl
{
public:
	DbSymbolTableImpl()
	{
		this->indexAnonymousBlock = 0;
		this->pMap = new AcMap<AcString, DbSymbolTableRecord*>();
	}
	virtual ~DbSymbolTableImpl()
	{
	}

	int indexAnonymousBlock;
	AcMap<AcString, DbSymbolTableRecord*>* pMap;

	virtual bool isSymbolTable() { return true; }
};
class DbLayerTableImpl : public DbSymbolTableImpl
{
public:
	DbLayerTableImpl() {}
	virtual ~DbLayerTableImpl() {}

	virtual bool isLayerTable() { return true; }
};
class DbRegAppTableImpl : public DbSymbolTableImpl
{
public:
	DbRegAppTableImpl() {}
	virtual ~DbRegAppTableImpl() {}

	virtual bool isRegAppTable() { return true; }
};
class DbTextStyleTableImpl : public DbSymbolTableImpl
{
public:
	DbTextStyleTableImpl() {}
	virtual ~DbTextStyleTableImpl() {}

	virtual bool isTextStyleTable() { return true; }
};
class DbLinetypeTableImpl : public DbSymbolTableImpl
{
public:
	DbLinetypeTableImpl() {}
	virtual ~DbLinetypeTableImpl() {}

	virtual bool isLinetypeTable() { return true; }
};
class DbUCSTableImpl : public DbSymbolTableImpl
{
public:
	DbUCSTableImpl() {}
	virtual ~DbUCSTableImpl() {}

	virtual bool isUCSTable() { return true; }
};
class DbViewportTableImpl : public DbSymbolTableImpl
{
public:
	DbViewportTableImpl() {}
	virtual ~DbViewportTableImpl() {}

	virtual bool isViewportTable() { return true; }
};
class DbViewTableImpl : public DbSymbolTableImpl
{
public:
	DbViewTableImpl() {}
	virtual ~DbViewTableImpl() {}

	virtual bool isViewTable() { return true; }
};
class DbBlockTableImpl : public DbSymbolTableImpl
{
public:
	DbBlockTableImpl() {}
	virtual ~DbBlockTableImpl() {}

	virtual bool isBlockTable() { return true; }
};
class DbDimStyleTableImpl : public DbSymbolTableImpl
{
public:
	DbDimStyleTableImpl() {}
	virtual ~DbDimStyleTableImpl() {}

	virtual bool isDimStyleTable() { return true; }
};


class DbSymbolTableRecordImpl : public DbObjectImpl
{
public:
	DbSymbolTableRecordImpl() {}
	virtual ~DbSymbolTableRecordImpl() {}

	AcString name;

	virtual bool isSymbolTableRecord() { return true; }
};
class DbLayerTableRecordImpl : public DbSymbolTableRecordImpl
{
public:
	DbLayerTableRecordImpl()
	{
		this->isFrozen = false;
		this->isOff = false;
		this->VPDFLT = false;
		this->isLocked = false;
		this->isPlottable = true;
		this->lineWeight = Db::LineWeight::kLnWtByLwDefault;
		this->isHidden = false;
		this->description = "";
		this->color.setColorIndex(7);
		this->transparency.setAlpha(255);
	}
	virtual ~DbLayerTableRecordImpl() {}

	bool isFrozen;
	bool isOff;
	bool VPDFLT;
	bool isLocked;
	CmColor color;
	CmTransparency transparency;
	DbObjectId linetypeObjectId;
	bool isPlottable;
	Db::LineWeight lineWeight;
	bool isHidden;
	AcString description;

	virtual bool isLayerTableRecord() { return true; }
};
class DbRegAppTableRecordImpl : public DbSymbolTableRecordImpl
{
public:
	DbRegAppTableRecordImpl()
	{
	}
	virtual ~DbRegAppTableRecordImpl() {}

	virtual bool isRegAppTableRecord() { return true; }
};
class DbTextStyleTableRecordImpl : public DbSymbolTableRecordImpl
{
public:
	DbTextStyleTableRecordImpl()
	{
		this->isShapeFile = false;
		this->vertical = false;
		this->textSize = 2.5;
		this->widthFactor = 0;
		this->xScale = 1;
		this->obliquingAngle = 0;
		this->priorSize = 0;
		this->fileName = "";
		this->bigFontFileName = "";
		this->flagBits = 0;

		this->pTypeface = "";
		this->bold = false;
		this->italic = false;
		this->charset = Charset::kAnsiCharset;
		this->pitch = FontUtils::FontPitch::kDefault;
		this->family = FontUtils::FontFamily::kDecorative;
		this->bAllowMissingFont = false;
	}
	virtual ~DbTextStyleTableRecordImpl() {}

	bool isShapeFile;
	bool vertical;
	double textSize;
	double widthFactor;
	double xScale;
	double obliquingAngle;
	Adesk::UInt8 flagBits;
	double priorSize;
	AcString fileName;
	AcString bigFontFileName;

	AcString pTypeface;
	Adesk::Boolean bold;
	Adesk::Boolean italic;
	Charset charset;
	FontUtils::FontPitch pitch;
	FontUtils::FontFamily family;
	bool bAllowMissingFont;

	virtual bool isTextStyleTableRecord() { return true; }
};
class DbLinetypeDasheImpl : public RxObjectImpl
{
public:
	DbLinetypeDasheImpl()
	{
		this->text = "";
		this->dashLength = 0;
		this->shapeOffset = GeVector2d(0, 0);
		this->shapeScale = 1.0;
		this->shapeRotation = 0.0;
	}
	virtual ~DbLinetypeDasheImpl()
	{
	}

	AcString text;
	double dashLength;
	GeVector2d shapeOffset;
	double shapeScale;
	double shapeRotation;
};
class DbLinetypeTableRecordImpl : public DbSymbolTableRecordImpl
{
public:
	DbLinetypeTableRecordImpl()
	{
		this->patternLength = 0.0;
	}
	virtual ~DbLinetypeTableRecordImpl()
	{
		realloc_arry_all(this->linetypeDashe);
	}

	AcString comments;
	double patternLength;
	AcArray<DbLinetypeDashe*> linetypeDashe;

	virtual bool isLinetypeTableRecord() { return true; }
};
class DbUCSTableRecordImpl : public DbSymbolTableRecordImpl
{
public:
	DbUCSTableRecordImpl()
	{
		this->origin = GePoint3d::kOrigin;
		this->xAxis = GeVector3d::kXAxis;
		this->yAxis = GeVector3d::kYAxis;
	}
	virtual ~DbUCSTableRecordImpl() {}

	GePoint3d origin;
	GeVector3d xAxis;
	GeVector3d yAxis;

	virtual bool isUCSTableRecord() { return true; }
};
class DbAbstractViewTableRecordImpl : public DbSymbolTableRecordImpl
{
public:
	DbAbstractViewTableRecordImpl()
	{
		this->centerPoint = GePoint2d(21.3202, 11.2227);
		this->height = 25.4016;
		this->width = 44.8461;
		this->target = GePoint3d(0, 0, 0);
		this->twistAngle = 0.0;
		this->viewDirection = GeVector3d::kZAxis;
		this->lensLength = 50.0;
		this->frontClipDistance = 0.0;
		this->backClipDistance = 0.0;
		this->isFrontClipAtEye = true;
		this->isFrontClipEnabled = false;
		this->isBackClipEnabled = false;
		this->isPerspectiveEnabled = false;
		this->elevation = 0.0;
		this->origin = GePoint3d::kOrigin;
		this->xAxis = GeVector3d::kXAxis;
		this->yAxis = GeVector3d::kYAxis;
		this->ucsId = NULL;
		this->orthographicView = 1;
	}
	virtual ~DbAbstractViewTableRecordImpl()
	{
	}

	GePoint2d centerPoint;
	double height;
	double width;
	GePoint3d target;
	GeVector3d viewDirection;
	double twistAngle;
	double lensLength;
	double frontClipDistance;
	double backClipDistance;
	bool isFrontClipAtEye;
	DbObjectId background;
	DbObjectId ucsId;
	bool isFrontClipEnabled;
	bool isBackClipEnabled;
	bool isPerspectiveEnabled;
	double elevation;
	GePoint3d origin;
	GeVector3d xAxis;
	GeVector3d yAxis;
	int orthographicView;

	virtual bool isAbstractViewTableRecordImpl() { return true; }
};
class DbViewportTableRecordImpl : public DbAbstractViewTableRecordImpl
{
public:
	DbViewportTableRecordImpl()
	{
		this->makeChange = false;

		this->lowerLeftCorner = GePoint2d(0, 0);
		this->upperRightCorner = GePoint2d(1, 1);
		this->ucsFollowMode = false;
		this->ucsFollowMode = false;
		this->circleSides = 1000;
		this->iconEnabled = true;
		this->iconAtOrigin = true;
		this->gridEnabled = true;
		this->gridIncrements = GePoint2d(0.5, 0.5);
		this->snapEnabled = false;
		this->isometricSnapEnabled = false;
		this->snapPair = 0;
		this->snapAngle = 0.0;
		this->snapBase = GePoint2d(0, 0);
		this->snapIncrements = GePoint2d(0.5, 0.5);
		this->isUcsSavedWithViewport = true;
	}
	virtual ~DbViewportTableRecordImpl() {}

	bool makeChange;

	GePoint2d lowerLeftCorner;
	GePoint2d upperRightCorner;
	bool ucsFollowMode;
	unsigned short circleSides;
	bool iconEnabled;
	bool iconAtOrigin;
	bool gridEnabled;
	GePoint2d gridIncrements;
	bool snapEnabled;
	bool isometricSnapEnabled;
	unsigned short snapPair;
	double snapAngle;
	GePoint2d snapBase;
	GePoint2d snapIncrements;
	bool isUcsSavedWithViewport;

	GeMatrix3d displayMatrix;

	virtual bool isViewportTableRecord() { return true; }
};
class DbViewTableRecordImpl : public DbAbstractViewTableRecordImpl
{
public:
	DbViewTableRecordImpl()
	{
	}
	virtual ~DbViewTableRecordImpl() {}

	virtual bool isViewTableRecord() { return true; }
};
class DbDimStyleTableRecordImpl : public DbSymbolTableRecordImpl
{
public:
	DbDimStyleTableRecordImpl()
	{
		this->dimadec = 0;
		this->dimalt = false;
		this->dimaltd = 3;
		this->dimaltf = 0.0394;
		this->dimaltrnd = 0;
		this->dimalttd = 3;
		this->dimalttz = 0;
		this->dimaltu = 2;
		this->dimaltz = 0;
		this->dimapost = "";
		this->dimasz = 2.5;
		this->dimatfit = 3;
		this->dimaunit = 0;
		this->dimazin = 0;
		this->dimcen = 2.5;
		CmColor color;
		color.setColorIndex(0);
		this->dimclrd = color;
		this->dimclre = color;
		this->dimclrt = color;
		this->dimdec = 2;
		this->dimdle = 0;
		this->dimdli = 3.75;
		this->dimdsep = ',';
		this->dimexe = 1.25;
		this->dimexo = 0.625;
		this->dimfrac = 0;
		this->dimgap = 0.625;
		this->dimjust = 0;
		this->dimlfac = 1;
		this->dimlim = false;
		this->dimlunit = 2;
		this->dimlwd = Db::LineWeight::kLnWtByBlock;
		this->dimlwe = Db::LineWeight::kLnWtByBlock;
		this->dimpost = "";
		this->dimrnd = 0;
		this->dimsah = false;
		this->dimscale = 1;
		this->dimsd1 = false;
		this->dimsd2 = false;
		this->dimse1 = false;
		this->dimse2 = false;
		this->dimsoxd = false;
		this->dimtad = 1;
		this->dimtdec = 2;
		this->dimtfac = 1;
		this->dimtih = false;
		this->dimtix = false;
		this->dimtm = 0;
		this->dimtmove = 0;
		this->dimtofl = true;
		this->dimtoh = false;
		this->dimtol = false;
		this->dimtolj = 0;
		this->dimtp = 0;
		this->dimtsz = 0;
		this->dimtvp = 0;
		this->dimtxt = 2.5;
		this->dimtzin = 8;
		this->dimupt = false;
		this->dimzin = 8;
	}
	virtual ~DbDimStyleTableRecordImpl() {}

	bool dimadec;
	bool dimalt;
	int dimaltd;
	double dimaltf;
	double dimaltrnd;
	int dimalttd;
	int dimalttz;
	int dimaltu;
	int dimaltz;
	AcString dimapost;
	double dimasz;
	int dimatfit;
	int dimaunit;
	int dimazin;
	DbObjectId dimblk;
	DbObjectId dimblk1;
	DbObjectId dimblk2;
	double dimcen;
	CmColor dimclrd;
	CmColor dimclre;
	CmColor dimclrt;
	int dimdec;
	double dimdle;
	double dimdli;
	wchar_t dimdsep;
	double dimexe;
	double dimexo;
	int dimfrac;
	double dimgap;
	int dimjust;
	DbObjectId dimldrblk;
	double dimlfac;
	bool dimlim;
	int dimlunit;
	Db::LineWeight dimlwd;
	Db::LineWeight dimlwe;
	AcString dimpost;
	double dimrnd;
	bool dimsah;
	double dimscale;
	bool dimsd1;
	bool dimsd2;
	bool dimse1;
	bool dimse2;
	bool dimsoxd;
	int dimtad;
	int dimtdec;
	double dimtfac;
	bool dimtih;
	bool dimtix;
	double dimtm;
	int dimtmove;
	bool dimtofl;
	bool dimtoh;
	bool dimtol;
	int dimtolj;
	double dimtp;
	double dimtsz;
	double dimtvp;
	DbObjectId dimtxsty;
	double dimtxt;
	int dimtzin;
	bool dimupt;
	int dimzin;

	virtual bool isdimStyleTableRecord() { return true; }
};
class DbBlockTableRecordImpl : public DbSymbolTableRecordImpl
{
public:
	DbBlockTableRecordImpl()
	{
		this->isFromOverlayReference = false;
		this->unloaded = false;
		this->blockScaling = 0;
		this->explodable = true;
		this->blockInsertUnits = 0;
	}
	virtual ~DbBlockTableRecordImpl()
	{
	}

	AcString comments;
	AcString pathName;
	GePoint3d origin;
	bool isFromOverlayReference;
	DbObjectId layoutId;
	AcArray<DbEntity*> entitys;
	AcArray<DbEntity*> viewports;
	bool unloaded;
	int blockScaling;
	bool explodable;
	int blockInsertUnits;
	DbObjectIdArray blockReferenceIds;

	virtual bool isBlockTableRecord() { return true; }
};



class DbDictionaryImpl : public DbObjectImpl
{
public:
	DbDictionaryImpl()
	{

	}
	virtual ~DbDictionaryImpl()
	{

	}

	AcArray<DbDictionaryRecord*> records;

	virtual bool isDictionary() { return true; }
};


class DbPlotSettingsImpl : public DbObjectImpl
{
public:
	DbPlotSettingsImpl()
	{
	}
	virtual ~DbPlotSettingsImpl()
	{
	}

	virtual void clone(DbPlotSettingsImpl* pImpl)
	{
	}

	virtual bool isPlotSettings() { return true; }
};
class DbLayoutImpl : public DbPlotSettingsImpl
{
public:
	DbLayoutImpl()
	{
		this->order = 0;
		this->tabSelected = false;
		this->layoutName = "";
		this->blockTableRecordId = NULL;
		this->activationViewportId = NULL;
	}
	virtual ~DbLayoutImpl()
	{
	}

	int order;
	bool tabSelected;
	AcString layoutName;
	DbObjectId blockTableRecordId;
	DbObjectId activationViewportId;

	virtual void clone(DbLayoutImpl* pImpl)
	{
		DbPlotSettingsImpl::clone(pImpl);
		this->blockTableRecordId = NULL;
		this->layoutName = pImpl->layoutName;
		this->order = pImpl->order;
		this->tabSelected = false;
	}

	virtual bool isLayout() { return true; }
};
class DbLayoutManagerImpl : public RxObjectImpl
{
public:
	DbLayoutManagerImpl()
	{

	}
	virtual ~DbLayoutManagerImpl()
	{

	}

	AcArray<DbLayoutManagerReactor*> layoutManagerReactor;
};
class DbLayoutManagerReactorImpl : public RxObjectReactorImpl
{
public:
	DbLayoutManagerReactorImpl()
	{

	}
	virtual ~DbLayoutManagerReactorImpl()
	{
	}

	virtual bool isLayoutManagerReactor() { return true; }
};


class DbSymbolTableIteratorImpl : public RxObjectImpl
{
public:
	DbSymbolTableIteratorImpl()
	{
	}
	virtual ~DbSymbolTableIteratorImpl()
	{
	}

	AcIterator<RxObject*> iterator;
};
class DbBlockTableRecordIteratorImpl : public RxObjectImpl
{
public:
	DbBlockTableRecordIteratorImpl()
	{
		this->blockTableRecord = NULL;
	}
	virtual ~DbBlockTableRecordIteratorImpl()
	{
	}

	AcIterator<DbEntity*> iterator;
	DbBlockTableRecord* blockTableRecord;
};
class DbDictionaryIteratorImpl : public RxObjectImpl
{
public:
	DbDictionaryIteratorImpl()
	{
	}
	virtual ~DbDictionaryIteratorImpl()
	{
	}

	AcIterator<RxObject *> iterator;
};



#endif
