
#ifndef DBENTITY_H
#define DBENTITY_H

#include "acadstrc.h"
#include "acgidefs.h"
#include "export.h"
#include "AcArray.h"
#include "DbObject.h"
#include "CmColor.h"
#include "CmTransparency.h"
#include "DbFullSubentPath.h"
#include "GePoint3dArray.h"
#include "DbVoidPtrArray.h"
#include "DbIntArray.h"
#include "DbGripDataPtrArray.h"


class GePlane;
class GeMatrix3d;
class GiMapper;
class DbExtents;
class DbFullSubentPathArray;
class DbFullSubentPath;
class GeVector3d;
class GePoint3d;





class DbEntity : public DbObject
{
public:
    virtual ~DbEntity();
    DB_ENTITY_DECLARE_MEMBERS(DbEntity);

    DbObjectId blockId() const;

    CmColor color() const;

    // Deprecated method. Calls the above overload. Marked final to prevent overrides
    virtual Acad::ErrorStatus setColor(const CmColor& color) final;
    virtual Acad::ErrorStatus setColor(const CmColor& color, bool doSubents) final;
    UInt16 colorIndex() const;
    
    virtual Acad::ErrorStatus setColorIndex(UInt16 color);
    virtual Acad::ErrorStatus setColorIndex(UInt16 color, bool doSubents);
    CmEntityColor entityColor() const;

    CmTransparency transparency() const;
    virtual Acad::ErrorStatus setTransparency(const CmTransparency& trans, bool doSubents = true);

    Acad::ErrorStatus layer(AcString& sName) const;
    ACHAR* layer() const;  // deprecated
#if EMSDK
    emstring emsdk_layer() const;
#endif
    DbObjectId layerId() const;
    
    virtual Acad::ErrorStatus   setLayer(const ACHAR* newVal, Adesk::Boolean doSubents = true, bool allowHiddenLayer = false);
    virtual Acad::ErrorStatus   setLayer(DbObjectId newVal);
    virtual Acad::ErrorStatus   setLayer(DbObjectId newVal, Adesk::Boolean doSubents);
    virtual Acad::ErrorStatus   setLayer(DbObjectId newVal, Adesk::Boolean doSubents, bool allowHiddenLayer);
#if EMSDK
    virtual Acad::ErrorStatus emsdk_setLayer(const emstring& newVal);
    virtual Acad::ErrorStatus emsdk_setLayer(const emstring& newVal, Adesk::Boolean doSubents);
    virtual Acad::ErrorStatus emsdk_setLayer(const emstring& newVal, Adesk::Boolean doSubents, bool allowHiddenLayer);
#endif
    
    Acad::ErrorStatus linetype(AcString& sName) const;
    ACHAR* linetype() const;       // deprecated
    DbObjectId linetypeId() const;
#if EMSDK
    emstring emsdk_linetype() const;
#endif
    
    virtual Acad::ErrorStatus   setLinetype(const ACHAR* newVal, Adesk::Boolean doSubents = true);
    virtual Acad::ErrorStatus   setLinetype(DbObjectId newVal);
    virtual Acad::ErrorStatus   setLinetype(DbObjectId newVal, Adesk::Boolean doSubents);
#if EMSDK
    virtual Acad::ErrorStatus emsdk_setLinetype(const emstring& newVal);
    virtual Acad::ErrorStatus emsdk_setLinetype(const emstring& newVal, Adesk::Boolean doSubents);
#endif

    double linetypeScale() const;
    
    virtual Acad::ErrorStatus setLinetypeScale(double newval, bool doSubents = true);

    Db::Visibility visibility() const;
    virtual Acad::ErrorStatus setVisibility(Db::Visibility newVal, bool doSubents = true) final;

    Db::LineWeight lineWeight() const;
    
    virtual Acad::ErrorStatus setLineWeight(Db::LineWeight newVal, bool doSubents = true);

    virtual Db::CollisionType collisionType() const;

    RxObject* clone() const override;

    virtual bool castShadows() const;
    
    virtual void setCastShadows(bool newVal);
    
    virtual bool receiveShadows() const;
    
    virtual void setReceiveShadows(bool newVal);

    Acad::ErrorStatus setPropertiesFrom(const DbEntity* pEntity, bool doSubents = true);

    virtual bool isPlanar() const { return false; }
    virtual Acad::ErrorStatus getPlane(GePlane&, Db::Planarity&) const
    {
        return Acad::eNotApplicable;
    }

    
    virtual void getEcs(GeMatrix3d& retVal) const;

    virtual Acad::ErrorStatus getGeomExtents(DbExtents& extents) const final;

