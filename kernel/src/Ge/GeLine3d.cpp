#include "GeLine3d.h"
#include "GeScale2d.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GeCircArc3d.h"
#include "GePointOnCurve3d.h"
#include "GeEntity2dMethods.h"
#include "GeInterval.h"
#include "GeBoundBlock3d.h"
#include "GePlane.h"
#include "GeMatrix3d.h"
#include "GeImpl.h"

const GeLine3d GeLine3d::kXAxis = GeLine3d(GePoint3d(0, 0, 0), GeVector3d(1, 0, 0));
const GeLine3d GeLine3d::kYAxis = GeLine3d(GePoint3d(0, 0, 0), GeVector3d(0, 1, 0));
const GeLine3d GeLine3d::kZAxis = GeLine3d(GePoint3d(0, 0, 0), GeVector3d(0, 0, 1));

GeLine3d::GeLine3d()
{
	GE_IMP_MEMORY_ENTITY(GeLine3d);

	this->set(GePoint3d(0, 0, 0), GeVector3d(0, 0, 0));
}
GeLine3d::GeLine3d(const GeLine3d& source)
{
	GE_IMP_MEMORY_ENTITY(GeLine3d);

	this->set(GE_IMP_LINE3D(source.m_pImpl)->origin, GE_IMP_LINE3D(source.m_pImpl)->vector);
}
GeLine3d::GeLine3d(const GePoint3d& point, const GeVector3d& vect)
{
	GE_IMP_MEMORY_ENTITY(GeLine3d);

	this->set(point, vect);
}
GeLine3d::GeLine3d(const GePoint3d& point1, const GePoint3d& point2)
{
	GE_IMP_MEMORY_ENTITY(GeLine3d);

	this->set(point1, point2);
}

GeLine3d& GeLine3d::set(const GePoint3d& point, const GeVector3d& vect)
{
	GE_IMP_LINE3D(this->m_pImpl)->origin = point;
	GE_IMP_LINE3D(this->m_pImpl)->vector = vect;
	return *this;
}
GeLine3d& GeLine3d::set(const GePoint3d& point1, const GePoint3d& point2)
{
	GE_IMP_LINE3D(this->m_pImpl)->origin = point1;
	GE_IMP_LINE3D(this->m_pImpl)->vector = point2 - point1;
	return *this;
}
GeLine3d& GeLine3d::operator=(const GeLine3d& entity)
{
	this->set(GE_IMP_LINE3D(entity.m_pImpl)->origin, GE_IMP_LINE3D(entity.m_pImpl)->vector);
	return *this;
}

