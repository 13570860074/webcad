#ifndef GIIMPL_H
#define GIIMPL_H

#include "RxImpl.h"
#include "AcMap.h"
#include "AcEvent.h"
#include "AcEmsdkEvent.h"
#include "AcIterator.h"
#include "ApDocument.h"
#include "CmEntityColor.h"
#include "GeRay3d.h"
#include "GeMatrix3d.h"
#include "GiFill.h"
#include "GiTextStyle.h"
#include "GiMaterial.h"
#include "GiWorldDraw.h"
#include "GiEntityIndex.h"
#include "GiEntityMutex.h"
#include "GiEntityTraits.h"
#include "GiMaterialReactor.h"
#include "GiEntityReactor.h"
#include "GiEntityGeometry.h"
#include "GiWorldGeometryStake.h"
#include "GiWorldDrawGeometry.h"
#include "GiTriangleMeshGeometry.h"
#include "GiGeometryExtent.h"
#include "GiGeometryExtentView.h"
#include "GiEntityGeometryStake.h"
#include "GiEntityHighlander.h"
#include "PAL/api/charset.h"
#include <atomic>

#define GI_IMP_OBJECT(x) ((GiObjectImpl *)x)

#define GI_IMP_MATERIAL(x) ((GiMaterialImpl *)x)
#define GI_IMP_POINTMATERIAL(x) ((GiPointMaterialImpl *)x)
#define GI_IMP_LINEMATERIAL(x) ((GiLineMaterialImpl *)x)
#define GI_IMP_LINEDASHEDMATERIAL(x) ((GiLineDashedMaterialImpl *)x)
#define GI_IMP_LINEPIXELDASHEDMATERIAL(x) ((GiLinePixelDashedMaterialImpl *)x)
#define GI_IMP_MESHMATERIAL(x) ((GiMeshMaterialImpl *)x)
#define GI_IMP_GIMATERIALREACTOR(x) ((GiMaterialReactorImpl *)x)
#define GI_IMP_MATERIALITERATOR(x) ((GiMaterialIteratorImpl *)x)
#define GI_IMP_MATERIALMANAGER(x) ((GiMaterialManagerImpl *)x)

#define GI_IMP_ENTITY(x) ((GiEntityImpl *)x)
#define GI_IMP_POINTS(x) ((GiPointsImpl *)x)
#define GI_IMP_LINESEGMENT(x) ((GiLineSegmentImpl *)x)
#define GI_IMP_LINEAR(x) ((GiLinearImpl *)x)
#define GI_IMP_RAY(x) ((GiRayImpl *)x)
#define GI_IMP_MESH(x) ((GiMeshImpl *)x)
#define GI_IMP_ENTITYGEOMETRYMANAGER(x) ((GiEntityGeometryManagerImpl *)x)

#define GI_IMP_GIENTITYREACTOR(x) ((GiEntityReactorImpl *)x)
#define GI_IMP_ENTITYITERATOR(x) ((GiEntityIteratorImpl *)x)
#define GI_IMP_ENTITYMANAGER(x) ((GiEntityManagerImpl *)x)

#define GI_IMP_FILL(x) ((GiImpFill *)x)

#define GI_IMP_VERTEXDATA(x) ((GiVertexDataImpl *)x)
#define GI_IMP_FACEDATA(x) ((GiFaceDataImpl *)x)
#define GI_IMP_EDGEDATA(x) ((GiEdgeDataImpl *)x)

#define GI_IMP_SUBENTITYTRAITS(x) ((GiSubEntityTraitsImpl *)x)
#define GI_IMP_ENTITYTRAITS(x) ((GiEntityTraitsImpl *)x)

#define GI_IMP_WORLDDRAW(x) ((GiWorldDrawImpl *)x)
#define GI_IMP_ENTDRAW(x) ((GiEntDrawImpl *)x)
#define GI_IMP_DRAGDRAW(x) ((GiDragDrawImpl *)x)
#define GI_IMP_WORLDDRAWMANAGER(x) ((GiWorldDrawManagerImpl *)x)
#define GI_IMP_ENTITYHIGHLANDER(x) ((GiEntityHighlanderImpl *)x)

#define GI_IMP_ENTITYGEOMETRY(x) ((GiEntityGeometryImpl *)x)
#define GI_IMP_POINTGEOMETRY(x) ((GiPointGeometryImpl *)x)
#define GI_IMP_LINESEGMENTGEOMETRY(x) ((GiLineSegmentGeometryImpl *)x)
#define GI_IMP_TRIANGLEMESHGEOMETRY(x) ((GiTriangleMeshGeometryImpl *)x)

