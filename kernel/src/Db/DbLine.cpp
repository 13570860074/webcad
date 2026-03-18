#include "DbLine.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbGripDataPtrArray.h"
#include "GePlane.h"
#include "DbDwgFiler.h"
#include "DbRay.h"
#include "DbXline.h"
#include "DbArc.h"
#include "DbCircle.h"
#include "DbExtents.h"
#include "GeLineSeg2d.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GeLine3d.h"
#include "GeCircArc3d.h"
#include "GiEntityTraits.h"
#include "DbOsnapPointCompute.h"
#include "DbImpl.h"

DbLine::DbLine()
{
	this->m_pImpl = new DbLineImpl();
}
DbLine::DbLine(const GePoint3d &start, const GePoint3d &end)
{
	this->m_pImpl = new DbLineImpl();

}
DbLine::~DbLine()
{

}

Acad::ErrorStatus DbLine::getOffsetCurvesGivenPlaneNormal(
	const GeVector3d &normal,
	double offsetDist,
	DbVoidPtrArray &offsetCurves) const
{
	GeLineSeg3d line(this->startPoint(), this->endPoint());
	GeVoidPointerArray entitys;
	line.getTrimmedOffset(offsetDist, normal, entitys);
	for (int i = entitys.length() - 1; i >= 0; i--)
	{
		GeEntity3d *entity = (GeEntity3d *)entitys[i];
		if (entity->isKindOf(Ge::EntityId::kLineSeg3d) == true)
		{
			DbLine *line = DbLine::cast(this->clone());
			line->setStartPoint(((GeLineSeg3d *)entity)->startPoint());
			line->setEndPoint(((GeLineSeg3d *)entity)->endPoint());
			offsetCurves.append(line);
		}
		delete entity;
	}
	return Acad::ErrorStatus::eOk;
}

GePoint3d DbLine::startPoint() const
{
	return DB_IMP_LINE(this->m_pImpl)->startPoint;
}
Acad::ErrorStatus DbLine::setStartPoint(const GePoint3d &_startPoint)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		DB_IMP_LINE(this->m_pImpl)->startPoint = _startPoint;
		es = Acad::ErrorStatus::eOk;

	} while (false);

	return es;
}

GePoint3d DbLine::endPoint() const
{
	return DB_IMP_LINE(this->m_pImpl)->endPoint;
}
Acad::ErrorStatus DbLine::setEndPoint(const GePoint3d &_endPoint)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		DB_IMP_LINE(this->m_pImpl)->endPoint = _endPoint;
		es = Acad::ErrorStatus::eOk;

	} while (false);

	return es;
}

double DbLine::thickness() const
{
	return DB_IMP_LINE(this->m_pImpl)->thickness;
}
Acad::ErrorStatus DbLine::setThickness(double _thickness)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		DB_IMP_LINE(this->m_pImpl)->thickness = _thickness;
		es = Acad::ErrorStatus::eOk;

	} while (false);

	return es;
}