bool GeLine3d::isKindOf(Ge::EntityId entType) const
{
	if (entType == this->type())
	{
		return true;
	}
	return false;
}
Ge::EntityId GeLine3d::type() const
{
	return Ge::EntityId::kLine3d;
}
GeLine3d* GeLine3d::copy() const
{
	GeLine3d* pLine = new GeLine3d();
	pLine->set(GE_IMP_LINE3D(this->m_pImpl)->origin, GE_IMP_LINE3D(this->m_pImpl)->vector);
	return pLine;
}
bool GeLine3d::operator==(const GeLine3d& entity) const
{
	return this->isEqualTo(entity);
}
bool GeLine3d::operator!=(const GeLine3d& entity) const
{
	return !this->isEqualTo(entity);
}
bool GeLine3d::isEqualTo(const GeLine3d& entity) const
{
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeLine3d::isEqualTo(const GeLine3d& entity, const GeTol& tol) const
{
	if (this->pointOnLine().isEqualTo(entity.pointOnLine(), tol) == false)
	{
		return false;
	}
	if (this->direction().isEqualTo(entity.direction(), tol) == false)
	{
		return false;
	}
	return true;
}
GeLine3d& GeLine3d::transformBy(const GeMatrix3d& xfm)
{
	GePoint3d p1 = GE_IMP_LINE3D(this->m_pImpl)->origin;
	GePoint3d p2 = p1 + GE_IMP_LINE3D(this->m_pImpl)->vector;
	p1.transformBy(xfm);
	p2.transformBy(xfm);
	this->set(p1, p2);
	return *this;
}
GeLine3d& GeLine3d::translateBy(const GeVector3d& translateVec)
{
	this->set(this->pointOnLine() + translateVec, this->direction() * this->length());
	return *this;
}
GeLine3d& GeLine3d::rotateBy(double angle, const GeVector3d& vec)
{
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GeLine3d& GeLine3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
	GePoint3d p1 = GE_IMP_LINE3D(this->m_pImpl)->origin;
	GePoint3d p2 = p1 + GE_IMP_LINE3D(this->m_pImpl)->vector;
	p1.rotateBy(angle, vec, wrtPoint);
	p2.rotateBy(angle, vec, wrtPoint);
	this->set(p1, p2);
	return *this;
}
GeLine3d& GeLine3d::mirror(const GePlane& plane)
{
	GePoint3d p1 = GE_IMP_LINE3D(this->m_pImpl)->origin;
	GePoint3d p2 = p1 + GE_IMP_LINE3D(this->m_pImpl)->vector;
	p1.mirror(plane);
	p2.mirror(plane);
	this->set(p1, p2);
	return *this;
}
GeLine3d& GeLine3d::scaleBy(double scaleFactor)
{
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GeLine3d& GeLine3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
	GePoint3d p1 = GE_IMP_LINE3D(this->m_pImpl)->origin;
	GePoint3d p2 = p1 + GE_IMP_LINE3D(this->m_pImpl)->vector;
	p1.scaleBy(GeScale3d(scaleFactor, scaleFactor, scaleFactor), wrtPoint);
	p2.scaleBy(GeScale3d(scaleFactor, scaleFactor, scaleFactor), wrtPoint);
	this->set(p1, p2);
	return *this;
}
bool GeLine3d::isOn(const GePoint3d& pnt) const
{
	return this->isOn(pnt, GeContext::gTol);
}
bool GeLine3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
	GePoint3d vertical = GeLinearEnt3d::vertical(pnt, *this, tol);
	if (vertical.distanceTo(pnt) > tol.equalPoint())
	{
		return false;
	}
	return true;
}