#define GI_IMP_ENTITYGEOMETRYRECORD(x) ((GiEntityGeometryRecordImpl *)x)
#define GI_IMP_ENTITYGEOMETRYMANAGE(x) ((GiEntityGeometryManageImpl *)x)

#define GI_IMP_GEOMETRYSELECTORMANAGER(x) ((GiGeometryExtentManagerImpl *)x)

#define GI_IMP_VARIANT(x) ((GiVariantImpl *)x)

#define GI_IMP_GEOMETRYSELECTOR(x) ((GiGeometryExtentImpl *)x)
#define GI_IMP_GEOMETRYSELECTORMANAGER(x) ((GiGeometryExtentManagerImpl *)x)

#define GI_IMP_WORLDGEOMETRY(x) ((GiWorldGeometryImpl *)x)
#define GI_IMP_WORLDDRAWGEOMETRY(x) ((GiWorldDrawGeometryImpl *)x)
#define GI_IMP_DRAGDRAWGEOMETRY(x) ((GiDragDrawGeometryImpl *)x)

#define GI_IMP_TEXTSTYLE(x) ((GiTextStyleImpl *)x)
#define GI_IMP_FONTMANAGER(x) ((GiFontManagerImpl *)x)

class GiObjectImpl : public RxObjectImpl
{
public:
	GiObjectImpl()
	{
	}
	virtual ~GiObjectImpl()
	{
	}

	GiObjectId m_id;
};




/** 实体几何 */
/** -------------------------------------------------------------------- */


class GiEntityGeometryImpl : public GiObjectImpl
{
public:
	GiEntityGeometryImpl()
	{
		this->isMark = false;
		this->isErased = false;
		this->visibility = true;
		this->giEntity = NULL;
		this->stake = NULL;
	}
	virtual ~GiEntityGeometryImpl()
	{

	}

	bool isMark;
	bool isErased;
	bool visibility;
	GiEntity* giEntity;
	DbEntityStake* stake;
};
class GiPointGeometryImpl : public GiEntityGeometryImpl
{
public:
	GiPointGeometryImpl()
	{
	}
	virtual ~GiPointGeometryImpl()
	{
	}

	GePoint3d position;

	GePoint2d positionDisplayCoord;
};
class GiLineSegmentGeometryImpl : public GiEntityGeometryImpl
{
public:
	GiLineSegmentGeometryImpl()
	{

	}
	virtual ~GiLineSegmentGeometryImpl()
	{
	}

	GePoint3d startPoint;
	GePoint3d endPoint;

	GePoint2d startPointDisplayCoord;
	GePoint2d endPointDisplayCoord;
};
class GiTriangleMeshGeometryImpl : public GiEntityGeometryImpl
{
public:
	GiTriangleMeshGeometryImpl()
	{
	}
	virtual ~GiTriangleMeshGeometryImpl()
	{
	}

	GePoint3d vertex1;
	GePoint3d vertex2;
	GePoint3d vertex3;

	GePoint2d displayCoordVertex1;
	GePoint2d displayCoordVertex2;
	GePoint2d displayCoordVertex3;
};

/** -------------------------------------------------------------------- */



/** 实体 */
/** -------------------------------------------------------------------- */

class GiEntityImpl : public GiObjectImpl
{
public:
	GiEntityImpl()
	{
		this->bCreate = false;
		this->bUpdateVertex = false;
		this->bTransformation = false;
		this->vertexs = NULL;
		this->sizeVertexData = 0;
		this->vertexLength = 0;
		this->element = Gi::ElementType::kDragging;
		this->visibility = true;
		this->material = NULL;
	}
	virtual ~GiEntityImpl()
	{
		if (this->vertexs != NULL)
		{
			delete[] this->vertexs;
		}
	}

	bool bCreate;
	bool bUpdateVertex;
	bool bTransformation;
	Gi::ElementType element;
	bool visibility;
	double* vertexs;
	unsigned int sizeVertexData;
	unsigned int vertexLength;
	DbObjectId layerId;
	GeMatrix3d matrix;
	GiMaterial* material;
	AcArray<GiEntityGeometry*> geometrys;
};
class GiPointsImpl : public GiEntityImpl
{
public:
	GiPointsImpl()
	{
	}
	virtual ~GiPointsImpl()
	{
	}
};
class GiLineSegmentImpl : public GiEntityImpl
{
public:
	GiLineSegmentImpl()
	{
	}
	virtual ~GiLineSegmentImpl()
	{
	}
};
class GiLinearImpl : public GiEntityImpl
{
public:
	GiLinearImpl()
	{
	}
	virtual ~GiLinearImpl()
	{
	}

