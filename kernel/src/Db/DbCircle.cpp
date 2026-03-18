#include "DbCircle.h"
#include "GeDoubleArray.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbDwgFiler.h"
#include "GePlane.h"
#include "DbLine.h"
#include "DbRay.h"
#include "DbXline.h"
#include "DbArc.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GeLine3d.h"
#include "GeCircArc3d.h"
#include "DbOsnapPointCompute.h"
#include "DbImpl.h"

DbCircle::DbCircle()
{
    this->m_pImpl = new DbCircleImpl();
}
DbCircle::DbCircle(const GePoint3d &cntr, const GeVector3d &nrm, double radius)
{
    this->m_pImpl = new DbCircleImpl();
    this->setCenter(cntr);
    this->setNormal(nrm);
    this->setRadius(radius);
}
DbCircle::~DbCircle()
{
}
bool DbCircle::subOpen(Db::OpenMode mode)
{
    DB_IMP_CIRCLE(this->m_pImpl)->bInitPoint = false;
    return DbCurve::subOpen(mode);
}
GePoint3d DbCircle::center() const
{
    return DB_IMP_CIRCLE(this->m_pImpl)->center;
}
Acad::ErrorStatus DbCircle::setCenter(const GePoint3d &_center)
{
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
    {
        DB_IMP_CIRCLE(this->m_pImpl)->center = _center;
    }
    return Acad::ErrorStatus::eOk;
}

double DbCircle::radius() const
{
    return DB_IMP_CIRCLE(this->m_pImpl)->radius;
}
Acad::ErrorStatus DbCircle::setRadius(double _radius)
{
    if (_radius < 0.000000002)
    {
        return Acad::ErrorStatus::eFail;
    }
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
    {
        DB_IMP_CIRCLE(this->m_pImpl)->radius = _radius;
    }
    return Acad::ErrorStatus::eOk;
}

double DbCircle::thickness() const
{
    return DB_IMP_CIRCLE(this->m_pImpl)->thickness;
}
Acad::ErrorStatus DbCircle::setThickness(double _thickness)
{
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
    {
        DB_IMP_CIRCLE(this->m_pImpl)->thickness = _thickness;
    }
    return Acad::ErrorStatus::eOk;
}

GeVector3d DbCircle::normal() const
{
    return DB_IMP_CIRCLE(this->m_pImpl)->normal;
}
Acad::ErrorStatus DbCircle::setNormal(const GeVector3d &_normal)
{
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
    {
        DB_IMP_CIRCLE(this->m_pImpl)->normal = _normal;
    }
    return Acad::ErrorStatus::eOk;
}

double DbCircle::circumference() const
{
    return 2 * PI * this->radius();
}
Acad::ErrorStatus DbCircle::setCircumference(double _val)
{
    return this->setRadius(_val / (2 * PI));
}
double DbCircle::diameter() const
{
    return DB_IMP_CIRCLE(this->m_pImpl)->radius * 2;
}
Acad::ErrorStatus DbCircle::setDiameter(double _val)
{
    return this->setRadius(_val / 2.0);
}

