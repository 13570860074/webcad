#include "DbArcDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"


// Default constructor
DbArcDimension::DbArcDimension() {
    this->m_pImpl = new DbArcDimensionImpl();
}
DbArcDimension::DbArcDimension(
    const GePoint3d& centerPoint,
    const GePoint3d& xLine1Point,
    const GePoint3d& xLine2Point,
    const GePoint3d& arcPoint) {
    this->m_pImpl = new DbArcDimensionImpl();
    DB_IMP_ARCDIMENSION(this->m_pImpl)->centerPoint = centerPoint;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
DbArcDimension::DbArcDimension(
    const GePoint3d& centerPoint,
    const GePoint3d& xLine1Point,
    const GePoint3d& xLine2Point,
    const GePoint3d& arcPoint,
    const ACHAR* dimText,
    DbObjectId dimStyle) {
    this->m_pImpl = new DbArcDimensionImpl();
    DB_IMP_ARCDIMENSION(this->m_pImpl)->centerPoint = centerPoint;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
// Default destructor
DbArcDimension::~DbArcDimension() {

}



GePoint3d DbArcDimension::arcPoint() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->arcPoint;
}
Acad::ErrorStatus DbArcDimension::setArcPoint(const GePoint3d& arcPt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->arcPoint  = arcPt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
GePoint3d DbArcDimension::xLine1Point() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine1Point;
}    
Acad::ErrorStatus DbArcDimension::setXLine1Point(const GePoint3d& xLine1Pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine1Point = xLine1Pt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
GePoint3d DbArcDimension::xLine2Point() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine2Point;
}  
Acad::ErrorStatus DbArcDimension::setXLine2Point(const GePoint3d& xLine2Pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine2Point = xLine2Pt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GePoint3d DbArcDimension::centerPoint() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->centerPoint;
} 
Acad::ErrorStatus DbArcDimension::setCenterPoint(const GePoint3d& ctrPt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->centerPoint = ctrPt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbArcDimension::isPartial() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->isPartial;
}
Acad::ErrorStatus DbArcDimension::setIsPartial(const bool& partial) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->isPartial = partial;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
double DbArcDimension::arcStartParam() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->arcStartParam;
}   
Acad::ErrorStatus DbArcDimension::setArcStartParam(const double& arcParam) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->arcStartParam = arcParam;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
double DbArcDimension::arcEndParam() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->arcEndParam;
} 
Acad::ErrorStatus DbArcDimension::setArcEndParam(const double& arcParam) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->arcEndParam = arcParam;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbArcDimension::hasLeader() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->hasLeader;
}
Acad::ErrorStatus DbArcDimension::setHasLeader(const bool& leaderVal) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->hasLeader = leaderVal;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GePoint3d DbArcDimension::leader1Point() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->leader1Point;
}    
Acad::ErrorStatus DbArcDimension::setLeader1Point(const GePoint3d& ldr1Pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->leader1Point = ldr1Pt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GePoint3d DbArcDimension::leader2Point() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->leader2Point;
}
Acad::ErrorStatus DbArcDimension::setLeader2Point(const GePoint3d& ldr2Pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->leader2Point = ldr2Pt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

int DbArcDimension::arcSymbolType() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->arcSymbolType;
}

Acad::ErrorStatus DbArcDimension::setArcSymbolType(const int symbol) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->arcSymbolType = symbol;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}












Acad::ErrorStatus DbArcDimension::dwgInFields(DbDwgFiler* pFiler) {

    Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }

    GePoint3d arcPoint;
    pFiler->readPoint3d(&arcPoint);
    this->setArcPoint(arcPoint);

    GePoint3d xLine1Point;
    pFiler->readPoint3d(&xLine1Point);
    this->setXLine1Point(xLine1Point);

    GePoint3d xLine2Point;
    pFiler->readPoint3d(&xLine2Point);
    this->setXLine2Point(xLine2Point);

    GePoint3d centerPoint;
    pFiler->readPoint3d(&centerPoint);
    this->setCenterPoint(centerPoint);

    bool isPartial;
    pFiler->readBool(&isPartial);
    this->setIsPartial(isPartial);

    double arcStartParam;
    pFiler->readDouble(&arcStartParam);
    this->setArcStartParam(arcStartParam);

    double arcEndParam;
    pFiler->readDouble(&arcEndParam);
    this->setArcEndParam(arcEndParam);

    bool hasLeader;
    pFiler->readBool(&hasLeader);
    this->setHasLeader(hasLeader);

    GePoint3d leader1Point;
    pFiler->readPoint3d(&leader1Point);
    this->setLeader1Point(leader1Point);

    GePoint3d leader2Point;
    pFiler->readPoint3d(&leader2Point);
    this->setLeader2Point(leader2Point);

    int arcSymbolType;
    pFiler->readInt32(&arcSymbolType);
    this->setArcSymbolType(arcSymbolType);

    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArcDimension::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }

    pFiler->writePoint3d(this->arcPoint());
    pFiler->writePoint3d(this->xLine1Point());
    pFiler->writePoint3d(this->xLine2Point());
    pFiler->writePoint3d(this->centerPoint());
    pFiler->writeBool(this->isPartial());
    pFiler->writeDouble(this->arcStartParam());
    pFiler->writeDouble(this->arcEndParam());
    pFiler->writeBool(this->hasLeader());
    pFiler->writePoint3d(this->leader1Point());
    pFiler->writePoint3d(this->leader2Point());
    pFiler->writeInt32(this->arcSymbolType());

    return Acad::ErrorStatus::eOk;
}
bool DbArcDimension::subWorldDraw(GiWorldDraw* pWd) const {
    return DbDimension::subWorldDraw(pWd);
}


Acad::ErrorStatus DbArcDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArcDimension::subTransformBy(const GeMatrix3d& xform) {

    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbArcDimension::subGetGripPoints(
    DbGripDataPtrArray& grips,
    const double curViewUnitSize,
    const int gripSize,
    const GeVector3d& curViewDir,
    const int bitflags) const {



    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbArcDimension::subGetOsnapPoints(
    Db::OsnapMode osnapMode,
    Adesk::GsMarker gsSelectionMark,
    const GePoint3d& pickPoint,
    const GePoint3d& lastPoint,
    const GeMatrix3d& viewXform,
    GePoint3dArray& snapPoints,
    DbIntArray& geomIds) const {

    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbArcDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {


    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbArcDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArcDimension::subIntersectWith(
    const DbEntity* pEnt,
    Db::Intersect intType,
    GePoint3dArray& points,
    Adesk::GsMarker thisGsMarker,
    Adesk::GsMarker otherGsMarker) const {
    return Acad::ErrorStatus::eOk;
}