	AcArray<GeLine3d*> lines;
};
class GiRayImpl : public GiEntityImpl
{
public:
	GiRayImpl()
	{
	}
	virtual ~GiRayImpl()
	{
	}

	AcArray<GeRay3d*> rays;
};
class GiMeshImpl : public GiEntityImpl
{
public:
	GiMeshImpl()
	{
		this->pTriangleMeshGeometrys = new AcArray<GiTriangleMeshGeometry*>();
	}
	virtual ~GiMeshImpl()
	{
		delete this->pTriangleMeshGeometrys;
		this->pTriangleMeshGeometrys = NULL;
	}

	AcArray<GiTriangleMeshGeometry*>* pTriangleMeshGeometrys;
};
class GiEntityGeometryManagerImpl : public GiObjectImpl
{
public:
	GiEntityGeometryManagerImpl()
	{
		this->index = 0;
		this->notUsedIndex = 0;
	}
	virtual ~GiEntityGeometryManagerImpl()
	{
	}

	AcArray<unsigned int> notUsedIds;
	unsigned int notUsedIndex;

	unsigned int index;
	AcArray<GiEntityGeometry*> entitys;
	AcArray<GiEntityGeometry*> erasedEntityGeometrys;
};

class GiEntityReactorImpl : public RxObjectReactorImpl
{
public:
	GiEntityReactorImpl()
	{
	}
	virtual ~GiEntityReactorImpl()
	{
	}
};
class GiEntityIteratorImpl : public GiObjectImpl
{
public:
	GiEntityIteratorImpl()
	{
	}
	virtual ~GiEntityIteratorImpl()
	{
	}

	AcIterator<GiEntity*> iterator;
};
class GiEntityManagerImpl : public RxObjectImpl
{
public:
	GiEntityManagerImpl()
	{
		this->entityMutex = new GiEntityMutex();
		this->entityIndexs = new GiEntityIndexPointerTypeArray();
	}
	virtual ~GiEntityManagerImpl()
	{
	}

	GiEntityMutex* entityMutex;

	AcArray<GiEntity*> entitys;
	AcArray<GiEntityReactor*> entityReactors;

	GiEntityIndexPointerTypeArray* entityIndexs;
};

/** -------------------------------------------------------------------- */


/** 材质 */
/** -------------------------------------------------------------------- */

class GiMaterialImpl : public GiObjectImpl
{
public:
	GiMaterialImpl()
	{
		this->color = 0;
		this->transparency = 0;
		this->colorIndex = 0;
	}
	virtual ~GiMaterialImpl()
	{
	}

	unsigned int color;
	unsigned char transparency;
	unsigned char colorIndex;
};
class GiPointMaterialImpl : public GiMaterialImpl
{
public:
	GiPointMaterialImpl()
	{
		this->size = 0;
	}
	virtual ~GiPointMaterialImpl()
	{
	}

	unsigned char size;
};
class GiLineMaterialImpl : public GiMaterialImpl
{
public:
	GiLineMaterialImpl()
	{
		this->lineWidth = 0;
	}
	virtual ~GiLineMaterialImpl()
	{
	}

	double lineWidth;
};
class GiLineDashedMaterialImpl : public GiMaterialImpl
{
public:
	GiLineDashedMaterialImpl()
	{
		this->lineWidth = 0;
		this->dashSize = 3.0;
		this->gapSize = 3.0;
	}
	virtual ~GiLineDashedMaterialImpl()
	{
	}

	double dashSize;
	double gapSize;
	double lineWidth;
};
class GiLinePixelDashedMaterialImpl : public GiMaterialImpl
{
public:
	GiLinePixelDashedMaterialImpl()
	{
		this->lineWidth = 0;
		this->dashSize = 3.0;
		this->gapSize = 3.0;
		this->pixelRatio = 1.0;
	}
	virtual ~GiLinePixelDashedMaterialImpl()
	{
	}

	double pixelRatio;
	double dashSize;
	double gapSize;
	double lineWidth;
};
class GiMeshMaterialImpl : public GiMaterialImpl
{
public:
	GiMeshMaterialImpl()
	{
	}
	virtual ~GiMeshMaterialImpl()
	{
	}
};
class GiMaterialReactorImpl : public RxObjectReactorImpl
{
public:
	GiMaterialReactorImpl()
	{
	}
	virtual ~GiMaterialReactorImpl()
	{
	}
};
class GiMaterialIteratorImpl : public GiObjectImpl
{
public:
	GiMaterialIteratorImpl()
	{
	}
	virtual ~GiMaterialIteratorImpl()
	{
	}

