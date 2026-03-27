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
#include "DbExtents.h"
#include "DbImpl.h"
#include <cmath>

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
	// ODA: 圆的包围盒需考虑任意法线方向
	GePoint3d cen = this->center();
	double r = this->radius();
	GeVector3d n = this->normal();

	// 计算各轴向上的半径投影
	double rx = r * sqrt(1.0 - n.x * n.x);
	double ry = r * sqrt(1.0 - n.y * n.y);
	double rz = r * sqrt(1.0 - n.z * n.z);

	extents.addPoint(GePoint3d(cen.x - rx, cen.y - ry, cen.z - rz));
	extents.addPoint(GePoint3d(cen.x + rx, cen.y + ry, cen.z + rz));
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
Acad::ErrorStatus DbCircle::getParamAtPoint(const GePoint3d &point, double &param) const
{
	GeMatrix3d mat;
	mat.setToPlaneToWorld(this->normal());
	GeVector3d refVec = GeVector3d::kXAxis;
	refVec.transformBy(mat);
	refVec.normalize();

	GeVector3d v = point - this->center();
	param = refVec.angleToCCW(v, this->normal());
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getDistAtParam(double param, double &dist) const
{
	// 距离 = 弧长 = radius * angle
	dist = this->radius() * param;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getParamAtDist(double dist, double &param) const
{
	double r = this->radius();
	if (r < 1e-10) return Acad::ErrorStatus::eFail;
	param = dist / r;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getDistAtPoint(const GePoint3d &point, double &dist) const
{
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getDistAtParam(param, dist);
}
Acad::ErrorStatus DbCircle::getPointAtDist(double dist, GePoint3d &point) const
{
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtDist(dist, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getPointAtParam(param, point);
}
Acad::ErrorStatus DbCircle::getFirstDeriv(double param, GeVector3d &firstDeriv) const
{
	// 一阶导数: 圆在param处的切线方向 * radius
	GeMatrix3d mat;
	mat.setToPlaneToWorld(this->normal());
	GeVector3d refVec = GeVector3d::kXAxis;
	refVec.transformBy(mat);
	refVec.normalize();

	// 切线 = d/dt(center + r*cos(t)*refVec + r*sin(t)*perpVec)
	GeVector3d perpVec = this->normal().crossProduct(refVec);
	perpVec.normalize();
	double r = this->radius();
	firstDeriv = refVec * (-r * sin(param)) + perpVec * (r * cos(param));
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getFirstDeriv(const GePoint3d &point, GeVector3d &firstDeriv) const
{
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getFirstDeriv(param, firstDeriv);
}
Acad::ErrorStatus DbCircle::getSecondDeriv(double param, GeVector3d &secDeriv) const
{
	// 二阶导数: 指向圆心的法线方向 * radius
	GeMatrix3d mat;
	mat.setToPlaneToWorld(this->normal());
	GeVector3d refVec = GeVector3d::kXAxis;
	refVec.transformBy(mat);
	refVec.normalize();

	GeVector3d perpVec = this->normal().crossProduct(refVec);
	perpVec.normalize();
	double r = this->radius();
	secDeriv = refVec * (-r * cos(param)) + perpVec * (-r * sin(param));
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getSecondDeriv(const GePoint3d &point, GeVector3d &secDeriv) const
{
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getSecondDeriv(param, secDeriv);
}
Acad::ErrorStatus DbCircle::getClosestPointTo(const GePoint3d &givenPnt, GePoint3d &pointOnCurve, bool extend) const
{
	GeVector3d v = givenPnt - this->center();
	double len = v.length();
	if (len < 1e-10)
	{
		// 点在圆心, 返回参数0处的点
		return this->getPointAtParam(0.0, pointOnCurve);
	}
	v.normalize();
	pointOnCurve = this->center() + v * this->radius();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getClosestPointTo(const GePoint3d &givenPnt, const GeVector3d &normal, GePoint3d &pointOnCurve, bool extend) const
{
	// 将点投影到圆所在平面后求最近点
	GePoint3d projPnt = givenPnt.project(GePlane(this->center(), this->normal()), normal);
	return this->getClosestPointTo(projPnt, pointOnCurve, extend);
}
Acad::ErrorStatus DbCircle::getOrthoProjectedCurve(const GePlane &, DbCurve *&) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getProjectedCurve(const GePlane &, const GeVector3d &, DbCurve *&) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getOffsetCurves(double offsetDist, DbVoidPtrArray &curves) const
{
	double r = this->radius() + offsetDist;
	if (r < 1e-10)
		return Acad::ErrorStatus::eFail;
	DbCircle *pCircle = new DbCircle();
	pCircle->setCenter(this->center());
	pCircle->setRadius(r);
	pCircle->setNormal(this->normal());
	curves.append(pCircle);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getSpline(DbSpline *&) const
{
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbCircle::getSplitCurves(const GeDoubleArray &params, DbVoidPtrArray &curveSegments) const
{
	if (params.length() == 0)
		return Acad::ErrorStatus::eInvalidInput;

	GeDoubleArray sortedParams;
	for (int i = 0; i < params.length(); i++)
		sortedParams.append(params[i]);
	for (int i = 0; i < sortedParams.length() - 1; i++)
		for (int j = i + 1; j < sortedParams.length(); j++)
			if (sortedParams[i] > sortedParams[j])
			{
				double t = sortedParams[i];
				sortedParams[i] = sortedParams[j];
				sortedParams[j] = t;
			}

	for (int i = 0; i < sortedParams.length(); i++)
	{
		double sa = sortedParams[i];
		double ea = (i + 1 < sortedParams.length()) ? sortedParams[i + 1] : sortedParams[0] + 2.0 * PI;
		DbArc *arc = new DbArc();
		arc->setCenter(this->center());
		arc->setRadius(this->radius());
		arc->setNormal(this->normal());
		arc->setStartAngle(sa);
		arc->setEndAngle(ea);
		curveSegments.append(arc);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCircle::getSplitCurves(const GePoint3dArray &points, DbVoidPtrArray &curveSegments) const
{
	if (points.length() == 0)
		return Acad::ErrorStatus::eInvalidInput;
	GeDoubleArray params;
	for (int i = 0; i < points.length(); i++)
	{
		double p;
		this->getParamAtPoint(points[i], p);
		params.append(p);
	}
	return this->getSplitCurves(params, curveSegments);
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
