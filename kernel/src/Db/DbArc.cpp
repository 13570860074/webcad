#include "DbArc.h"
#include "GeDoubleArray.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbDwgFiler.h"
#include "GeCircArc2d.h"
#include "GePlane.h"
#include "DbLine.h"
#include "DbRay.h"
#include "DbXline.h"
#include "DbCircle.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GeLine3d.h"
#include "GeCircArc3d.h"
#include "DbOsnapPointCompute.h"
#include "DbExtents.h"
#include "DbImpl.h"
#include <cmath>

DbArc::DbArc()
{
	this->m_pImpl = new DbArcImpl();
}
DbArc::DbArc(const GePoint3d &center, double radius, double startAngle, double endAngle)
{
	this->m_pImpl = new DbArcImpl();
	this->setCenter(center);
	this->setRadius(radius);
	this->setStartAngle(startAngle);
	this->setEndAngle(endAngle);
}
DbArc::DbArc(const GePoint3d &center, const GeVector3d &normal, double radius, double startAngle, double endAngle)
{
	this->m_pImpl = new DbArcImpl();
	this->setCenter(center);
	this->setRadius(radius);
	this->setNormal(normal);
	this->setStartAngle(startAngle);
	this->setEndAngle(endAngle);
}
DbArc::~DbArc()
{
}

GePoint3d DbArc::center() const
{
	return DB_IMP_ARC(this->m_pImpl)->center;
}
Acad::ErrorStatus DbArc::setCenter(const GePoint3d &_center)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_ARC(this->m_pImpl)->center = _center;
	}
	return Acad::ErrorStatus::eOk;
}

double DbArc::radius() const
{
	return DB_IMP_ARC(this->m_pImpl)->radius;
}
Acad::ErrorStatus DbArc::setRadius(double _radius)
{
	if (_radius < GeContext::gTol.equalPoint())
	{
		return Acad::ErrorStatus::eFail;
	}
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_ARC(this->m_pImpl)->radius = _radius;
	}
	return Acad::ErrorStatus::eOk;
}
double DbArc::startAngle() const
{
	return DB_IMP_ARC(this->m_pImpl)->startAngle;
}
Acad::ErrorStatus DbArc::setStartAngle(double _val)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_ARC(this->m_pImpl)->startAngle = _val;
	}
	return Acad::ErrorStatus::eOk;
}

double DbArc::endAngle() const
{
	return DB_IMP_ARC(this->m_pImpl)->endAngle;
}
Acad::ErrorStatus DbArc::setEndAngle(double _val)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_ARC(this->m_pImpl)->endAngle = _val;
	}
	return Acad::ErrorStatus::eOk;
}

double DbArc::totalAngle() const
{
	double startAng = this->startAngle();
	double endAng = this->startAngle();
	if (endAng < startAng)
	{
		endAng += PI * 2;
	}
	return endAng - startAng;
}

double DbArc::length() const
{
	return this->radius() * this->totalAngle();
}
double DbArc::thickness() const
{
	return DB_IMP_ARC(this->m_pImpl)->thickness;
}
Acad::ErrorStatus DbArc::setThickness(double _thickness)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_ARC(this->m_pImpl)->thickness = _thickness;
	}
	return Acad::ErrorStatus::eOk;
}

