#ifndef DBOBJECTMETHODS_H
#define DBOBJECTMETHODS_H


#define DB_DECLARE_MEMBERS(CLASSNAME)\
	RX_DECLARE_MEMBERS(CLASSNAME)\
	virtual Acad::ErrorStatus dwgInFields(DbDwgFiler* pFiler);\
	virtual Acad::ErrorStatus dwgOutFields(DbDwgFiler* pFiler) const;\

#define DB_DECLARE_MEMBERS_VIRTUAL(CLASSNAME)\
	RX_DECLARE_MEMBERS_VIRTUAL(CLASSNAME)\
	virtual Acad::ErrorStatus dwgInFields(DbDwgFiler* pFiler);\
	virtual Acad::ErrorStatus dwgOutFields(DbDwgFiler* pFiler) const;\


#define DB_ENTITY_DECLARE_MEMBERS(CLASSNAME)\
	DB_DECLARE_MEMBERS(CLASSNAME)\
	virtual bool subWorldDraw(GiWorldDraw* pWd) const;\


#define DB_CURVE_DECLARE_MEMBERS(CLASSNAME)\
	DB_ENTITY_DECLARE_MEMBERS(CLASSNAME)\
	Adesk::Boolean isClosed() const;\
	Adesk::Boolean isPeriodic() const;\
	Adesk::Boolean isPlanar() const;\
	Acad::ErrorStatus getStartParam(double&) const;\
	Acad::ErrorStatus extend(double) const;\
	Acad::ErrorStatus extend(Adesk::Boolean, const GePoint3d&) const;\


#define DB_ENTITY_DECLARE_OVERRIDE_METHOD\
    virtual Acad::ErrorStatus subGetGeomExtents(DbExtents& extents) const;\
    virtual Acad::ErrorStatus subTransformBy(const GeMatrix3d& xform);\
    virtual Acad::ErrorStatus subGetGripPoints(\
        DbGripDataPtrArray& grips,\
        const double curViewUnitSize,\
        const int gripSize,\
        const GeVector3d& curViewDir,\
        const int bitflags) const;\
    virtual Acad::ErrorStatus subGetOsnapPoints(\
        Db::OsnapMode osnapMode,\
        Adesk::GsMarker gsSelectionMark,\
        const GePoint3d& pickPoint,\
        const GePoint3d& lastPoint,\
        const GeMatrix3d& viewXform,\
        GePoint3dArray& snapPoints,\
        DbIntArray& geomIds) const;\
    virtual Acad::ErrorStatus subMoveGripPointsAt(\
        const DbIntArray& indices,\
        const GeVector3d& offset);\
    virtual Acad::ErrorStatus subMoveGripPointsAt(const\
        DbVoidPtrArray& gripAppData,\
        const GeVector3d& offset,\
        const int bitflags);\
    virtual Acad::ErrorStatus subIntersectWith(\
        const DbEntity* pEnt,\
        Db::Intersect intType,\
        GePoint3dArray& points,\
        Adesk::GsMarker thisGsMarker = 0,\
        Adesk::GsMarker otherGsMarker = 0) const;\


#define DB_CURVE_DECLARE_OVERRIDE_METHOD\
	    DB_ENTITY_DECLARE_OVERRIDE_METHOD\






#endif