Acad::ErrorStatus DbLine::dwgInFields(DbDwgFiler *pFiler)
{

	Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	GePoint3d startPoint;
	pFiler->readPoint3d(&startPoint);
	this->setStartPoint(startPoint);

	GePoint3d endPoint;
	pFiler->readPoint3d(&endPoint);
	this->setEndPoint(endPoint);

	double thickness;
	pFiler->readDouble(&thickness);
	this->setThickness(thickness);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::dwgOutFields(DbDwgFiler *pFiler) const
{

	Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	pFiler->writePoint3d(this->startPoint());

	pFiler->writePoint3d(this->endPoint());

	pFiler->writeDouble(this->thickness());

	return Acad::ErrorStatus::eOk;
}
bool DbLine::subWorldDraw(GiWorldDraw *pWd) const
{
	return pWd->geometry().line(this->startPoint(), this->endPoint());
}

Acad::ErrorStatus DbLine::subGetGeomExtents(DbExtents& extents) const{
	GePoint3d minPoint;
	GePoint3d maxPoint;
	minPoint.x = this->startPoint().x < this->endPoint().x ? this->startPoint().x : this->endPoint().x;
	minPoint.y = this->startPoint().y < this->endPoint().y ? this->startPoint().y : this->endPoint().y;
	minPoint.z = this->startPoint().z < this->endPoint().z ? this->startPoint().z : this->endPoint().z;
	maxPoint.x = this->startPoint().x > this->endPoint().x ? this->startPoint().x : this->endPoint().x;
	maxPoint.y = this->startPoint().y > this->endPoint().y ? this->startPoint().y : this->endPoint().y;
	maxPoint.z = this->startPoint().z > this->endPoint().z ? this->startPoint().z : this->endPoint().z;
	extents.set(minPoint, maxPoint);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::subTransformBy(const GeMatrix3d &xform)
{
	DB_IMP_LINE(this->m_pImpl)->startPoint.transformBy(xform);
	DB_IMP_LINE(this->m_pImpl)->endPoint.transformBy(xform);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::subGetGripPoints(
	DbGripDataPtrArray &grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d &curViewDir,
	const int bitflags) const
{

	DbGripData *gripData = new DbGripData();
	gripData->setGripPoint(this->startPoint());
	grips.append(gripData);

	gripData = new DbGripData();
	GePoint3d position = this->startPoint();
	position.x += (this->endPoint().x - this->startPoint().x) / 2.0;
	position.y += (this->endPoint().y - this->startPoint().y) / 2.0;
	position.z += (this->endPoint().z - this->startPoint().z) / 2.0;
	gripData->setGripPoint(position);
	grips.append(gripData);

	gripData = new DbGripData();
	gripData->setGripPoint(this->endPoint());
	grips.append(gripData);

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbLine::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d &pickPoint,
	const GePoint3d &lastPoint,
	const GeMatrix3d &viewXform,
	GePoint3dArray &snapPoints,
	DbIntArray &geomIds) const
{

	if (osnapMode == Db::OsnapMode::kOsModeNear)
	{
		GePoint3dArray points = ::osnap_near_to_lintseg(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeMid)
	{
		GePoint3dArray points = ::osnap_mid_to_lintseg(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeEnd)
	{
		GePoint3dArray points = ::osnap_end_to_lintseg(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModePerp)
	{
		GePoint3dArray points = ::osnap_perp_to_lintseg(pickPoint, lastPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::subMoveGripPointsAt(const DbIntArray &indices, const GeVector3d &offset)
{

	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) == 0)
		{
			DB_IMP_LINE(this->m_pImpl)->startPoint += offset;
		}
		else if (indices.at(i) == 1)
		{
			DB_IMP_LINE(this->m_pImpl)->startPoint += offset;
			DB_IMP_LINE(this->m_pImpl)->endPoint += offset;
		}
		else if (indices.at(i) == 2)
		{
			DB_IMP_LINE(this->m_pImpl)->endPoint += offset;
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::subMoveGripPointsAt(const DbVoidPtrArray &gripAppData, const GeVector3d &offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::subIntersectWith(const DbEntity *pEnt, Db::Intersect intType, GePoint3dArray &points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const
{

	if (pEnt == NULL)
	{
		return Acad::ErrorStatus::eFail;
	}

	if (DB_IMP_ENTITY(pEnt->m_pImpl)->isLine())
	{
		GePoint3dArray ps = ::osnap_inters_lintseg_to_lintseg(this, (DbLine *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isXline())
	{
		GePoint3dArray ps = ::osnap_inters_lintseg_to_line(this, (DbXline *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isRay())
	{
		GePoint3dArray ps = ::osnap_inters_lintseg_to_ray(this, (DbRay *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isCircle())
	{
		GePoint3dArray ps = ::osnap_inters_lintseg_to_circle(this, (DbCircle *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isArc())
	{
		GePoint3dArray ps = ::osnap_inters_lintseg_to_arc(this, (DbArc *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isPolyline())
	{
		GePoint3dArray ps = ::osnap_inters_lintseg_to_polyline(this, (DbPolyline *)pEnt, intType);
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

bool DbLine::isClosed() const
{
	return false;
}
bool DbLine::isPeriodic() const
{
	return false;
}
bool DbLine::isPlanar() const
{
	return false;
}
Acad::ErrorStatus DbLine::getPlane(GePlane &plane, Db::Planarity &planarity) const
{
	plane = GePlane(this->startPoint(), (this->endPoint() - this->startPoint()).normal());
	planarity = Db::Planarity::kLinear;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getStartParam(double &_v) const
{
	_v = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getEndParam(double &_v) const
{
	GeLineSeg3d line(this->startPoint(), this->endPoint());
	_v = line.length();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getStartPoint(GePoint3d &_v) const
{
	_v = this->startPoint();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getEndPoint(GePoint3d &_v) const
{
	_v = this->endPoint();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getPointAtParam(double param, GePoint3d &point) const
{
	GeLineSeg3d line(this->startPoint(), this->endPoint());
	point = line.evalPoint(param);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getParamAtPoint(const GePoint3d &point, double &param) const
{
	GeLineSeg3d line(this->startPoint(), this->endPoint());
	param = line.paramOf(point);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getDistAtParam(double param, double &dist) const
{
	dist = abs(param);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getParamAtDist(double dist, double &param) const
{
	param = dist;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getDistAtPoint(const GePoint3d &point, double &dist) const
{
	GeLineSeg3d line(this->startPoint(), this->endPoint());
	dist = line.distanceTo(point);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getPointAtDist(double dist, GePoint3d &point) const
{
	GeLineSeg3d line(this->startPoint(), this->endPoint());
	point = line.evalPoint(dist);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getFirstDeriv(double, GeVector3d &) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::getFirstDeriv(const GePoint3d &, GeVector3d &) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::getSecondDeriv(double, GeVector3d &) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::getSecondDeriv(const GePoint3d &, GeVector3d &) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::getClosestPointTo(const GePoint3d &point, GePoint3d &closest, bool) const
{
	GeLineSeg3d line(this->startPoint(), this->endPoint());
	closest = line.closestPointTo(point);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getClosestPointTo(const GePoint3d &point, const GeVector3d &projectDirection, GePoint3d &closest, bool) const
{
	GeLineSeg3d line(this->startPoint(), this->endPoint());
	closest = line.projClosestPointTo(point, projectDirection);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getOrthoProjectedCurve(const GePlane &plane, DbCurve *&curve) const
{
	return this->getProjectedCurve(plane, plane.normal(), curve);
}
Acad::ErrorStatus DbLine::getProjectedCurve(const GePlane &plane, const GeVector3d &normal, DbCurve *&curve) const
{
	GePoint3d startPoint = this->startPoint();
	startPoint = startPoint.project(plane, normal);

	GePoint3d endPoint = this->endPoint();
	endPoint = endPoint.project(plane, normal);

	curve = new DbLine();
	((DbLine *)curve)->setStartPoint(startPoint);
	((DbLine *)curve)->setEndPoint(endPoint);
	((DbLine *)curve)->setLayer(this->layerId());
	((DbLine *)curve)->setColorIndex(this->colorIndex());
	((DbLine *)curve)->setThickness(this->thickness());
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::getOffsetCurves(double v, DbVoidPtrArray &curves) const
{

	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::getSpline(DbSpline *&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::getSplitCurves(const GeDoubleArray &, DbVoidPtrArray &curveSegments) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::getSplitCurves(const GePoint3dArray &, DbVoidPtrArray &) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::extend(double)
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::extend(bool, const GePoint3d &)
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::getArea(double &area) const
{
	area = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::reverseCurve()
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLine::getGeCurve(GeCurve3d *&pGeCurve, const GeTol &tol) const
{
	pGeCurve = new GeLineSeg3d(this->startPoint(), this->endPoint());
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLine::setFromGeCurve(const GeCurve3d &geCurve, GeVector3d *normal, const GeTol &tol)
{
	return Acad::ErrorStatus::eFail;
}