    // Subentity acquisition and manipulation
    //
    virtual Acad::ErrorStatus addSubentPaths(const DbFullSubentPathArray& newPaths) final;

    virtual Acad::ErrorStatus deleteSubentPaths(const DbFullSubentPathArray& paths) final;

    virtual Acad::ErrorStatus transformSubentPathsBy(
        const DbFullSubentPathArray& paths,
        const GeMatrix3d& xform) final;

    virtual Acad::ErrorStatus getGripPointsAtSubentPath(
        const DbFullSubentPath& path,
        DbGripDataPtrArray& grips,
        const double curViewUnitSize,
        const int gripSize,
        const GeVector3d& curViewDir,
        const int bitflags) const final;

    virtual Acad::ErrorStatus moveGripPointsAtSubentPaths(
        const DbFullSubentPathArray& paths,
        const DbVoidPtrArray& gripAppData,
        const GeVector3d& offset,
        const int bitflags) final;

    virtual Acad::ErrorStatus getSubentPathGeomExtents(
        const DbFullSubentPath& path,
        DbExtents& extents) const final;

    virtual Acad::ErrorStatus getSubentPathsAtGsMarker(
        Db::SubentType type,
        Adesk::GsMarker gsMark,
        const GePoint3d& pickPoint,
        const GeMatrix3d& viewXform,
        int& numPaths,
        DbFullSubentPath*& subentPaths,
        int numInserts = 0,
        DbObjectId* entAndInsertStack= nullptr) const final;

    virtual Acad::ErrorStatus getGsMarkersAtSubentPath(
        const DbFullSubentPath& subPath,
        AcArray<Adesk::GsMarker>& gsMarkers) const final;

    virtual Acad::ErrorStatus highlight(
        const DbFullSubentPath& subId = 0,
        const bool highlightAll
        = false) const final;
    virtual Acad::ErrorStatus unhighlight(
        const DbFullSubentPath& subId = 0,
        const bool highlightAll = false) const final;
    virtual DbEntity* subentPtr(const DbFullSubentPath& id) const final;

    virtual Acad::ErrorStatus getOsnapPoints(
        Db::OsnapMode osnapMode,
        Adesk::GsMarker gsSelectionMark,
        const GePoint3d& pickPoint,
        const GePoint3d& lastPoint,
        const GeMatrix3d& viewXform,
        GePoint3dArray& snapPoints,
        DbIntArray& geomIds) const final;

    virtual Acad::ErrorStatus getOsnapPoints(
        Db::OsnapMode osnapMode,
        Adesk::GsMarker gsSelectionMark,
        const GePoint3d& pickPoint,
        const GePoint3d& lastPoint,
        const GeMatrix3d& viewXform,
        GePoint3dArray& snapPoints,
        DbIntArray& geomIds,
        const GeMatrix3d& insertionMat) const final;

    virtual bool isContentSnappable() const final;

    virtual Acad::ErrorStatus getGripPoints(
        GePoint3dArray& gripPoints,
        DbIntArray& osnapModes,
        DbIntArray& geomIds) const;

    virtual Acad::ErrorStatus moveGripPointsAt(
        const DbIntArray& indices,
        const GeVector3d& offset) final;

    virtual Acad::ErrorStatus getGripPoints(
        DbGripDataPtrArray& grips,
        const double curViewUnitSize, 
        const int gripSize,
        const GeVector3d& curViewDir,
        const int bitflags) const final;

    virtual Acad::ErrorStatus moveGripPointsAt(
        const DbVoidPtrArray& gripAppData,
        const GeVector3d& offset,
        const int bitflags) final;

    virtual Acad::ErrorStatus getStretchPoints(GePoint3dArray& stretchPoints) const final;

    virtual Acad::ErrorStatus moveStretchPointsAt(
        const DbIntArray& indices,
        const GeVector3d& offset) final;

    virtual Acad::ErrorStatus intersectWith(
        const DbEntity* pEnt,
        Db::Intersect intType,
        GePoint3dArray& points,
        Adesk::GsMarker thisGsMarker = 0,
        Adesk::GsMarker otherGsMarker = 0) const final;

    virtual Acad::ErrorStatus intersectWith(
        const DbEntity* pEnt,
        Db::Intersect intType,
        const GePlane& projPlane,
        GePoint3dArray& points,
        Adesk::GsMarker thisGsMarker = 0,
        Adesk::GsMarker otherGsMarker = 0) const final;

    Acad::ErrorStatus boundingBoxIntersectWith(
        const DbEntity* pEnt,
        Db::Intersect intType,
        GePoint3dArray& points,
        Adesk::GsMarker thisGsMarker,
        Adesk::GsMarker otherGsMarker) const;