GeVector3d DbArc::normal() const
{
	return DB_IMP_ARC(this->m_pImpl)->normal;
}
Acad::ErrorStatus DbArc::setNormal(const GeVector3d &_normal)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_ARC(this->m_pImpl)->normal = _normal;
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::dwgInFields(DbDwgFiler *pFiler)
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

	double startAngle;
	pFiler->readDouble(&startAngle);
	this->setStartAngle(startAngle);

	double endAngle;
	pFiler->readDouble(&endAngle);
	this->setEndAngle(endAngle);

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	this->setNormal(normal);

	double thickness;
	pFiler->readDouble(&thickness);
	this->setThickness(thickness);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::dwgOutFields(DbDwgFiler *pFiler) const
{

	Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	pFiler->writePoint3d(this->center());

	pFiler->writeDouble(this->radius());

	pFiler->writeDouble(this->startAngle());

	pFiler->writeDouble(this->endAngle());

	pFiler->writeVector3d(this->normal());

	pFiler->writeDouble(this->thickness());

	return Acad::ErrorStatus::eOk;
}
bool DbArc::subWorldDraw(GiWorldDraw *pWd) const
{
	bool isError = true;

	// 获得开始法向
	GeVector3d startVector;
	if (this->normal().isEqualTo(GeVector3d::kZAxis) == true)
	{
		startVector = GeVector3d::kXAxis;
	}
	else if (this->normal().isEqualTo(GeVector3d(0, 0, -1)) == true)
	{
		startVector = GeVector3d(-1, 0, 0);
	}
	else
	{
		startVector = GeVector3d::kZAxis.crossProduct(this->normal());
	}
	startVector.rotateBy(this->startAngle(), this->normal());

	// 获得起始和结束弧度
	double startAng = this->startAngle();
	double endAng = this->endAngle();
	if (startAng > endAng)
	{
		endAng += PI * 2;
	}

	if (endAng - startAng > 0.001)
	{
		isError = pWd->geometry().circularArc(this->center(), this->radius(), this->normal(), startVector, endAng - startAng);
	}
	return isError;
}

bool DbArc::subOpen(Db::OpenMode mode)
{
	DB_IMP_ARC(this->m_pImpl)->bInitPoint = false;
	return DbCurve::subOpen(mode);
}