	AcIterator<GiMaterial*> iterator;
};
class GiMaterialManagerImpl : public GiObjectImpl
{
public:
	GiMaterialManagerImpl()
	{
		this->maxInitMaterial = 0;
		this->defaultLineBeginIndex = 0;
		this->defaultHighlightLineBeginIndex = 0;
		this->defaultLockLineBeginIndex = 0;
		this->defaultMeshBeginIndex = 0;
		this->defaultHighlightMeshBeginIndex = 0;
		this->defaultPointBeginIndex = 0;
		this->pMaterials = new AcArray<GiMaterial*>();
		this->pMaterialReactors = new AcArray<GiMaterialReactor*>();
	}
	virtual ~GiMaterialManagerImpl()
	{
		delete this->pMaterials;
		delete this->pMaterialReactors;
	}

	int maxInitMaterial;
	int defaultLineBeginIndex;
	int defaultHighlightLineBeginIndex;
	int defaultLockLineBeginIndex;
	int defaultMeshBeginIndex;
	int defaultHighlightMeshBeginIndex;
	int defaultPointBeginIndex;
	AcArray<GiMaterial*>* pMaterials;
	AcArray<GiMaterialReactor*>* pMaterialReactors;
	AcArray<GiMaterial*> createMaterials;
};

/** -------------------------------------------------------------------- */



/** 绘图器 */
/** -------------------------------------------------------------------- */

class GiImpFill : public GiObjectImpl
{
public:
	GiImpFill()
	{
		this->deviation = 0.0;
	}
	virtual ~GiImpFill()
	{
	}

	double deviation;
};


class GiWorldGeometryImpl : public RxObjectImpl
{
public:
	GiWorldGeometryImpl()
	{
		this->pEntityTraits = NULL;
	}
	virtual ~GiWorldGeometryImpl()
	{
	}

	AcArray<GiWorldGeometryStake*> worldGeometryStakes;
	GiEntityTraits* pEntityTraits;
	AcArray<GeMatrix3d> modelTransformStack;
};
class GiWorldDrawGeometryImpl : public GiWorldGeometryImpl
{
public:
	GiWorldDrawGeometryImpl()
	{
	}
	virtual ~GiWorldDrawGeometryImpl()
	{
	}
};


class GiSubEntityTraitsImpl : public GiObjectImpl
{
public:
	GiSubEntityTraitsImpl()
	{
		this->colorIndex = 7;
		this->isTrueColor = false;
		this->lineWidth = Db::LineWeight::kLnWtByLwDefault;
		this->fillType = GiFillType::kGiFillNever;
		this->lineTypeScale = 0.0;
		this->thickness = 0.0;
		this->sectionable = false;
		this->isVisibility = true;
		this->pFill = new GiFill();
	}
	virtual ~GiSubEntityTraitsImpl()
	{
	}

	unsigned short colorIndex;
	bool isTrueColor;
	CmEntityColor trueColor;
	DbObjectId layerId;
	DbObjectId lineType;
	Db::LineWeight lineWidth;
	GiFillType fillType;
	double lineTypeScale;
	double thickness;
	bool sectionable;
	CmTransparency transparency;
	GiFill* pFill;
	bool isVisibility;
};
class GiEntityTraitsImpl : public GiSubEntityTraitsImpl
{
public:
	GiEntityTraitsImpl()
	{
		this->isMatrix3d = false;
		this->pEntity = NULL;
		this->document = NULL;
		this->parentEntity = NULL;
		this->stake = NULL;
	}
	virtual ~GiEntityTraitsImpl()
	{
	}

	DbEntity* pEntity;
	ApDocument* document;
	DbEntity* parentEntity;
	bool isMatrix3d;
	GeMatrix3d matrix3d;
	DbEntityStake* stake;
};


class GiWorldDrawImpl : public GiObjectImpl
{
public:
	GiWorldDrawImpl()
	{
		this->pDocument = NULL;
		this->pEntityTraits = NULL;
		this->regenType = GiRegenType::kGiStandardDisplay;
	}
	virtual ~GiWorldDrawImpl()
	{
		if (this->pEntityTraits != NULL)
		{
			delete this->pEntityTraits;
			this->pEntityTraits = NULL;
		}
	}

	ApDocument* pDocument;
	GiRegenType regenType;
	GiEntityTraits* pEntityTraits;

	virtual bool isGiEntDraw()
	{
		return false;
	}