double GeLine3d::length() const
{
	return GE_IMP_LINE3D(this->m_pImpl)->vector.length();
}
double GeLine3d::length(double fromParam, double toParam) const
{
	return this->length(0, 1, GeContext::gTol.equalPoint());
}
double GeLine3d::length(double fromParam, double toParam, double tol) const
{
	return (toParam - fromParam) * this->length();
}
double GeLine3d::paramAtLength(double datumParam, double length) const
{
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeLine3d::paramAtLength(double datumParam, double length, double tol) const
{
	double param = 0.0;

	param = datumParam + length / this->length();

	return param;
}
double GeLine3d::area() const
{
	return this->area(GeContext::gTol);
}
double GeLine3d::area(const GeTol& tol) const
{
	return 0.0;
}
double GeLine3d::distanceTo(const GePoint3d& point) const
{
	return this->distanceTo(point, GeContext::gTol);
}
double GeLine3d::distanceTo(const GePoint3d& point, const GeTol& tol) const
{
	GePoint3d closest = this->closestPointTo(point, tol);
	return closest.distanceTo(point);
}
double GeLine3d::distanceTo(const GeLine3d& entity) const
{
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLine3d::distanceTo(const GeLine3d& line, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLine3d::distanceTo(const GeLineSeg3d& entity) const
{
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLine3d::distanceTo(const GeLineSeg3d& line, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLine3d::distanceTo(const GeRay3d& entity) const
{
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLine3d::distanceTo(const GeRay3d& line, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLine3d::distanceTo(const GeCircArc3d& entity) const
{
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLine3d::distanceTo(const GeCircArc3d& arc, const GeTol& tol) const
{

	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(arc, closest, tol);
	return closest.distanceTo(itself);
}
GePoint3d GeLine3d::closestPointTo(const GePoint3d& pnt) const
{
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeLine3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const
{
	GePoint3d point = GeLine3d::vertical(pnt, *this, tol);
	return point;
}
GePoint3d GeLine3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLine3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;

	do
	{
		// 如果两直线平行
		if (this->isParallelTo(curve3d, tol) == true)
		{
			pntOnOtherCrv = curve3d.pointOnLine();
			closest = GeLine3d::vertical(pntOnOtherCrv, *this);
			break;
		}

		// 如果两直线存在交点
		GePoint3d intersect;
		if (this->intersectWith(curve3d, intersect, tol) == true)
		{
			pntOnOtherCrv.set(intersect.x, intersect.y, intersect.z);
			closest.set(intersect.x, intersect.y, intersect.z);
			break;
		}

		// 两法向叉乘得到公垂线
		GeVector3d vertical = this->direction().crossProduct(curve3d.direction());

		// 构建一个平面
		GePlane plane(curve3d.pointOnLine(), curve3d.direction(), vertical);

		// 点沿着直线方向到平面的交点作为最近点
		closest = this->pointOnLine().project(plane, this->direction());

		pntOnOtherCrv = GeLine3d::vertical(closest, curve3d);

	} while (false);

	return closest;
}
GePoint3d GeLine3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLine3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;

	do
	{
		// 判断是否相交
		GePoint3d intersect;
		if (this->intersectWith(curve3d, intersect, tol) == true)
		{
			pntOnOtherCrv.set(intersect.x, intersect.y, intersect.z);
			closest.set(intersect.x, intersect.y, intersect.z);
			break;
		}

		GePoint3d point;
		GePoint3dArray pointItselfs, pointOthers;

		// 获得两直线最近点
		GeLine3d lineOtherCrv(curve3d.pointOnLine(), curve3d.direction());
		GePoint3d pointOtherCrv;
		point = this->closestPointTo(lineOtherCrv, pointOtherCrv, tol);
		if (lineOtherCrv.isOn(pointOtherCrv, tol) == true)
		{
			pointItselfs.append(point);
			pointOthers.append(pointOtherCrv);
		}
		else
		{
			point = this->closestPointTo(curve3d.startPoint(), tol);
			pointItselfs.append(point);
			pointOthers.append(curve3d.startPoint());

			point = this->closestPointTo(curve3d.endPoint(), tol);
			pointItselfs.append(point);
			pointOthers.append(curve3d.endPoint());
		}

		// 计算出最近点
		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++)
		{
			double dist = pointItselfs[i].distanceTo(pointOthers[i]);
			if (i == 0)
			{
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pntOnOtherCrv.set(pointOthers[i].x, pointOthers[i].y, pointOthers[i].z);
				continue;
			}
			if (dist < minDist)
			{
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pntOnOtherCrv.set(pointOthers[i].x, pointOthers[i].y, pointOthers[i].z);
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeLine3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLine3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;

	do
	{
		// 判断是否相交
		GePoint3d intersect;
		if (this->intersectWith(curve3d, intersect, tol) == true)
		{
			pntOnOtherCrv.set(intersect.x, intersect.y, intersect.z);
			closest.set(intersect.x, intersect.y, intersect.z);
			break;
		}

		GePoint3d point;
		GePoint3dArray pointItselfs, pointOthers;

		// 获得两直线最近点
		GeLine3d lineOtherCrv(curve3d.pointOnLine(), curve3d.direction());
		GePoint3d pointOtherCrv;
		point = this->closestPointTo(lineOtherCrv, pointOtherCrv, tol);
		if (lineOtherCrv.isOn(pointOtherCrv, tol) == true)
		{
			pointItselfs.append(point);
			pointOthers.append(pointOtherCrv);
		}
		else
		{
			point = this->closestPointTo(curve3d.pointOnLine(), tol);
			pointItselfs.append(point);
			pointOthers.append(curve3d.pointOnLine());
		}

		// 计算出最近点
		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++)
		{
			double dist = pointItselfs[i].distanceTo(pointOthers[i]);
			if (i == 0)
			{
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pntOnOtherCrv.set(pointOthers[i].x, pointOthers[i].y, pointOthers[i].z);
				continue;
			}
			if (dist < minDist)
			{
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pntOnOtherCrv.set(pointOthers[i].x, pointOthers[i].y, pointOthers[i].z);
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeLine3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLine3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{

	GePoint3d closest;

	do
	{
		GePoint3dArray pointItselfs, pointOthers;

		// 获得点到直线的垂点
		GePoint3d vertical = GeLine3d::vertical(curve3d.center(), *this);

		// 获得圆心到垂点的矢量
		GeVector3d vec = vertical - curve3d.center();

		// 矢量和圆法向的叉乘得到垂线
		GeVector3d cross = vec.crossProduct(curve3d.normal());

		// 获得圆的最近点
		GeMatrix3d mat;
		GePoint3d pointLine;
		GePoint3d point = curve3d.center() + curve3d.normal() * curve3d.radius();
		mat.setToRotation(PI / 2.0, cross, curve3d.center());
		point.transformBy(mat);
		pointLine = GeLine3d::vertical(point, *this);
		if (curve3d.isOn(point, tol) == true)
		{
			pointItselfs.append(pointLine);
			pointOthers.append(point);
		}

		point = curve3d.center() + curve3d.normal() * curve3d.radius();
		mat.setToRotation(0 - PI / 2.0, cross, curve3d.center());
		point.transformBy(mat);
		pointLine = GeLine3d::vertical(point, *this);
		if (curve3d.isOn(point, tol) == true)
		{
			pointItselfs.append(pointLine);
			pointOthers.append(point);
		}

		// 获得端点
		pointItselfs.append(GeLine3d::vertical(curve3d.startPoint(), *this));
		pointOthers.append(curve3d.startPoint());

		pointItselfs.append(GeLine3d::vertical(curve3d.endPoint(), *this));
		pointOthers.append(curve3d.endPoint());

		// 计算出最近点
		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++)
		{
			double dist = pointItselfs[i].distanceTo(pointOthers[i]);
			if (i == 0)
			{
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pntOnOtherCrv.set(pointOthers[i].x, pointOthers[i].y, pointOthers[i].z);
				continue;
			}
			if (dist < minDist)
			{
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pntOnOtherCrv.set(pointOthers[i].x, pointOthers[i].y, pointOthers[i].z);
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeLine3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection) const
{
	return this->projClosestPointTo(pnt, projectDirection, GeContext::gTol);
}
GePoint3d GeLine3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeLine3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLine3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeLine3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLine3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeLine3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLine3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeLine3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLine3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
void GeLine3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const
{
	return this->getProjClosestPointTo(pnt, projectDirection, pntOnCrv, GeContext::gTol);
}
void GeLine3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const
{
}
void GeLine3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeLine3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
void GeLine3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeLine3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
void GeLine3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeLine3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
void GeLine3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeLine3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
bool GeLine3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const
{
	return this->getNormalPoint(pnt, pntOnCrv, GeContext::gTol);
}
bool GeLine3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const
{

	GePoint3d vertical = GeLine3d::vertical(pnt, *this);
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(this->paramOf(vertical, tol));
	return true;
}
GeLine3d* GeLine3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const
{
	return this->project(projectionPlane, projectDirection, GeContext::gTol);
}
GeLine3d* GeLine3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const
{

	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction();

	p1.project(projectionPlane, projectDirection);
	p2.project(projectionPlane, projectDirection);

	GeLine3d* line = new GeLine3d();
	line->set(p1, p2 - p1);
	return line;
}
GeLine3d* GeLine3d::orthoProject(const GePlane& projectionPlane) const
{
	return this->orthoProject(projectionPlane, GeContext::gTol);
}
GeLine3d* GeLine3d::orthoProject(const GePlane& projectionPlane, const GeTol& tol) const
{
	return this->project(projectionPlane, projectionPlane.normal(), tol);
}
bool GeLine3d::isOn(const GePoint3d& pnt, double& param) const
{
	return this->isOn(pnt, param, GeContext::gTol);
}
bool GeLine3d::isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const
{
	param = this->paramOf(pnt, tol);
	return this->isOn(pnt, tol);
}
bool GeLine3d::isOn(double param) const
{
	return this->isOn(param, GeContext::gTol);
}
bool GeLine3d::isOn(double param, const GeTol& tol) const
{
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);

	return this->isOn(pointOnCurve.point(), tol);
}
GECURVE3D_METHODS_GETCLOSESTPOINTTO(GeLine3d);
double GeLine3d::paramOf(const GePoint3d& pnt) const
{
	return this->paramOf(pnt, GeContext::gTol);
}
double GeLine3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{

	double param = 0.0;

	do
	{
		GeLine3d line(this->pointOnLine(), this->direction());
		if (line.isOn(pnt, tol) == false)
		{
			break;
		}

		GeVector3d direction = pnt - this->pointOnLine();
		direction.normalize();
		if (direction.isEqualTo(this->direction(), tol) == true)
		{
			param = (pnt.distanceTo(this->pointOnLine())) / this->length();
		}
		else
		{
			param = 0 - (pnt.distanceTo(this->pointOnLine())) / this->length();
		}

	} while (false);

	return param;
}
void GeLine3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList) const
{
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeLine3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const
{
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, extensionType, GeContext::gTol);
}
void GeLine3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const
{
	if (abs(distance) < 0.0000001)
	{
		GeLine3d* line = new GeLine3d();
		line->set(GE_IMP_LINEARENT3D(this->m_pImpl)->origin, GE_IMP_LINEARENT3D(this->m_pImpl)->vector);
		offsetCurveList.append(line);
		return;
	}

	GeVector3d axis = this->direction();
	axis.rotateBy(PI / 2.0, planeNormal);
	axis.normalize();

	GePoint3d origin = GE_IMP_LINE3D(this->m_pImpl)->origin + axis * distance;

	GeLine3d* line = new GeLine3d();
	line->set(origin, GE_IMP_LINE3D(this->m_pImpl)->vector);

	offsetCurveList.append(line);
}
bool GeLine3d::isClosed() const
{
	return this->isClosed(GeContext::gTol);
}
bool GeLine3d::isClosed(const GeTol& tol) const
{
	return false;
}
bool GeLine3d::isPlanar(GePlane& plane) const
{
	return this->isPlanar(plane, GeContext::gTol);
}
bool GeLine3d::isPlanar(GePlane& plane, const GeTol& tol) const
{
	return false;
}
bool GeLine3d::isLinear(GeLine3d& line) const
{
	return this->isLinear(line, GeContext::gTol);
}
bool GeLine3d::isLinear(GeLine3d& line, const GeTol& tol) const
{
	return false;
}
bool GeLine3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeLine3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane, const GeTol& tol) const
{

	// 判断是否共线
	if (this->isOn(curve3d.pointOnLine(), tol) == true &&
		this->direction().isParallelTo(curve3d.direction(), tol) == true)
	{
		return false;
	}

	// 判断是否平行
	if (this->isParallelTo(curve3d, tol) == true)
	{

		GeVector3d cross = curve3d.pointOnLine() - this->pointOnLine();
		cross = cross.crossProduct(this->direction());
		plane.set(this->pointOnLine(), cross.normal());
		return true;
	}

	// 判断是否有交点
	GePoint3d intersect;
	if (this->intersectWith(curve3d, intersect, tol) == false)
	{
		return false;
	}

	// 设置平面
	plane.set(intersect, this->direction().crossProduct(curve3d.direction()));

	return true;
}
bool GeLine3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeLine3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane, const GeTol& tol) const
{

	GeLine3d line1, line2;
	line1.set(this->pointOnLine(), this->direction());
	line2.set(curve3d.pointOnLine(), curve3d.direction());
	return line1.isCoplanarWith(line2, plane, tol);
}
bool GeLine3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeLine3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane, const GeTol& tol) const
{
	GeLine3d line1, line2;
	line1.set(this->pointOnLine(), this->direction());
	line2.set(curve3d.pointOnLine(), curve3d.direction());
	return line1.isCoplanarWith(line2, plane, tol);
}
bool GeLine3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeLine3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane, const GeTol& tol) const
{

	if (this->direction().isPerpendicularTo(curve3d.normal(), tol) == false)
	{
		return false;
	}

	plane.set(curve3d.center(), curve3d.normal());
	if (plane.isOn(this->pointOnLine(), tol) == false)
	{
		return false;
	}

	return true;
}
void GeLine3d::getSplitCurves(double param, GeCurve3d* piece1, GeCurve3d* piece2) const
{
	GePointOnCurve3d pointOnCurve(*this, param);
	GePoint3d point = pointOnCurve.point();
	piece1 = new GeRay3d(point, this->direction());
	piece2 = new GeRay3d(point, this->direction().negate());
}
bool GeLine3d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const
{
	return false;
}
GeBoundBlock3d GeLine3d::boundBlock() const
{
	GeInterval range;
	range.set(this->paramOf(this->pointOnLine()), this->paramOf(this->pointOnLine() + this->direction() * this->length()));
	return this->boundBlock(range);
}
GeBoundBlock3d GeLine3d::boundBlock(const GeInterval& range) const
{

	GeBoundBlock3d boundBlock;

	GePoint3d basePoint;
	basePoint = this->pointOnLine() + (this->direction() * this->length()) * range.lowerBound();

	GePoint3d endPoint;
	endPoint = this->pointOnLine() + (this->direction() * this->length()) * range.upperBound();

	boundBlock.set(basePoint, endPoint);

	return boundBlock;
}
GeBoundBlock3d GeLine3d::orthoBoundBlock() const
{
	GeInterval range;
	range.set(this->paramOf(this->pointOnLine()), this->paramOf(this->pointOnLine() + this->direction() * this->length()));
	return this->orthoBoundBlock(range);
}
GeBoundBlock3d GeLine3d::orthoBoundBlock(const GeInterval& range) const
{

	GeBoundBlock3d boundBlock;

	GePoint3dArray points;
	points.append(this->pointOnLine() + (this->direction() * this->length()) * range.lowerBound());
	points.append(this->pointOnLine() + (this->direction() * this->length()) * range.upperBound());

	GePoint3d minPoint, maxPoint;
	for (int i = 0; i < points.length(); i++)
	{
		if (i == 0)
		{
			minPoint = points[i];
			maxPoint = points[i];
			continue;
		}

		if (points[i].x < minPoint.x)
		{
			minPoint.x = points[i].x;
		}
		if (points[i].x > maxPoint.x)
		{
			maxPoint.x = points[i].x;
		}
		if (points[i].y < minPoint.y)
		{
			minPoint.y = points[i].y;
		}
		if (points[i].y > maxPoint.y)
		{
			maxPoint.y = points[i].y;
		}
	}

	boundBlock.set(minPoint, maxPoint);

	return boundBlock;
}
bool GeLine3d::hasStartPoint(GePoint3d& startPoint) const
{
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(0.0);
	startPoint = pointOnCurve.point();
	return false;
}
bool GeLine3d::hasEndPoint(GePoint3d& endPoint) const
{
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(1.0);
	endPoint = pointOnCurve.point();
	return false;
}
GePoint3d GeLine3d::evalPoint(double param) const
{
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);
	return pointOnCurve.point();
}
void GeLine3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const
{
	return this->getSamplePoints(fromParam, toParam, approxEps, pointArray, paramArray);
}
void GeLine3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray, bool forceResampling) const
{

	double param = fromParam;

	while (true)
	{

		pointArray.append(this->evalPoint(param));
		paramArray.append(param);

		param = param + approxEps;
		if (param > toParam)
		{
			break;
		}
	}
}
void GeLine3d::getSamplePoints(int numSample, GePoint3dArray& pointArray) const
{
	return this->getSamplePoints(numSample, pointArray);
}
void GeLine3d::getSamplePoints(int numSample, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const
{
	for (int i = 0; i < numSample; i++)
	{
		double param = 1.0 / numSample * i;
		pointArray.append(this->evalPoint(param));
		paramArray.append(param);
	}
}

bool GeLine3d::intersectWith(const GeLine3d& line, GePoint3d& intPnt) const
{
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLine3d::intersectWith(const GeLine3d& line, GePoint3d& intPnt, const GeTol& tol) const
{

	bool isValue = true;

	do
	{
		//判断是否平行
		if (GE_IMP_LINEARENT3D(this->m_pImpl)->vector.isParallelTo(GE_IMP_LINEARENT3D(line.m_pImpl)->vector) == true)
		{
			isValue = false;
			break;
		}

		//求直线点1到直线2的垂点
		GePoint3d vertical = GeLinearEnt3d::vertical(GE_IMP_LINEARENT3D(this->m_pImpl)->origin, line);
		if (vertical.isEqualTo(GE_IMP_LINEARENT3D(this->m_pImpl)->origin, tol) == true) {
			intPnt.set(vertical.x, vertical.y, vertical.z);
			break;
		}

		// 获得两直线的夹角
		double angle = this->direction().angleTo(line.direction());

		// 得到邻边长度
		double dist = this->pointOnLine().distanceTo(vertical);
		dist = dist / tan(angle);

		// 获得交点
		GeVector3d direction = line.direction();
		GePoint3d intersect = vertical + direction * dist;
		if (this->isOn(intersect, tol) == false)
		{
			intersect = vertical - direction * dist;
		}
		intPnt.x = intersect.x;
		intPnt.y = intersect.y;

} while (false);

	return isValue;


#if 0
// 计算两直线的交点
GeVector3d u = this->direction();
GeVector3d v = line.direction();
GeVector3d w = this->pointOnLine() - line.pointOnLine();

double a = u.dotProduct(u);
double b = u.dotProduct(v);
double c = v.dotProduct(v);
double d = u.dotProduct(w);
double e = v.dotProduct(w);

double denom = a * c - b * b;

// 检查分母，如果为0，线是平行的
if (fabs(denom) < GeContext::gTol.equalVector())
{
	return false;
}

// 计算参数s和t
double s = (b * e - c * d) / denom;
double t = (a * e - b * d) / denom;

// 计算交点
intPnt = this->pointOnLine() + u * s;
return true;
#endif
}
bool GeLine3d::intersectWith(const GeRay3d& line, GePoint3d& intPnt) const
{
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLine3d::intersectWith(const GeRay3d& line, GePoint3d& intPnt, const GeTol& tol) const
{

	GeLine3d line3d(line.pointOnLine(), line.direction());
	if (this->intersectWith(line3d, intPnt, tol) == false)
	{
		return false;
	}
	if (line.isOn(intPnt, tol) == false)
	{
		return false;
	}
	return true;
}
bool GeLine3d::intersectWith(const GeLineSeg3d& line, GePoint3d& intPnt) const
{
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLine3d::intersectWith(const GeLineSeg3d& line, GePoint3d& intPnt, const GeTol& tol) const
{
	GeLine3d line3d(line.pointOnLine(), line.direction());
	if (this->intersectWith(line3d, intPnt, tol) == false)
	{
		return false;
	}
	if (line.isOn(intPnt, tol) == false)
	{
		return false;
	}
	return true;
}
