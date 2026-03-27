#include "DbEllipse.h"
#include "GePlane.h"
#include "DbImpl.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "GeEllipArc3d.h"
#include "DbExtents.h"
#include <cmath>


DbEllipse::DbEllipse() {
	this->m_pImpl = new DbEllipseImpl();
}
DbEllipse::DbEllipse(const GePoint3d& center, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double radiusRatio) {
	this->m_pImpl = new DbEllipseImpl();
	this->set(center, unitNormal, majorAxis, radiusRatio);
}
DbEllipse::DbEllipse(const GePoint3d& center, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double radiusRatio, double startAngle, double endAngle) {
	this->m_pImpl = new DbEllipseImpl();
	this->set(center, unitNormal, majorAxis, radiusRatio, startAngle, endAngle);
}
DbEllipse::~DbEllipse() {

}




Acad::ErrorStatus DbEllipse::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	GePoint3d center;
	pFiler->readPoint3d(&center);
	this->setCenter(center);

	GeVector3d majorAxis;
	pFiler->readVector3d(&majorAxis);
	DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis = majorAxis;

	GeVector3d minorAxis;
	pFiler->readVector3d(&minorAxis);
	DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis = minorAxis;

	double radiusRatio;
	pFiler->readDouble(&radiusRatio);
	DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = radiusRatio;

	double startAngle;
	pFiler->readDouble(&startAngle);
	DB_IMP_ELLIPSE(this->m_pImpl)->startAngle = startAngle;

	double endAngle;
	pFiler->readDouble(&endAngle);
	DB_IMP_ELLIPSE(this->m_pImpl)->endAngle = endAngle;

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	DB_IMP_ELLIPSE(this->m_pImpl)->normal = normal;

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->center());
	pFiler->writeVector3d(DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis);
	pFiler->writeVector3d(DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis);
	pFiler->writeDouble(DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio);
	pFiler->writeDouble(DB_IMP_ELLIPSE(this->m_pImpl)->startAngle);
	pFiler->writeDouble(DB_IMP_ELLIPSE(this->m_pImpl)->endAngle);
	pFiler->writeVector3d(DB_IMP_ELLIPSE(this->m_pImpl)->normal);

	return Acad::ErrorStatus::eOk;
}
bool DbEllipse::subWorldDraw(GiWorldDraw* pWd) const {

	// 构造椭圆弧几何体
	GeVector3d major = this->majorAxis();
	double majorR = this->majorRadius();
	double minorR = this->minorRadius();
	GeVector3d unitNormal = this->normal();

	GeEllipArc3d arc;
	arc.set(this->center(), unitNormal, major, majorR, minorR, this->startAngle(), this->endAngle());

	// 绘制椭圆弧
	pWd->geometry().ellipArc(arc);

	return true;
}