	virtual bool isGiDragDraw()
	{
		return false;
	}
};
class GiEntDrawImpl : public GiWorldDrawImpl
{
public:
	GiEntDrawImpl()
	{
		this->isInit = false;
		this->pEntityTraits = new GiEntityTraits();
		this->pWorldGeometry = new GiWorldDrawGeometry();
		((GiWorldDrawGeometryImpl*)this->pWorldGeometry->m_pImpl)->pEntityTraits = this->pEntityTraits;
	}
	virtual ~GiEntDrawImpl()
	{
		if (this->pWorldGeometry != NULL)
		{
			delete this->pWorldGeometry;
			this->pWorldGeometry = NULL;
		}
	}

	bool isInit;
	GiWorldDrawGeometry* pWorldGeometry;
	AcArray<unsigned int> entityIds;
	AcArray<unsigned int> layerTableRecordIds;

	virtual bool isGiEntDraw()
	{
		return true;
	}
};
class GiWorldDrawManagerImpl : public GiObjectImpl
{
public:
	GiWorldDrawManagerImpl()
	{
		this->pWorldDocument = NULL;
		this->entityHighlander = new GiEntityHighlander();
	}
	virtual ~GiWorldDrawManagerImpl()
	{
		realloc_arry_all(this->entDraws);
		realloc_arry_all(this->dragDraws);
	}

	ApDocument* pWorldDocument;
	AcArray<GiWorldDraw*> entDraws;
	AcArray<GiWorldDraw*> dragDraws;
	GiEntityHighlander* entityHighlander;
};
class GiEntityHighlanderImpl : public GiObjectImpl
{
public:
	GiEntityHighlanderImpl()
	{
		this->lastEntity = NULL;
		this->lastMaterial = NULL;
	}
	virtual ~GiEntityHighlanderImpl()
	{
	}

	GiEntity* lastEntity;
	GiMaterial* lastMaterial;
};


/** -------------------------------------------------------------------- */




/** -------------------------------------------------------------------- */


class GiGeometryExtentImpl : public GiObjectImpl
{
public:
	GiGeometryExtentImpl()
	{
		this->document = NULL;
		this->modelSpace = NULL;
		this->markRefresh = true;
		for (int i = 0; i < 4; i++)
		{
			for (int u = 0; u < 4; u++)
			{
				this->displayMatrix.entry[i][u] = 0.0;
			}
		}
		this->viewExtents = new AcArray<GiGeometryExtentView*>();
	}
	virtual ~GiGeometryExtentImpl()
	{
	}

	ApDocument* document;
	DbObjectId modelSpace;
	GeMatrix3d displayMatrix;
	bool markRefresh;
	AcArray<GiGeometryExtentView*>* viewExtents;
};
class GiGeometryExtentManagerImpl : public GiObjectImpl
{
public:
	GiGeometryExtentManagerImpl()
	{
		this->isDrag = false;
		this->activeViewportId = NULL;
		this->isLock.store(false);
		this->bUpdateMatrix.store(false);
		this->bUpdateExtent.store(false);
	}
	virtual ~GiGeometryExtentManagerImpl()
	{
	}

	bool isDrag;
	std::atomic<bool> isLock;
	std::atomic<bool> bUpdateMatrix;
	std::atomic<bool> bUpdateExtent;
	DbObjectId activeViewportId;
	AcArray<GiGeometryExtent*> extents;
	AcArray<DbEntity*> drawEntitys;
};

/** -------------------------------------------------------------------- */




class GiTextStyleImpl : public GiObjectImpl
{
public:
	GiTextStyleImpl()
	{
		this->fontName = "";
		this->bigFontName = "";
		this->textSize = 3.5;
		this->xScale = 1;
		this->obliqueAngle = 0;
		this->trPercent = 0;

		this->isBackward = false;
		this->isUpsideDown = false;
		this->isVertical = false;

		this->isOverlined = false;
		this->isUnderlined = false;
		this->isStrikethrough = false;

		this->styleName = "";

		this->pTypeface = "";
		this->bold = false;
		this->italic = false;
		this->charset = Charset::kAnsiCharset;
		this->pitch = FontUtils::FontPitch::kDefault;
		this->family = FontUtils::FontFamily::kDecorative;
	}
	virtual ~GiTextStyleImpl()
	{
	}

	AcString fontName;
	AcString bigFontName;
	double textSize;
	double xScale;
	double obliqueAngle;
	double trPercent;

	Adesk::Boolean isBackward;
	Adesk::Boolean isUpsideDown;
	Adesk::Boolean isVertical;

	Adesk::Boolean isOverlined;
	Adesk::Boolean isUnderlined;
	Adesk::Boolean isStrikethrough;

	AcString styleName;

	AcString pTypeface;
	Adesk::Boolean bold;
	Adesk::Boolean italic;
	Charset charset;
	FontUtils::FontPitch pitch;
	FontUtils::FontFamily family;
};


#endif