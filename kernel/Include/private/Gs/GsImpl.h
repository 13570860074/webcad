#ifndef GSIMPL_H
#define GSIMPL_H

#include "RxObjectImpl.h"
#include "AcArray.h"
#include "AcIterator.h"
#include "ApDocument.h"
#include "DbUCSTableRecord.h"
#include "GiEntity.h"
#include "GsElement.h"
#include "GsCursor.h"
#include "GsCoordSystem.h"
#include "GiLineSegment.h"
#include "GiMesh.h"

#define GS_IMP_OBJECT(x) ((GsObjectImpl *)x)

#define GS_IMP_ELEMENTMANAGER(x) ((GsElementManagerImpl *)x)

#define GS_IMP_ELEMENT(x) ((GsElementImpl *)x)
#define GS_IMP_CURSOR(x) ((GsCursorImpl *)x)

#define GS_IMP_COORDSYSTEM(x) ((GsCoordSystemImpl *)x)
#define GS_IMP_COORDSYSTEMMANAGER(x) ((GsCoordSystemManagerImpl *)x)

#define GS_IMP_GRIP(x) ((GsGripImpl *)x)
#define GS_IMP_GRIPMANAGER(x) ((GsGripManagerImpl *)x)

#define GS_IMP_OSNAP(x) ((GsOsnapImpl *)x)
#define GS_IMP_OSNAPMANAGER(x) ((GsOsnapManagerImpl *)x)

#define GS_IMP_TWISTBENCHMARK(x) ((GsTwistBenchmarkImpl *)x)
#define GS_IMP_TWISTBENCHMARKMANAGER(x) ((GsTwistBenchmarkManagerImpl *)x)

#define GS_IMP_CANVAS(x) ((GsCanvasImpl *)x)
#define GS_IMP_CANVASMANAGER(x) ((GsCanvasManagerImpl *)x)

#define GS_IMP_SELECTIONBORDER(x) ((GsSelectionBorderImpl*)x)
#define GS_IMP_SELECTIONBORDERMANAGER(x) ((GsSelectionBorderManagerImpl*)x)

#define GS_IMP_DYNDIM(x) ((GsDynDimImpl*)x)
#define GS_IMP_DYNDIMMANAGER(x) ((GsDynDimManagerImpl*)x)

#define GS_IMP_GRIDLINE(x) ((GsGridlineImpl*)x)
#define GS_IMP_GRIDLINEMANAGER(x) ((GsGridlineManagerImpl*)x)



class GsObjectImpl : public RxObjectImpl
{
public:
	GsObjectImpl()
	{
	}
	virtual ~GsObjectImpl()
	{
	}
};


class GsElementImpl : public GsObjectImpl
{
public:
	GsElementImpl()
	{
		this->document = NULL;
	}
	virtual ~GsElementImpl()
	{
	}

	ApDocument* document;
};
class GsCursorImpl : public GsElementImpl
{
public:
	GsCursorImpl()
	{
		this->bEnable = true;
		this->cursorEntity = NULL;
		this->leftSelectionSetMesh = NULL;
		this->rightSelectionSetMesh = NULL;
		this->style = (GsCursor::ECursorStyle)(GsCursor::ECursorStyle::kTarget | GsCursor::ECursorStyle::kCrossCursor);
	}
	virtual ~GsCursorImpl()
	{
	}

	bool bEnable;
	GsCursor::ECursorStyle style;
	GiLineSegment* cursorEntity;
	GiMesh* leftSelectionSetMesh;
	GiMesh* rightSelectionSetMesh;
};
class GsCoordSystemImpl : public GsElementImpl {
public:
	GsCoordSystemImpl()
	{
		this->coordSys = NULL;
		this->isDrag = false;
		this->isFixedLowerLeftCorner = true;
		this->ordinaryCoorSysEntity = NULL;
		this->layoutCoorSysEntity = NULL;
		this->style = GsCoordSystem::ECoordSystemStyle::kOrdinaryCoordSys;
	}
	virtual ~GsCoordSystemImpl()
	{
	}

	DbUCSTableRecord* coordSys;
	bool isDrag;
	bool isFixedLowerLeftCorner;
	GsCoordSystem::ECoordSystemStyle style;
	GiLineSegment *ordinaryCoorSysEntity;
	GiLineSegment* layoutCoorSysEntity;
};



class GsElementManagerImpl :public GsObjectImpl {
public:
	GsElementManagerImpl() {
		this->curCursor = NULL;
		this->coordSystem = NULL;
	}
	virtual ~GsElementManagerImpl() {
		realloc_arry_all(elements);
	}

	GsElement* curCursor;
	GsCoordSystem *coordSystem;
	AcArray<GsElement*> elements;
};




#endif