GePoint3d DbEllipse::center() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->center;
}
Acad::ErrorStatus DbEllipse::setCenter(const GePoint3d& center) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ELLIPSE(this->m_pImpl)->center = center;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GeVector3d DbEllipse::normal() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->normal;
}
GeVector3d DbEllipse::majorAxis() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis;
}
GeVector3d DbEllipse::minorAxis() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis;
}
double DbEllipse::majorRadius() const {
	return this->majorAxis().length();
}
Acad::ErrorStatus DbEllipse::setMajorRadius(double _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		// 获得短轴长度
		double minorRadius = this->minorRadius();

		// 当长轴大于短轴
		if (_v > minorRadius) {
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis *= _v;
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length() / DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.length();
		}
		// 当短轴大于长轴时将两轴调换
		else {
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= _v;
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis *= minorRadius;
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length() / DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.length();
		}

		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
double DbEllipse::minorRadius() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length();
}
Acad::ErrorStatus DbEllipse::setMinorRadius(double _v) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		// 获得长轴长度
		double majorRadius = this->majorRadius();

		// 当短轴小于长轴
		if (_v < majorRadius) {
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= _v;
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length() / DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.length();
		}
		// 将两轴调换
		else {
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= majorRadius;
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis *= _v;
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length() / DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.length();
		}

		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbEllipse::radiusRatio() const {
	return  DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio;
}
Acad::ErrorStatus DbEllipse::setRadiusRatio(double ratio) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (abs(ratio) > 1) {
			return Acad::ErrorStatus::eFail;
		}

		if (ratio > 0) {
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= (this->majorRadius() * ratio);
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = ratio;
		}
		else {
			GeVector3d v = DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis;
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis;
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis = v;

			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= (this->majorRadius() * abs(ratio));
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = abs(ratio);
		}

		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbEllipse::startAngle() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->startAngle;
}
Acad::ErrorStatus DbEllipse::setStartAngle(double startAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ELLIPSE(this->m_pImpl)->startAngle = startAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbEllipse::endAngle() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->endAngle;
}
Acad::ErrorStatus DbEllipse::setEndAngle(double endAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ELLIPSE(this->m_pImpl)->endAngle = endAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbEllipse::setStartParam(double startParam) {
	this->setStartAngle(startParam);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::setEndParam(double endParam) {
	this->setEndAngle(endParam);
	return Acad::ErrorStatus::eOk;
}

double DbEllipse::paramAtAngle(double angle) const {
	return angle;
}
double DbEllipse::angleAtParam(double param) const {
	return param;
}

Acad::ErrorStatus DbEllipse::get(
	GePoint3d& center,
	GeVector3d& unitNormal,
	GeVector3d& majorAxis, double& radiusRatio, double& startAngle,
	double& endAngle) const {
	center = DB_IMP_ELLIPSE(this->m_pImpl)->center;
	unitNormal = DB_IMP_ELLIPSE(this->m_pImpl)->normal;
	majorAxis = DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis;
	radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio;
	startAngle = DB_IMP_ELLIPSE(this->m_pImpl)->startAngle;
	endAngle = DB_IMP_ELLIPSE(this->m_pImpl)->endAngle;
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEllipse::set(const GePoint3d& center, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double radiusRatio) {
	return this->set(center, unitNormal, majorAxis, radiusRatio, 0.0, PI * 2);
}
Acad::ErrorStatus DbEllipse::set(
	const GePoint3d& center,
	const GeVector3d& unitNormal,
	const GeVector3d& majorAxis,
	double radiusRatio,
	double startAngle,
	double endAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		if (this->isNull() == true) {
			return Acad::ErrorStatus::eFail;
		}

		DB_IMP_ELLIPSE(this->m_pImpl)->center = center;
		DB_IMP_ELLIPSE(this->m_pImpl)->normal = unitNormal;
		DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis = majorAxis;
		DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = radiusRatio;
		DB_IMP_ELLIPSE(this->m_pImpl)->startAngle = startAngle;
		DB_IMP_ELLIPSE(this->m_pImpl)->endAngle = endAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Adesk::Boolean DbEllipse::isNull() const {
	if (DB_IMP_ELLIPSE(this->m_pImpl)->normal.isPerpendicularTo(DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis) == false) {
		return true;
	}
	if (DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio > 1 + GeContext::gTol.equalVector()) {
		return true;
	}
	if (abs(DB_IMP_ELLIPSE(this->m_pImpl)->startAngle - DB_IMP_ELLIPSE(this->m_pImpl)->endAngle) < GeContext::gTol.equalVector()) {
		return true;
	}
	return false;
}


Acad::ErrorStatus DbEllipse::subGetGeomExtents(DbExtents &extents) const
{
	// 采样椭圆上的点来计算包围盒
	double sa = this->startAngle();
	double ea = this->endAngle();
	if (ea <= sa) ea += 2.0 * PI;
	int nSeg = 64;
	double step = (ea - sa) / nSeg;
	for (int i = 0; i <= nSeg; i++)
	{
		GePoint3d pt;
		this->getPointAtParam(sa + step * i, pt);
		extents.addPoint(pt);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::subTransformBy(const GeMatrix3d& xform)
{
	GePoint3d cen = this->center();
	GeVector3d major = this->majorAxis();
	GeVector3d minor = this->minorAxis();
	GeVector3d norm = this->normal();

	// 变换中心和轴向量
	cen.transformBy(xform);
	major.transformBy(xform);
	minor.transformBy(xform);

	// 重新计算法线
	norm = major.crossProduct(minor);
	if (norm.length() > 1e-10)
		norm.normalize();

	double majorLen = major.length();
	double minorLen = minor.length();

	// 确保长轴 >= 短轴
	if (minorLen > majorLen)
	{
		GeVector3d temp = major;
		major = minor;
		minor = temp;
		double tl = majorLen;
		majorLen = minorLen;
		minorLen = tl;
	}

	double ratio = (majorLen > 1e-10) ? (minorLen / majorLen) : 0.0;

	DB_IMP_ELLIPSE(this->m_pImpl)->center = cen;
	DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis = major;
	DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis = minor;
	DB_IMP_ELLIPSE(this->m_pImpl)->normal = norm;
	DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = ratio;

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEllipse::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{
	// 中心 + 4个轴端点
	DbGripData* gripData = new DbGripData();
	gripData->setGripPoint(this->center());
	grips.append(gripData);

	GePoint3d pt;
	this->getPointAtParam(0.0, pt);
	gripData = new DbGripData();
	gripData->setGripPoint(pt);
	grips.append(gripData);

	this->getPointAtParam(PI / 2.0, pt);
	gripData = new DbGripData();
	gripData->setGripPoint(pt);
	grips.append(gripData);

	this->getPointAtParam(PI, pt);
	gripData = new DbGripData();
	gripData->setGripPoint(pt);
	grips.append(gripData);

	this->getPointAtParam(PI * 1.5, pt);
	gripData = new DbGripData();
	gripData->setGripPoint(pt);
	grips.append(gripData);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const
{
	if (osnapMode == Db::OsnapMode::kOsModeEnd)
	{
		// 椭圆弧的起点和终点
		if (!this->isClosed())
		{
			GePoint3d sp, ep;
			this->getStartPoint(sp);
			this->getEndPoint(ep);
			snapPoints.append(sp);
			snapPoints.append(ep);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeMid)
	{
		// 中点 = 弧参数范围的中间
		double sa = this->startAngle();
		double ea = this->endAngle();
		if (ea <= sa) ea += 2.0 * PI;
		double midParam = (sa + ea) / 2.0;
		GePoint3d mp;
		this->getPointAtParam(midParam, mp);
		snapPoints.append(mp);
	}
	else if (osnapMode == Db::OsnapMode::kOsModeNear)
	{
		GePoint3d closest;
		this->getClosestPointTo(pickPoint, closest, false);
		snapPoints.append(closest);
	}
	else if (osnapMode == Db::OsnapMode::kOsModeCen)
	{
		snapPoints.append(this->center());
	}

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEllipse::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset)
{
	bool moveCenter = false;
	bool moveAxis = false;
	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) == 0)
			moveCenter = true;
		else
			moveAxis = true;
	}

	if (moveCenter && !moveAxis)
	{
		// 仅移动中心点
		DB_IMP_ELLIPSE(this->m_pImpl)->center += offset;
	}
	else if (moveCenter && moveAxis)
	{
		// 整体移动
		DB_IMP_ELLIPSE(this->m_pImpl)->center += offset;
	}
	else if (moveAxis)
	{
		// 移动轴端点 — 简化为整体移动
		DB_IMP_ELLIPSE(this->m_pImpl)->center += offset;
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {

	if (pEnt == NULL) {
		return Acad::ErrorStatus::eFail;
	}

	return Acad::ErrorStatus::eOk;
}






bool DbEllipse::isClosed() const
{
	if (abs(DB_IMP_ELLIPSE(this->m_pImpl)->endAngle - DB_IMP_ELLIPSE(this->m_pImpl)->startAngle) < 0.00002) {
		return true;
	}
	return false;
}
bool DbEllipse::isPeriodic() const
{
	return false;
}
bool DbEllipse::isPlanar() const
{
	return true;
}
Acad::ErrorStatus DbEllipse::getPlane(GePlane& plane, Db::Planarity& planarity) const
{
	plane.set(this->center(), this->normal());
	planarity = Db::Planarity::kPlanar;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getStartParam(double& _v) const
{
	_v = this->startAngle();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getEndParam(double& _v) const
{
	_v = this->endAngle();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getStartPoint(GePoint3d& point) const
{
	return this->getPointAtParam(this->startAngle(), point);
}
Acad::ErrorStatus DbEllipse::getEndPoint(GePoint3d& point) const
{
	return this->getPointAtParam(this->endAngle(), point);
}
Acad::ErrorStatus DbEllipse::getPointAtParam(double param, GePoint3d& point) const
{
	// ODA: 椭圆参数方程 P = center + a*cos(t)*majorDir + b*sin(t)*minorDir
	GeVector3d majorDir = this->majorAxis();
	double a = majorDir.length();
	if (a < 1e-10) return Acad::ErrorStatus::eFail;
	majorDir.normalize();

	GeVector3d minorDir = this->minorAxis();
	double b = minorDir.length();
	minorDir.normalize();

	point = this->center() + majorDir * (a * cos(param)) + minorDir * (b * sin(param));
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getParamAtPoint(const GePoint3d& pt, double& param) const {
	GeVector3d majorDir = this->majorAxis();
	double a = majorDir.length();
	if (a < 1e-10) return Acad::ErrorStatus::eFail;
	majorDir.normalize();

	GeVector3d minorDir = this->minorAxis();
	double b = minorDir.length();
	if (b < 1e-10) return Acad::ErrorStatus::eFail;
	minorDir.normalize();

	GeVector3d v = pt - this->center();
	double x = v.dotProduct(majorDir) / a;
	double y = v.dotProduct(minorDir) / b;
	param = atan2(y, x);
	if (param < 0) param += 2.0 * PI;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getDistAtParam(double param, double& dist) const {
	// 椭圆弧长: 数值积分 Simpson
	double sa = this->startAngle();
	int N = 64;
	double h = (param - sa) / N;
	if (fabs(h) < 1e-15) { dist = 0.0; return Acad::ErrorStatus::eOk; }

	double a = this->majorRadius();
	double b = this->minorRadius();

	// ds/dt = sqrt(a^2*sin^2(t) + b^2*cos^2(t))
	auto speed = [a, b](double t) -> double {
		double st = sin(t), ct = cos(t);
		return sqrt(a * a * st * st + b * b * ct * ct);
	};

	double sum = speed(sa) + speed(param);
	for (int i = 1; i < N; i++)
	{
		double t = sa + i * h;
		sum += (i % 2 == 0) ? 2.0 * speed(t) : 4.0 * speed(t);
	}
	dist = fabs(sum * h / 3.0);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getParamAtDist(double dist, double& param) const {
	// 二分法求解: getDistAtParam(param) == dist
	double sa = this->startAngle();
	double ea = this->endAngle();
	if (ea <= sa) ea += 2.0 * PI;

	double lo = sa, hi = ea;
	for (int iter = 0; iter < 50; iter++)
	{
		double mid = (lo + hi) / 2.0;
		double d = 0.0;
		this->getDistAtParam(mid, d);
		if (d < dist)
			lo = mid;
		else
			hi = mid;
	}
	param = (lo + hi) / 2.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getDistAtPoint(const GePoint3d& point, double& dist) const {
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getDistAtParam(param, dist);
}
Acad::ErrorStatus DbEllipse::getPointAtDist(double dist, GePoint3d& point) const {
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtDist(dist, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getPointAtParam(param, point);
}
Acad::ErrorStatus DbEllipse::getFirstDeriv(double param, GeVector3d& firstDeriv) const {
	// d/dt(a*cos(t)*majorDir + b*sin(t)*minorDir) = -a*sin(t)*majorDir + b*cos(t)*minorDir
	GeVector3d majorDir = this->majorAxis();
	double a = majorDir.length();
	if (a < 1e-10) return Acad::ErrorStatus::eFail;
	majorDir.normalize();

	GeVector3d minorDir = this->minorAxis();
	double b = minorDir.length();
	minorDir.normalize();

	firstDeriv = majorDir * (-a * sin(param)) + minorDir * (b * cos(param));
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getFirstDeriv(const GePoint3d& point, GeVector3d& firstDeriv) const {
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getFirstDeriv(param, firstDeriv);
}
Acad::ErrorStatus DbEllipse::getSecondDeriv(double param, GeVector3d& secDeriv) const {
	GeVector3d majorDir = this->majorAxis();
	double a = majorDir.length();
	if (a < 1e-10) return Acad::ErrorStatus::eFail;
	majorDir.normalize();

	GeVector3d minorDir = this->minorAxis();
	double b = minorDir.length();
	minorDir.normalize();

	secDeriv = majorDir * (-a * cos(param)) + minorDir * (-b * sin(param));
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getSecondDeriv(const GePoint3d& point, GeVector3d& secDeriv) const {
	double param = 0.0;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::ErrorStatus::eOk) return es;
	return this->getSecondDeriv(param, secDeriv);
}
Acad::ErrorStatus DbEllipse::getClosestPointTo(const GePoint3d& givenPnt, GePoint3d& pointOnCurve, bool extend) const {
	// 迭代法: 在椭圆上采样找最近点, 然后精化
	double sa = this->startAngle();
	double ea = this->endAngle();
	if (extend) { sa = 0.0; ea = 2.0 * PI; }
	if (ea <= sa) ea += 2.0 * PI;

	double bestParam = sa;
	double bestDist = 1e30;
	int nSeg = 64;
	double step = (ea - sa) / nSeg;

	for (int i = 0; i <= nSeg; i++)
	{
		GePoint3d pt;
		double t = sa + step * i;
		this->getPointAtParam(t, pt);
		double d = pt.distanceTo(givenPnt);
		if (d < bestDist) { bestDist = d; bestParam = t; }
	}

	// Newton精化
	for (int iter = 0; iter < 10; iter++)
	{
		GePoint3d pt;
		this->getPointAtParam(bestParam, pt);
		GeVector3d deriv;
		this->getFirstDeriv(bestParam, deriv);
		GeVector3d secDeriv;
		this->getSecondDeriv(bestParam, secDeriv);

		GeVector3d diff = pt - givenPnt;
		double num = diff.dotProduct(deriv);
		double den = deriv.dotProduct(deriv) + diff.dotProduct(secDeriv);
		if (fabs(den) < 1e-15) break;
		double dt = num / den;
		bestParam -= dt;
		if (!extend)
		{
			if (bestParam < sa) bestParam = sa;
			if (bestParam > ea) bestParam = ea;
		}
		if (fabs(dt) < 1e-12) break;
	}

	this->getPointAtParam(bestParam, pointOnCurve);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getClosestPointTo(const GePoint3d& givenPnt, const GeVector3d& normal, GePoint3d& pointOnCurve, bool extend) const {
	GePoint3d projPnt = givenPnt.project(GePlane(this->center(), this->normal()), normal);
	return this->getClosestPointTo(projPnt, pointOnCurve, extend);
}
Acad::ErrorStatus DbEllipse::getOrthoProjectedCurve(const GePlane&, DbCurve*&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getProjectedCurve(const GePlane&, const GeVector3d&, DbCurve*&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getOffsetCurves(double, DbVoidPtrArray&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getSpline(DbSpline*&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getSplitCurves(const GeDoubleArray&, DbVoidPtrArray& curveSegments) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::extend(double) {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::extend(bool, const GePoint3d&) {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getArea(double& area) const {
	// 椭圆扇形面积 (ODA: 积分公式)
	double a = this->majorRadius();
	double b = this->minorRadius();
	if (this->isClosed())
	{
		area = PI * a * b;
	}
	else
	{
		// 扇形面积 = 0.5 * a * b * (endAngle - startAngle)
		double sa = this->startAngle();
		double ea = this->endAngle();
		if (ea <= sa) ea += 2.0 * PI;
		area = 0.5 * a * b * (ea - sa);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::reverseCurve() {
	double sa = this->startAngle();
	double ea = this->endAngle();
	this->setStartAngle(ea);
	this->setEndAngle(sa);
	// 反转法线方向
	DB_IMP_ELLIPSE(this->m_pImpl)->normal = DB_IMP_ELLIPSE(this->m_pImpl)->normal.negate();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const {
	GeVector3d major = this->majorAxis();
	double majorR = this->majorRadius();
	double minorR = this->minorRadius();
	GeVector3d norm = this->normal();
	GeEllipArc3d* pArc = new GeEllipArc3d();
	pArc->set(this->center(), norm, major, majorR, minorR, this->startAngle(), this->endAngle());
	pGeCurve = pArc;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol) {
	return Acad::ErrorStatus::eFail;
}