Acad::ErrorStatus DbCircle::dwgInFields(DbDwgFiler *pFiler)
{

    Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
    if (es != Acad::eOk)
    {
        return (es);
    }

    GePoint3d center;
    pFiler->readPoint3d(&center);
    this->setCenter(center);

    double radius;
    pFiler->readDouble(&radius);
    this->setRadius(radius);

    GeVector3d normal;
    pFiler->readVector3d(&normal);
    this->setNormal(normal);

    double thickness;
    pFiler->readDouble(&thickness);
    this->setThickness(thickness);

    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::dwgOutFields(DbDwgFiler *pFiler) const
{

    Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
    if (es != Acad::eOk)
    {
        return (es);
    }

    pFiler->writePoint3d(this->center());

    pFiler->writeDouble(this->radius());

    pFiler->writeVector3d(this->normal());

    pFiler->writeDouble(this->thickness());

    return Acad::ErrorStatus::eOk;
}
bool DbCircle::subWorldDraw(GiWorldDraw *pWd) const
{
    return pWd->geometry().circle(this->center(), this->radius(), this->normal());
}

Acad::ErrorStatus DbCircle::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::subTransformBy(const GeMatrix3d &xform)
{
    GePoint3d startPoint;
    this->getStartPoint(startPoint);
    startPoint.transformBy(xform);
    GePoint3d midPoint;
    this->getPointAtParam(PI / 2.0, midPoint);
    midPoint.transformBy(xform);

    // 求圆心
    GePoint3d center = this->center();
    center.transformBy(xform);

    // 求法线
    GeVector3d normal = (startPoint - center).normal().crossProduct((midPoint - center).normal());

    // 求半径
    double radius = startPoint.distanceTo(center);

    this->setCenter(center);
    this->setRadius(radius);
    this->setNormal(normal);
    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbCircle::subGetGripPoints(
    DbGripDataPtrArray &grips,
    const double curViewUnitSize,
    const int gripSize,
    const GeVector3d &curViewDir,
    const int bitflags) const
{
    DbGripData *gripData = new DbGripData();
    gripData->setGripPoint(this->center());
    grips.append(gripData);

    GePoint3d pos;
    this->getPointAtParam(0, pos);
    gripData = new DbGripData();
    gripData->setGripPoint(pos);
    grips.append(gripData);

    this->getPointAtParam(PI / 2.0 * 1, pos);
    gripData = new DbGripData();
    gripData->setGripPoint(pos);
    grips.append(gripData);

    this->getPointAtParam(PI / 2.0 * 2, pos);
    gripData = new DbGripData();
    gripData->setGripPoint(pos);
    grips.append(gripData);

    this->getPointAtParam(PI / 2.0 * 3, pos);
    gripData = new DbGripData();
    gripData->setGripPoint(pos);
    grips.append(gripData);

    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::subGetOsnapPoints(
    Db::OsnapMode osnapMode,
    Adesk::GsMarker gsSelectionMark,
    const GePoint3d &pickPoint,
    const GePoint3d &lastPoint,
    const GeMatrix3d &viewXform,
    GePoint3dArray &snapPoints,
    DbIntArray &geomIds) const
{

    do
    {

        if (osnapMode == Db::OsnapMode::kOsModeNear)
        {
            GePoint3dArray points = ::osnap_near_to_circle(pickPoint, this, viewXform);
            for (int i = 0; i < points.length(); i++)
            {
                snapPoints.append(points[i]);
            }
        }
        else if (osnapMode == Db::OsnapMode::kOsModeCen)
        {
            GePoint3dArray points = ::osnap_center_to_circle(pickPoint, this, viewXform);
            for (int i = 0; i < points.length(); i++)
            {
                snapPoints.append(points[i]);
            }
        }
        else if (osnapMode == Db::OsnapMode::kOsModeQuad)
        {
            GePoint3dArray points = ::osnap_quad_to_circle(pickPoint, this, viewXform);
            for (int i = 0; i < points.length(); i++)
            {
                snapPoints.append(points[i]);
            }
        }
        else if (osnapMode == Db::OsnapMode::kOsModePerp)
        {
            GePoint3dArray points = ::osnap_perp_to_circle(pickPoint, lastPoint, this, viewXform);
            for (int i = 0; i < points.length(); i++)
            {
                snapPoints.append(points[i]);
            }
        }
        else if (osnapMode == Db::OsnapMode::kOsModeTan)
        {
            GePoint3dArray points = ::osnap_tan_to_circle(pickPoint, lastPoint, this, viewXform);
            for (int i = 0; i < points.length(); i++)
            {
                snapPoints.append(points[i]);
            }
        }

    } while (false);

    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbCircle::subMoveGripPointsAt(const DbIntArray &indices, const GeVector3d &offset)
{
    for (int i = 0; i < indices.length(); i++)
    {
        if (indices.at(i) == 0)
        {
            DB_IMP_CIRCLE(this->m_pImpl)->center += offset;
        }
        else
        {

            if (DB_IMP_CIRCLE(this->m_pImpl)->bInitPoint == false)
            {
                DB_IMP_CIRCLE(this->m_pImpl)->bInitPoint = true;
                this->getPointAtParam(PI / 2.0 * (indices.at(i) - 1.0), DB_IMP_CIRCLE(this->m_pImpl)->initPoint);
            }

            DB_IMP_CIRCLE(this->m_pImpl)->initPoint += offset;
            GePoint3d curPoint = DB_IMP_CIRCLE(this->m_pImpl)->initPoint;
            curPoint = curPoint.project(GePlane(this->center(), this->normal()), GeVector3d::kZAxis);
            DB_IMP_CIRCLE(this->m_pImpl)->radius = this->center().distanceTo(curPoint);
        }
    }
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::subMoveGripPointsAt(const DbVoidPtrArray &gripAppData, const GeVector3d &offset, const int bitflags)
{
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::subIntersectWith(const DbEntity *pEnt, Db::Intersect intType, GePoint3dArray &points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const
{

    if (pEnt == NULL)
    {
        return Acad::ErrorStatus::eFail;
    }

    if (DB_IMP_ENTITY(pEnt->m_pImpl)->isLine())
    {
        GePoint3dArray ps = ::osnap_inters_circle_to_lintseg(this, (DbLine *)pEnt, intType);
        for (int i = 0; i < ps.length(); i++)
        {
            points.append(ps[i]);
        }
    }
    else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isXline())
    {
        GePoint3dArray ps = ::osnap_inters_circle_to_line(this, (DbXline *)pEnt, intType);
        for (int i = 0; i < ps.length(); i++)
        {
            points.append(ps[i]);
        }
    }
    else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isRay())
    {
        GePoint3dArray ps = ::osnap_inters_circle_to_ray(this, (DbRay *)pEnt, intType);
        for (int i = 0; i < ps.length(); i++)
        {
            points.append(ps[i]);
        }
    }
    else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isCircle())
    {
        GePoint3dArray ps = ::osnap_inters_circle_to_circle(this, (DbCircle *)pEnt, intType);
        for (int i = 0; i < ps.length(); i++)
        {
            points.append(ps[i]);
        }
    }
    else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isArc())
    {
        GePoint3dArray ps = ::osnap_inters_circle_to_arc(this, (DbArc *)pEnt, intType);
        for (int i = 0; i < ps.length(); i++)
        {
            points.append(ps[i]);
        }
    }
    else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isPolyline())
    {
        GePoint3dArray ps = ::osnap_inters_circle_to_polyline(this, (DbPolyline *)pEnt, intType);
        for (int i = 0; i < ps.length(); i++)
        {
            points.append(ps[i]);
        }
    }
    else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isBlockReference())
    {

    }

    return Acad::ErrorStatus::eOk;
}

bool DbCircle::isClosed() const
{
    return true;
}
bool DbCircle::isPeriodic() const
{
    return false;
}
bool DbCircle::isPlanar() const
{
    return true;
}
Acad::ErrorStatus DbCircle::getPlane(GePlane &plane, Db::Planarity &planarity) const
{
    plane.set(this->center(), this->normal());
    planarity = Db::Planarity::kPlanar;
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getStartParam(double &param) const
{
    param = 0.0;
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getEndParam(double &param) const
{
    param = PI * 2.0;
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getStartPoint(GePoint3d &pos) const
{
    double param = 0.0;
    this->getStartParam(param);
    return this->getPointAtParam(param, pos);
}
Acad::ErrorStatus DbCircle::getEndPoint(GePoint3d &pos) const
{
    double param = 0.0;
    this->getEndParam(param);
    return this->getPointAtParam(param, pos);
}
Acad::ErrorStatus DbCircle::getPointAtParam(double param, GePoint3d &pos) const
{
    GeMatrix3d mat;
    mat.setToPlaneToWorld(this->normal());
    GeVector3d refVec = GeVector3d::kXAxis;
    refVec.transformBy(mat);
    refVec.normalize();

    pos = this->center() + refVec * this->radius();
    pos.rotateBy(param, this->normal(), this->center());
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getParamAtPoint(const GePoint3d &, double &) const
{
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getDistAtParam(double, double &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getParamAtDist(double, double &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getDistAtPoint(const GePoint3d &, double &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getPointAtDist(double, GePoint3d &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getFirstDeriv(double, GeVector3d &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getFirstDeriv(const GePoint3d &, GeVector3d &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getSecondDeriv(double, GeVector3d &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getSecondDeriv(const GePoint3d &, GeVector3d &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getClosestPointTo(const GePoint3d &, GePoint3d &, bool) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getClosestPointTo(const GePoint3d &, const GeVector3d &, GePoint3d &, bool) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getOrthoProjectedCurve(const GePlane &, DbCurve *&) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getProjectedCurve(const GePlane &, const GeVector3d &, DbCurve *&) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getOffsetCurves(double, DbVoidPtrArray &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getSpline(DbSpline *&) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getSplitCurves(const GeDoubleArray &, DbVoidPtrArray &curveSegments) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getSplitCurves(const GePoint3dArray &, DbVoidPtrArray &) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::extend(double _val)
{
    return this->setRadius(this->radius() + _val);
}
Acad::ErrorStatus DbCircle::extend(bool, const GePoint3d &)
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getArea(double &val) const
{
    val = PI * this->radius() * this->radius();
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::reverseCurve()
{
    return this->setNormal(this->normal().negate());
}
Acad::ErrorStatus DbCircle::getGeCurve(GeCurve3d *&pGeCurve, const GeTol &tol) const
{
    GeMatrix3d mat;
    mat.setToWorldToPlane(this->normal());

    pGeCurve = new GeCircArc3d(this->center(), this->normal(), this->radius());
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::setFromGeCurve(const GeCurve3d &geCurve, GeVector3d *normal, const GeTol &tol)
{
    return Acad::ErrorStatus::eFail;
}