Acad::ErrorStatus DbArc::subGetGeomExtents(DbExtents &extents) const
{
	// 采样弧上的点 + 检查轴向极值点
	GePoint3d pt;
	this->getStartPoint(pt);
	extents.addPoint(pt);
	this->getEndPoint(pt);
	extents.addPoint(pt);

	GeMatrix3d mat;
	mat.setToPlaneToWorld(this->normal());
	GeVector3d refVec = GeVector3d::kXAxis;
	refVec.transformBy(mat);
	refVec.normalize();

	double sa = this->startAngle();
	double ea = this->endAngle();
	// 检查0, PI/2, PI, 3PI/2这四个轴向极值角是否在弧上
	for (int i = 0; i < 4; i++)
	{
		double angle = i * PI / 2.0;
		bool inArc = false;
		if (ea > sa)
			inArc = (angle >= sa && angle <= ea);
		else
			inArc = (angle >= sa || angle <= ea);
		if (inArc)
		{
			this->getPointAtParam(angle, pt);
			extents.addPoint(pt);
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::subTransformBy(const GeMatrix3d &xform)
{
	// 求开始点
	GePoint3d startPoint;
	this->getStartPoint(startPoint);
	startPoint.transformBy(xform);

	// 求中点
	GePoint3d midPoint;
	this->getPointAtParam(this->startAngle() + (this->endAngle() - this->startAngle()) / 2.0, midPoint);
	midPoint.transformBy(xform);

	// 求结束点
	GePoint3d endPoint;
	this->getEndPoint(endPoint);
	endPoint.transformBy(xform);

	GeCircArc3d circArc = GeCircArc3d(startPoint, midPoint, endPoint);
	this->setCenter(circArc.center());
	this->setRadius(circArc.radius());
	this->setNormal(circArc.normal());
	this->setStartAngle(circArc.startAng());
	this->setEndAngle(circArc.endAng());
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::subGetGripPoints(
	DbGripDataPtrArray &grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d &curViewDir,
	const int bitflags) const
{
	DbGripData *gripData = new DbGripData();
	gripData->setGripPoint(this->center());
	grips.append(gripData);

	GePoint3d startPoint;
	this->getStartPoint(startPoint);
	gripData = new DbGripData();
	gripData->setGripPoint(startPoint);
	grips.append(gripData);

	GePoint3d midPoint;
	if (this->endAngle() > this->startAngle())
	{
		this->getPointAtParam(this->startAngle() + (this->endAngle() - this->startAngle()) / 2.0, midPoint);
	}
	else
	{
		this->getPointAtParam(this->startAngle() + (this->endAngle() + PI * 2 - this->startAngle()) / 2.0, midPoint);
	}
	gripData = new DbGripData();
	gripData->setGripPoint(midPoint);
	grips.append(gripData);

	GePoint3d endPoint;
	this->getEndPoint(endPoint);
	gripData = new DbGripData();
	gripData->setGripPoint(endPoint);
	grips.append(gripData);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::subGetOsnapPoints(
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
		GePoint3dArray points = ::osnap_near_to_arc(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeCen)
	{
		GePoint3dArray points = ::osnap_center_to_arc(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeEnd)
	{
		GePoint3dArray points = ::osnap_end_to_arc(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeQuad)
	{
		GePoint3dArray points = ::osnap_quad_to_arc(pickPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModePerp)
	{
		GePoint3dArray points = ::osnap_perp_to_arc(pickPoint, lastPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeTan)
	{
		GePoint3dArray points = ::osnap_tan_to_arc(pickPoint, lastPoint, this, viewXform);
		for (int i = 0; i < points.length(); i++)
		{
			snapPoints.append(points[i]);
		}
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::subMoveGripPointsAt(const DbIntArray &indices, const GeVector3d &offset)
{
	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) == 0)
		{
			DB_IMP_ARC(this->m_pImpl)->center += offset;
		}
		else if (indices.at(i) == 1)
		{
			if (DB_IMP_ARC(this->m_pImpl)->bInitPoint == false)
			{
				DB_IMP_ARC(this->m_pImpl)->bInitPoint = true;

				double startAngle = this->startAngle();
				double endAngle = this->endAngle();
				this->getPointAtParam(this->startAngle(), DB_IMP_ARC(this->m_pImpl)->initVertex1);
				if (endAngle > startAngle)
				{
					this->getPointAtParam(startAngle + (endAngle - startAngle) / 2.0, DB_IMP_ARC(this->m_pImpl)->initVertex2);
				}
				else
				{
					this->getPointAtParam(startAngle + (endAngle + PI * 2 - startAngle) / 2.0, DB_IMP_ARC(this->m_pImpl)->initVertex2);
				}
				this->getPointAtParam(this->endAngle(), DB_IMP_ARC(this->m_pImpl)->initVertex3);

				DB_IMP_ARC(this->m_pImpl)->bClockWise = GeCircArc3d::isClockWise(
					DB_IMP_ARC(this->m_pImpl)->initVertex1,
					DB_IMP_ARC(this->m_pImpl)->initVertex2,
					DB_IMP_ARC(this->m_pImpl)->initVertex3,
					this->normal());
			}

			GePoint3d curPoint = DB_IMP_ARC(this->m_pImpl)->initVertex1 + offset;
			curPoint = curPoint.project(GePlane(DB_IMP_ARC(this->m_pImpl)->initVertex2, this->normal()), GeVector3d::kZAxis);

			GePoint3d startPoint = curPoint;
			GePoint3d midPoint = DB_IMP_ARC(this->m_pImpl)->initVertex2;
			GePoint3d endPoint = DB_IMP_ARC(this->m_pImpl)->initVertex3;
			if (curPoint.isEqualTo(midPoint) || curPoint.isEqualTo(endPoint))
			{
				continue;
			}

			GePoint3d center;
			if (GeCircArc3d::pointToCenter(startPoint, midPoint, endPoint, center) == true)
			{

				this->setCenter(center);
				this->setRadius(startPoint.distanceTo(center));

				GeMatrix3d mat;
				mat.setToPlaneToWorld(this->normal());
				GeVector3d refVec = GeVector3d::kXAxis;
				refVec.transformBy(mat);
				refVec.normalize();

				double startAng = refVec.angleToCCW((startPoint - center).normal(), this->normal());
				double endAng = refVec.angleToCCW((endPoint - center).normal(), this->normal());
				bool isClockWise = GeCircArc3d::isClockWise(startPoint, midPoint, endPoint, this->normal());
				if (isClockWise == DB_IMP_ARC(this->m_pImpl)->bClockWise)
				{
					this->setStartAngle(startAng);
					this->setEndAngle(endAng);
				}
				else
				{
					this->setStartAngle(endAng);
					this->setEndAngle(startAng);
				}

				DB_IMP_ARC(this->m_pImpl)->initVertex1 += offset;
			}
		}
		else if (indices.at(i) == 2)
		{
			if (DB_IMP_ARC(this->m_pImpl)->bInitPoint == false)
			{
				DB_IMP_ARC(this->m_pImpl)->bInitPoint = true;

				double startAngle = this->startAngle();
				double endAngle = this->endAngle();
				this->getPointAtParam(this->startAngle(), DB_IMP_ARC(this->m_pImpl)->initVertex1);
				if (endAngle > startAngle)
				{
					this->getPointAtParam(startAngle + (endAngle - startAngle) / 2.0, DB_IMP_ARC(this->m_pImpl)->initVertex2);
				}
				else
				{
					this->getPointAtParam(startAngle + (endAngle + PI * 2 - startAngle) / 2.0, DB_IMP_ARC(this->m_pImpl)->initVertex2);
				}
				this->getPointAtParam(this->endAngle(), DB_IMP_ARC(this->m_pImpl)->initVertex3);

				DB_IMP_ARC(this->m_pImpl)->bClockWise = GeCircArc3d::isClockWise(
					DB_IMP_ARC(this->m_pImpl)->initVertex1,
					DB_IMP_ARC(this->m_pImpl)->initVertex2,
					DB_IMP_ARC(this->m_pImpl)->initVertex3,
					this->normal());
			}

			GePoint3d curPoint = DB_IMP_ARC(this->m_pImpl)->initVertex2 + offset;
			curPoint = curPoint.project(GePlane(DB_IMP_ARC(this->m_pImpl)->initVertex1, this->normal()), GeVector3d::kZAxis);

			GePoint3d startPoint = DB_IMP_ARC(this->m_pImpl)->initVertex1;
			GePoint3d midPoint = curPoint;
			GePoint3d endPoint = DB_IMP_ARC(this->m_pImpl)->initVertex3;
			if (curPoint.isEqualTo(startPoint) || curPoint.isEqualTo(endPoint))
			{
				continue;
			}

			GePoint3d center;
			if (GeCircArc3d::pointToCenter(startPoint, midPoint, endPoint, center) == true)
			{

				this->setCenter(center);
				this->setRadius(startPoint.distanceTo(center));

				GeMatrix3d mat;
				mat.setToPlaneToWorld(this->normal());
				GeVector3d refVec = GeVector3d::kXAxis;
				refVec.transformBy(mat);
				refVec.normalize();

				double startAng = refVec.angleToCCW((startPoint - center).normal(), this->normal());
				double endAng = refVec.angleToCCW((endPoint - center).normal(), this->normal());
				bool isClockWise = GeCircArc3d::isClockWise(startPoint, midPoint, endPoint, this->normal());
				if (isClockWise == DB_IMP_ARC(this->m_pImpl)->bClockWise)
				{
					this->setStartAngle(startAng);
					this->setEndAngle(endAng);
				}
				else
				{
					this->setStartAngle(endAng);
					this->setEndAngle(startAng + 2 * PI);
				}

				DB_IMP_ARC(this->m_pImpl)->initVertex2 += offset;
			}
		}
		else if (indices.at(i) == 3)
		{
			if (DB_IMP_ARC(this->m_pImpl)->bInitPoint == false)
			{
				DB_IMP_ARC(this->m_pImpl)->bInitPoint = true;

				double startAngle = this->startAngle();
				double endAngle = this->endAngle();
				this->getPointAtParam(this->startAngle(), DB_IMP_ARC(this->m_pImpl)->initVertex1);
				if (endAngle > startAngle)
				{
					this->getPointAtParam(startAngle + (endAngle - startAngle) / 2.0, DB_IMP_ARC(this->m_pImpl)->initVertex2);
				}
				else
				{
					this->getPointAtParam(startAngle + (endAngle + PI * 2 - startAngle) / 2.0, DB_IMP_ARC(this->m_pImpl)->initVertex2);
				}
				this->getPointAtParam(this->endAngle(), DB_IMP_ARC(this->m_pImpl)->initVertex3);

				DB_IMP_ARC(this->m_pImpl)->bClockWise = GeCircArc3d::isClockWise(
					DB_IMP_ARC(this->m_pImpl)->initVertex1,
					DB_IMP_ARC(this->m_pImpl)->initVertex2,
					DB_IMP_ARC(this->m_pImpl)->initVertex3,
					this->normal());
			}

			GePoint3d curPoint = DB_IMP_ARC(this->m_pImpl)->initVertex3 + offset;
			curPoint = curPoint.project(GePlane(DB_IMP_ARC(this->m_pImpl)->initVertex2, this->normal()), GeVector3d::kZAxis);

			GePoint3d startPoint = DB_IMP_ARC(this->m_pImpl)->initVertex1;
			GePoint3d midPoint = DB_IMP_ARC(this->m_pImpl)->initVertex2;
			GePoint3d endPoint = curPoint;
			if (curPoint.isEqualTo(startPoint) || curPoint.isEqualTo(midPoint))
			{
				continue;
			}

			GePoint3d center;
			if (GeCircArc3d::pointToCenter(startPoint, midPoint, endPoint, center) == true)
			{

				this->setCenter(center);
				this->setRadius(startPoint.distanceTo(center));

				GeMatrix3d mat;
				mat.setToPlaneToWorld(this->normal());
				GeVector3d refVec = GeVector3d::kXAxis;
				refVec.transformBy(mat);
				refVec.normalize();

				double startAng = refVec.angleToCCW((startPoint - center).normal(), this->normal());
				double endAng = refVec.angleToCCW((endPoint - center).normal(), this->normal());
				bool isClockWise = GeCircArc3d::isClockWise(startPoint, midPoint, endPoint, this->normal());
				if (isClockWise == DB_IMP_ARC(this->m_pImpl)->bClockWise)
				{
					this->setStartAngle(startAng);
					this->setEndAngle(endAng);
				}
				else
				{
					this->setStartAngle(endAng);
					this->setEndAngle(startAng);
				}

				DB_IMP_ARC(this->m_pImpl)->initVertex3 += offset;
			}
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::subMoveGripPointsAt(const DbVoidPtrArray &gripAppData, const GeVector3d &offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::subIntersectWith(const DbEntity *pEnt, Db::Intersect intType, GePoint3dArray &points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const
{

	if (pEnt == NULL)
	{
		return Acad::ErrorStatus::eFail;
	}

	if (DB_IMP_ENTITY(pEnt->m_pImpl)->isLine())
	{
		GePoint3dArray ps = ::osnap_inters_arc_to_lintseg(this, (DbLine *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isXline())
	{
		GePoint3dArray ps = ::osnap_inters_arc_to_line(this, (DbXline *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isRay())
	{
		GePoint3dArray ps = ::osnap_inters_arc_to_ray(this, (DbRay *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isCircle())
	{
		GePoint3dArray ps = ::osnap_inters_arc_to_circle(this, (DbCircle *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isArc())
	{
		GePoint3dArray ps = ::osnap_inters_arc_to_arc(this, (DbArc *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isPolyline())
	{
		GePoint3dArray ps = ::osnap_inters_arc_to_polyline(this, (DbPolyline *)pEnt, intType);
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

bool DbArc::isClosed() const
{
	return false;
}
bool DbArc::isPeriodic() const
{
	return false;
}
bool DbArc::isPlanar() const
{
	return true;
}
Acad::ErrorStatus DbArc::getPlane(GePlane &plane, Db::Planarity &planarity) const
{
	plane.set(this->center(), this->normal());
	planarity = Db::Planarity::kPlanar;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getStartParam(double &_v) const
{
	_v = this->startAngle();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getEndParam(double &_v) const
{
	_v = this->endAngle();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getStartPoint(GePoint3d &point) const
{
	return this->getPointAtParam(this->startAngle(), point);
}
Acad::ErrorStatus DbArc::getEndPoint(GePoint3d &point) const
{
	return this->getPointAtParam(this->endAngle(), point);
}
Acad::ErrorStatus DbArc::getPointAtParam(double param, GePoint3d &point) const
{
	GeMatrix3d mat;
	mat.setToPlaneToWorld(this->normal());
	GeVector3d refVec = GeVector3d::kXAxis;
	refVec.transformBy(mat);
	refVec.normalize();

	point = this->center() + refVec * this->radius();
	point.rotateBy(param, this->normal(), this->center());
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getParamAtPoint(const GePoint3d &point, double &param) const
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
Acad::ErrorStatus DbArc::getDistAtParam(double param, double &dist) const
{
	double sa = this->startAngle();
	double angle = param - sa;
	if (angle < 0) angle += 2.0 * PI;
	dist = this->radius() * angle;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getParamAtDist(double dist, double &param) const
{
	double r = this->radius();
	if (r < 1e-10) return Acad::ErrorStatus::eFail;
	double angle = dist / r;
	param = this->startAngle() + angle;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getDistAtPoint(const GePoint3d &point, double &dist) const
{
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getDistAtParam(param, dist);
}
Acad::ErrorStatus DbArc::getPointAtDist(double dist, GePoint3d &point) const
{
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtDist(dist, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getPointAtParam(param, point);
}
Acad::ErrorStatus DbArc::getFirstDeriv(double param, GeVector3d &firstDeriv) const
{
	GeMatrix3d mat;
	mat.setToPlaneToWorld(this->normal());
	GeVector3d refVec = GeVector3d::kXAxis;
	refVec.transformBy(mat);
	refVec.normalize();

	GeVector3d perpVec = this->normal().crossProduct(refVec);
	perpVec.normalize();
	double r = this->radius();
	firstDeriv = refVec * (-r * sin(param)) + perpVec * (r * cos(param));
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getFirstDeriv(const GePoint3d &point, GeVector3d &firstDeriv) const
{
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getFirstDeriv(param, firstDeriv);
}
Acad::ErrorStatus DbArc::getSecondDeriv(double param, GeVector3d &secDeriv) const
{
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
Acad::ErrorStatus DbArc::getSecondDeriv(const GePoint3d &point, GeVector3d &secDeriv) const
{
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getSecondDeriv(param, secDeriv);
}
Acad::ErrorStatus DbArc::getClosestPointTo(const GePoint3d &givenPnt, GePoint3d &pointOnCurve, bool extend) const
{
	GeVector3d v = givenPnt - this->center();
	double len = v.length();
	if (len < 1e-10)
	{
		return this->getStartPoint(pointOnCurve);
	}
	v.normalize();
	GePoint3d candidate = this->center() + v * this->radius();

	if (!extend)
	{
		// 检查candidate是否在弧上
		double param = 0.0;
		this->getParamAtPoint(candidate, param);
		double sa = this->startAngle();
		double ea = this->endAngle();
		bool onArc = false;
		if (ea > sa)
			onArc = (param >= sa - 1e-10 && param <= ea + 1e-10);
		else
			onArc = (param >= sa - 1e-10 || param <= ea + 1e-10);

		if (!onArc)
		{
			// 返回距离较近的端点
			GePoint3d sp, ep;
			this->getStartPoint(sp);
			this->getEndPoint(ep);
			pointOnCurve = (givenPnt.distanceTo(sp) <= givenPnt.distanceTo(ep)) ? sp : ep;
			return Acad::ErrorStatus::eOk;
		}
	}
	pointOnCurve = candidate;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getClosestPointTo(const GePoint3d &givenPnt, const GeVector3d &normal, GePoint3d &pointOnCurve, bool extend) const
{
	GePoint3d projPnt = givenPnt.project(GePlane(this->center(), this->normal()), normal);
	return this->getClosestPointTo(projPnt, pointOnCurve, extend);
}
Acad::ErrorStatus DbArc::getOrthoProjectedCurve(const GePlane &, DbCurve *&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbArc::getProjectedCurve(const GePlane &, const GeVector3d &, DbCurve *&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbArc::getOffsetCurves(double offsetDist, DbVoidPtrArray &curves) const
{
	double r = this->radius() + offsetDist;
	if (r < 1e-10)
		return Acad::ErrorStatus::eFail;
	DbArc *pArc = new DbArc();
	pArc->setCenter(this->center());
	pArc->setRadius(r);
	pArc->setNormal(this->normal());
	pArc->setStartAngle(this->startAngle());
	pArc->setEndAngle(this->endAngle());
	curves.append(pArc);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getSpline(DbSpline *&) const
{
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbArc::getSplitCurves(const GeDoubleArray &params, DbVoidPtrArray &curveSegments) const
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

	double prevParam = this->startAngle();
	for (int i = 0; i < sortedParams.length(); i++)
	{
		DbArc *arc = new DbArc();
		arc->setCenter(this->center());
		arc->setRadius(this->radius());
		arc->setNormal(this->normal());
		arc->setStartAngle(prevParam);
		arc->setEndAngle(sortedParams[i]);
		curveSegments.append(arc);
		prevParam = sortedParams[i];
	}
	DbArc *arc = new DbArc();
	arc->setCenter(this->center());
	arc->setRadius(this->radius());
	arc->setNormal(this->normal());
	arc->setStartAngle(prevParam);
	arc->setEndAngle(this->endAngle());
	curveSegments.append(arc);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getSplitCurves(const GePoint3dArray &points, DbVoidPtrArray &curveSegments) const
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
Acad::ErrorStatus DbArc::extend(double newParam)
{
	double sa = this->startAngle();
	double ea = this->endAngle();
	if (newParam < sa)
		this->setStartAngle(newParam);
	else if (newParam > ea)
		this->setEndAngle(newParam);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::extend(bool extendStart, const GePoint3d &toPoint)
{
	double param = 0.0;
	this->getParamAtPoint(toPoint, param);
	if (extendStart)
		this->setStartAngle(param);
	else
		this->setEndAngle(param);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getArea(double &area) const
{
	// 弧所围扇形面积 = 0.5 * r^2 * totalAngle
	double ta = this->totalAngle();
	area = 0.5 * this->radius() * this->radius() * ta;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::reverseCurve()
{
	double sa = this->startAngle();
	double ea = this->endAngle();
	this->setStartAngle(ea);
	this->setEndAngle(sa);
	this->setNormal(this->normal().negate());
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::getGeCurve(GeCurve3d *&pGeCurve, const GeTol &tol) const
{
	GePoint3d sp, mp, ep;
	this->getStartPoint(sp);
	double midParam = this->startAngle() + this->totalAngle() * 0.5;
	this->getPointAtParam(midParam, mp);
	this->getEndPoint(ep);
	pGeCurve = new GeCircArc3d(sp, mp, ep);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArc::setFromGeCurve(const GeCurve3d &geCurve, GeVector3d *normal, const GeTol &tol)
{
	return Acad::ErrorStatus::eFail;
}