    Acad::ErrorStatus boundingBoxIntersectWith(
        const DbEntity* pEnt,
        Db::Intersect intType,
        const GePlane& projPlane,
        GePoint3dArray& points,
        Adesk::GsMarker thisGsMarker,
        Adesk::GsMarker otherGsMarker) const;

    virtual void  list() const final;

    virtual Acad::ErrorStatus transformBy(const GeMatrix3d& xform) final;
    virtual Acad::ErrorStatus getTransformedCopy(const GeMatrix3d& xform, DbEntity*& pEnt) const final;
    virtual Acad::ErrorStatus explode(DbVoidPtrArray& entitySet) const final;

    void recordGraphicsModified(bool setModified = true);

    Acad::ErrorStatus draw();
    virtual void close();
    
    virtual void saveAs(GiWorldDraw* mode, Db::SaveType st);

    virtual Acad::ErrorStatus getCompoundObjectTransform(GeMatrix3d& xMat) const final;

    void setDatabaseDefaults();
    void setDatabaseDefaults(DbDatabase* pDb);
    
    virtual void subSetDatabaseDefaults(DbDatabase* pDb);

    GiDrawable* drawable() override;

    // GiDrawable interface
    //
    bool bounds(DbExtents& _bounds) const;

    virtual bool cloneMeForDragging() final;
    virtual bool hideMeForDragging() const final;

    virtual void dragStatus(const Db::DragStat status);
    virtual void gripStatus(const Db::GripStat status) final;
    virtual void subentGripStatus(const Db::GripStat status, const DbFullSubentPath& subentity) final;

    virtual Acad::ErrorStatus getGripEntityUCS(
        const void* pGripAppData,
        GeVector3d& normalVec,
        GePoint3d& origin,
        GeVector3d& xAxis) const final;


em_protected:
    // Makes DbEntity an abstract class.
    DbEntity();

public:
    // DbOsnapOverrule
    virtual Acad::ErrorStatus subGetOsnapPoints(
        Db::OsnapMode osnapMode,
        Adesk::GsMarker gsSelectionMark,
        const GePoint3d& pickPoint,
        const GePoint3d& lastPoint,
        const GeMatrix3d& viewXform,
        GePoint3dArray& snapPoints,
        DbIntArray& geomIds) const;
    
    virtual Acad::ErrorStatus subGetOsnapPoints(
        Db::OsnapMode osnapMode,
        Adesk::GsMarker gsSelectionMark,
        const GePoint3d& pickPoint,
        const GePoint3d& lastPoint,
        const GeMatrix3d& viewXform,
        GePoint3dArray& snapPoints,
        DbIntArray& geomIds,
        const GeMatrix3d& insertionMat) const;

    
    virtual bool subIsContentSnappable() const;

    // DbTransformOverrule
    virtual Acad::ErrorStatus subTransformBy(const GeMatrix3d& xform);
    virtual Acad::ErrorStatus subGetTransformedCopy(const GeMatrix3d& xform,DbEntity*& pEnt) const;
    virtual Acad::ErrorStatus subExplode(DbVoidPtrArray& entitySet) const;
    virtual bool subCloneMeForDragging();
    virtual bool subHideMeForDragging() const;

    // DbGripOverrule
    virtual Acad::ErrorStatus subGetGripPoints(
        GePoint3dArray& gripPoints,
        DbIntArray& osnapModes,
        DbIntArray& geomIds) const;

    
    virtual Acad::ErrorStatus subGetGripPoints(
        DbGripDataPtrArray& grips,
        const double curViewUnitSize, 
        const int gripSize,
        const GeVector3d& curViewDir, 
        const int bitflags) const;

    
    virtual Acad::ErrorStatus subMoveGripPointsAt(const 
        DbIntArray& indices,
        const GeVector3d& offset);

    
    virtual Acad::ErrorStatus subMoveGripPointsAt(const 
        DbVoidPtrArray& gripAppData,
        const GeVector3d& offset,
        const int bitflags);

    
    virtual Acad::ErrorStatus subGetStretchPoints(
        GePoint3dArray& stretchPoints) const;

    
    virtual Acad::ErrorStatus subMoveStretchPointsAt(
        const DbIntArray& indices,
        const GeVector3d& offset);

    
    virtual Acad::ErrorStatus subGetGeomExtents(DbExtents& extents) const;

    // DbSubentityOverrule related
    virtual Acad::ErrorStatus subAddSubentPaths(const DbFullSubentPathArray& newPaths);

    
    virtual Acad::ErrorStatus subDeleteSubentPaths(const DbFullSubentPathArray& paths);

    
    virtual Acad::ErrorStatus subTransformSubentPathsBy(
        const DbFullSubentPathArray& paths,
        const GeMatrix3d& xform);

    
    virtual Acad::ErrorStatus subGetGripPointsAtSubentPath(
        const DbFullSubentPath& path,
        DbGripDataPtrArray& grips,
        const double curViewUnitSize,
        const int gripSize,
        const GeVector3d& curViewDir,
        const int bitflags) const;

    
    virtual Acad::ErrorStatus subMoveGripPointsAtSubentPaths(
        const DbFullSubentPathArray& paths,
        const DbVoidPtrArray& gripAppData,
        const GeVector3d& offset,
        const int bitflags);

    
    virtual Acad::ErrorStatus subGetSubentPathGeomExtents(const DbFullSubentPath& path, DbExtents& extents) const;

    
    virtual Acad::ErrorStatus subGetSubentPathsAtGsMarker(
        Db::SubentType type,
        Adesk::GsMarker gsMark,
        const GePoint3d& pickPoint,
        const GeMatrix3d& viewXform,
        int& numPaths,
        DbFullSubentPath*& subentPaths,
        int numInserts = 0,
        DbObjectId* entAndInsertStack = nullptr) const;

    
    virtual Acad::ErrorStatus subGetGsMarkersAtSubentPath(
        const DbFullSubentPath& subPath,
        AcArray<Adesk::GsMarker>& gsMarkers) const;

    
    virtual DbEntity* subSubentPtr(const DbFullSubentPath& id) const;

    
    virtual void subGripStatus(const Db::GripStat status);
    
    virtual void subSubentGripStatus(const Db::GripStat status, const DbFullSubentPath& subentity);

    // DbHighlightOverrule related
    
    virtual Acad::ErrorStatus subHighlight(
        const DbFullSubentPath& subId = 0,
        const bool highlightAll = false) const;
    
    virtual Acad::ErrorStatus subUnhighlight(
        const DbFullSubentPath& subId = 0,
        const bool highlightAll = false) const;

    // DbVisibilityOverrule related
    virtual Db::Visibility  subVisibility() const;
    virtual Acad::ErrorStatus subSetVisibility(Db::Visibility newVal,
    bool doSubents = true);

    // DbGeometryOverrule related
    
    virtual Acad::ErrorStatus subIntersectWith(
        const DbEntity* pEnt,
        Db::Intersect intType,
        GePoint3dArray& points,
        Adesk::GsMarker thisGsMarker = 0,
        Adesk::GsMarker otherGsMarker = 0) const;

    
    virtual Acad::ErrorStatus subIntersectWith(
        const DbEntity* pEnt,
        Db::Intersect intType,
        const GePlane& projPlane,
        GePoint3dArray& points,
        Adesk::GsMarker thisGsMarker = 0,
        Adesk::GsMarker otherGsMarker = 0) const;

    
    virtual Acad::ErrorStatus subGetGripEntityUCS(
        const void* pGripAppData,
        GeVector3d& normalVec, 
        GePoint3d& origin,
        GeVector3d& xAxis) const;

    
    virtual Acad::ErrorStatus subGetCompoundObjectTransform(GeMatrix3d& xMat) const;

    friend class DbPropertiesOverrule;
    friend class DbOsnapOverrule;
    friend class DbTransformOverrule;
    friend class DbGripOverrule;
    friend class DbSubentityOverrule;
    friend class DbHighlightOverrule;
    friend class DbHighlightStateOverrule;
    friend class DbVisibilityOverrule;
    friend class DbGeometryOverrule;
    friend class DbGripEntityOverrule;

public:
    Acad::ErrorStatus pushHighlight(const DbFullSubentPath& subId, GiHighlightStyle highlightStyle);
    Acad::ErrorStatus  popHighlight(const DbFullSubentPath& subId);
    GiHighlightStyle highlightState(const DbFullSubentPath& subId);


protected:
    // DbHighlightStateOverrule related
    virtual Acad::ErrorStatus subPushHighlight(const DbFullSubentPath& subId, GiHighlightStyle highlightStyle);
    virtual Acad::ErrorStatus subPopHighlight(const DbFullSubentPath& subId);
    virtual GiHighlightStyle subHighlightState(const DbFullSubentPath& subId);

    // No copy construction or assignment (but there is a copyFrom method)
    DbEntity(const DbEntity&) = delete;
    DbEntity& operator = (const DbEntity&) = delete;
};






#endif



