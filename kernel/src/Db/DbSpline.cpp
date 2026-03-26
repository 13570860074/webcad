#include "DbSpline.h"
#include "GePlane.h"
#include "DbImpl.h"


DbSpline::DbSpline() {
	this->m_pImpl = new DbSplineImpl();
}
DbSpline::DbSpline(const GePoint3dArray& fitPoints, int order, double fitTolerance) {
	this->m_pImpl = new DbSplineImpl();
	DB_IMP_SPLINE(this->m_pImpl)->degree = order;
	DB_IMP_SPLINE(this->m_pImpl)->fitPoints = fitPoints;
	DB_IMP_SPLINE(this->m_pImpl)->fitTolerance = fitTolerance;
}
DbSpline::DbSpline(int degree, bool rational, bool closed, bool periodic,
	const GePoint3dArray& controlPoints, const GeDoubleArray& knots,
	const GeDoubleArray& weights, double controlPtTol, double knotTol) {
	this->m_pImpl = new DbSplineImpl();
	DB_IMP_SPLINE(this->m_pImpl)->degree = degree;
	DB_IMP_SPLINE(this->m_pImpl)->rational = rational;
	DB_IMP_SPLINE(this->m_pImpl)->closed = closed;
	DB_IMP_SPLINE(this->m_pImpl)->periodic = periodic;
	DB_IMP_SPLINE(this->m_pImpl)->controlPoints = controlPoints;
	DB_IMP_SPLINE(this->m_pImpl)->knots = knots;
	DB_IMP_SPLINE(this->m_pImpl)->weights = weights;
}
DbSpline::~DbSpline() {

}


Acad::ErrorStatus DbSpline::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	int degree;
	pFiler->readInt32(&degree);
	DB_IMP_SPLINE(this->m_pImpl)->degree = degree;

	bool rational;
	pFiler->readBool(&rational);
	DB_IMP_SPLINE(this->m_pImpl)->rational = rational;

	bool closed;
	pFiler->readBool(&closed);
	DB_IMP_SPLINE(this->m_pImpl)->closed = closed;

	bool periodic;
	pFiler->readBool(&periodic);
	DB_IMP_SPLINE(this->m_pImpl)->periodic = periodic;

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	DB_IMP_SPLINE(this->m_pImpl)->normal = normal;

	double fitTolerance;
	pFiler->readDouble(&fitTolerance);
	DB_IMP_SPLINE(this->m_pImpl)->fitTolerance = fitTolerance;

	GeVector3d startTangent;
	pFiler->readVector3d(&startTangent);
	DB_IMP_SPLINE(this->m_pImpl)->startTangent = startTangent;

	GeVector3d endTangent;
	pFiler->readVector3d(&endTangent);
	DB_IMP_SPLINE(this->m_pImpl)->endTangent = endTangent;

	// 读取控制点
	DB_IMP_SPLINE(this->m_pImpl)->controlPoints.removeAll();
	unsigned int numCtrl = 0;
	pFiler->readUInt32(&numCtrl);
	for (unsigned int i = 0; i < numCtrl; i++) {
		GePoint3d pt;
		pFiler->readPoint3d(&pt);
		DB_IMP_SPLINE(this->m_pImpl)->controlPoints.append(pt);
	}

	// 读取节点向量
	DB_IMP_SPLINE(this->m_pImpl)->knots.removeAll();
	unsigned int numKnots = 0;
	pFiler->readUInt32(&numKnots);
	for (unsigned int i = 0; i < numKnots; i++) {
		double val;
		pFiler->readDouble(&val);
		DB_IMP_SPLINE(this->m_pImpl)->knots.append(val);
	}

	// 读取权重
	DB_IMP_SPLINE(this->m_pImpl)->weights.removeAll();
	unsigned int numWeights = 0;
	pFiler->readUInt32(&numWeights);
	for (unsigned int i = 0; i < numWeights; i++) {
		double val;
		pFiler->readDouble(&val);
		DB_IMP_SPLINE(this->m_pImpl)->weights.append(val);
	}

	// 读取拟合点
	DB_IMP_SPLINE(this->m_pImpl)->fitPoints.removeAll();
	unsigned int numFit = 0;
	pFiler->readUInt32(&numFit);
	for (unsigned int i = 0; i < numFit; i++) {
		GePoint3d pt;
		pFiler->readPoint3d(&pt);
		DB_IMP_SPLINE(this->m_pImpl)->fitPoints.append(pt);
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeInt32(DB_IMP_SPLINE(this->m_pImpl)->degree);
	pFiler->writeBool(DB_IMP_SPLINE(this->m_pImpl)->rational);
	pFiler->writeBool(DB_IMP_SPLINE(this->m_pImpl)->closed);
	pFiler->writeBool(DB_IMP_SPLINE(this->m_pImpl)->periodic);
	pFiler->writeVector3d(DB_IMP_SPLINE(this->m_pImpl)->normal);
	pFiler->writeDouble(DB_IMP_SPLINE(this->m_pImpl)->fitTolerance);
	pFiler->writeVector3d(DB_IMP_SPLINE(this->m_pImpl)->startTangent);
	pFiler->writeVector3d(DB_IMP_SPLINE(this->m_pImpl)->endTangent);

	// 写入控制点
	pFiler->writeUInt32(DB_IMP_SPLINE(this->m_pImpl)->controlPoints.length());
	for (int i = 0; i < DB_IMP_SPLINE(this->m_pImpl)->controlPoints.length(); i++) {
		pFiler->writePoint3d(DB_IMP_SPLINE(this->m_pImpl)->controlPoints[i]);
	}

	// 写入节点向量
	pFiler->writeUInt32(DB_IMP_SPLINE(this->m_pImpl)->knots.length());
	for (int i = 0; i < DB_IMP_SPLINE(this->m_pImpl)->knots.length(); i++) {
		pFiler->writeDouble(DB_IMP_SPLINE(this->m_pImpl)->knots[i]);
	}

	// 写入权重
	pFiler->writeUInt32(DB_IMP_SPLINE(this->m_pImpl)->weights.length());
	for (int i = 0; i < DB_IMP_SPLINE(this->m_pImpl)->weights.length(); i++) {
		pFiler->writeDouble(DB_IMP_SPLINE(this->m_pImpl)->weights[i]);
	}

	// 写入拟合点
	pFiler->writeUInt32(DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length());
	for (int i = 0; i < DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length(); i++) {
		pFiler->writePoint3d(DB_IMP_SPLINE(this->m_pImpl)->fitPoints[i]);
	}

	return Acad::ErrorStatus::eOk;
}
bool DbSpline::subWorldDraw(GiWorldDraw* pWd) const {
	return false;
}


// --- 属性访问 ---

bool DbSpline::isRational() const {
	return DB_IMP_SPLINE(this->m_pImpl)->rational;
}
int DbSpline::degree() const {
	return DB_IMP_SPLINE(this->m_pImpl)->degree;
}

int DbSpline::numControlPoints() const {
	return DB_IMP_SPLINE(this->m_pImpl)->controlPoints.length();
}
Acad::ErrorStatus DbSpline::getControlPointAt(int index, GePoint3d& point) const {
	if (index < 0 || index >= DB_IMP_SPLINE(this->m_pImpl)->controlPoints.length()) {
		return Acad::ErrorStatus::eInvalidIndex;
	}
	point = DB_IMP_SPLINE(this->m_pImpl)->controlPoints[index];
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::setControlPointAt(int index, const GePoint3d& point) {
	if (index < 0 || index >= DB_IMP_SPLINE(this->m_pImpl)->controlPoints.length()) {
		return Acad::ErrorStatus::eInvalidIndex;
	}
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SPLINE(this->m_pImpl)->controlPoints[index] = point;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

int DbSpline::numFitPoints() const {
	return DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length();
}
Acad::ErrorStatus DbSpline::getFitPointAt(int index, GePoint3d& point) const {
	if (index < 0 || index >= DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length()) {
		return Acad::ErrorStatus::eInvalidIndex;
	}
	point = DB_IMP_SPLINE(this->m_pImpl)->fitPoints[index];
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::setFitPointAt(int index, const GePoint3d& point) {
	if (index < 0 || index >= DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length()) {
		return Acad::ErrorStatus::eInvalidIndex;
	}
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SPLINE(this->m_pImpl)->fitPoints[index] = point;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}
Acad::ErrorStatus DbSpline::insertFitPointAt(int index, const GePoint3d& point) {
	if (index < 0 || index > DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length()) {
		return Acad::ErrorStatus::eInvalidIndex;
	}
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SPLINE(this->m_pImpl)->fitPoints.insert(index, point);
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}
Acad::ErrorStatus DbSpline::removeFitPointAt(int index) {
	if (index < 0 || index >= DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length()) {
		return Acad::ErrorStatus::eInvalidIndex;
	}
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SPLINE(this->m_pImpl)->fitPoints.removeAt(index);
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

double DbSpline::fitTolerance() const {
	return DB_IMP_SPLINE(this->m_pImpl)->fitTolerance;
}
Acad::ErrorStatus DbSpline::setFitTol(double fitTolerance) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SPLINE(this->m_pImpl)->fitTolerance = fitTolerance;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

Acad::ErrorStatus DbSpline::getFitTangents(GeVector3d& startTangent, GeVector3d& endTangent) const {
	startTangent = DB_IMP_SPLINE(this->m_pImpl)->startTangent;
	endTangent = DB_IMP_SPLINE(this->m_pImpl)->endTangent;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::setFitTangents(const GeVector3d& startTangent, const GeVector3d& endTangent) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SPLINE(this->m_pImpl)->startTangent = startTangent;
		DB_IMP_SPLINE(this->m_pImpl)->endTangent = endTangent;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

bool DbSpline::hasFitData() const {
	return DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length() > 0;
}

Acad::ErrorStatus DbSpline::getNurbsData(
	int& degree, bool& rational, bool& closed, bool& periodic,
	GePoint3dArray& controlPoints, GeDoubleArray& knots,
	GeDoubleArray& weights, double& controlPtTol, double& knotTol) const {
	degree = DB_IMP_SPLINE(this->m_pImpl)->degree;
	rational = DB_IMP_SPLINE(this->m_pImpl)->rational;
	closed = DB_IMP_SPLINE(this->m_pImpl)->closed;
	periodic = DB_IMP_SPLINE(this->m_pImpl)->periodic;
	controlPoints = DB_IMP_SPLINE(this->m_pImpl)->controlPoints;
	knots = DB_IMP_SPLINE(this->m_pImpl)->knots;
	weights = DB_IMP_SPLINE(this->m_pImpl)->weights;
	controlPtTol = DB_IMP_SPLINE(this->m_pImpl)->fitTolerance;
	knotTol = DB_IMP_SPLINE(this->m_pImpl)->fitTolerance;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::setNurbsData(
	int degree, bool rational, bool closed, bool periodic,
	const GePoint3dArray& controlPoints, const GeDoubleArray& knots,
	const GeDoubleArray& weights, double controlPtTol, double knotTol) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SPLINE(this->m_pImpl)->degree = degree;
		DB_IMP_SPLINE(this->m_pImpl)->rational = rational;
		DB_IMP_SPLINE(this->m_pImpl)->closed = closed;
		DB_IMP_SPLINE(this->m_pImpl)->periodic = periodic;
		DB_IMP_SPLINE(this->m_pImpl)->controlPoints = controlPoints;
		DB_IMP_SPLINE(this->m_pImpl)->knots = knots;
		DB_IMP_SPLINE(this->m_pImpl)->weights = weights;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

double DbSpline::weightAt(int index) const {
	if (index < 0 || index >= DB_IMP_SPLINE(this->m_pImpl)->weights.length()) {
		return 0.0;
	}
	return DB_IMP_SPLINE(this->m_pImpl)->weights[index];
}
Acad::ErrorStatus DbSpline::setWeightAt(int index, double weight) {
	if (index < 0 || index >= DB_IMP_SPLINE(this->m_pImpl)->weights.length()) {
		return Acad::ErrorStatus::eInvalidIndex;
	}
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SPLINE(this->m_pImpl)->weights[index] = weight;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

Adesk::Boolean DbSpline::isNull() const {
	if (DB_IMP_SPLINE(this->m_pImpl)->controlPoints.length() == 0 &&
		DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length() == 0) {
		return true;
	}
	return false;
}


// --- DbCurve 虚函数实现 ---

bool DbSpline::isClosed() const {
	return DB_IMP_SPLINE(this->m_pImpl)->closed;
}
bool DbSpline::isPeriodic() const {
	return DB_IMP_SPLINE(this->m_pImpl)->periodic;
}
bool DbSpline::isPlanar() const { return true; }
Acad::ErrorStatus DbSpline::getPlane(GePlane& plane, Db::Planarity& planarity) const {
	planarity = Db::Planarity::kPlanar;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getStartParam(double& _v) const {
	if (DB_IMP_SPLINE(this->m_pImpl)->knots.length() > 0) {
		_v = DB_IMP_SPLINE(this->m_pImpl)->knots[0];
		return Acad::ErrorStatus::eOk;
	}
	_v = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getEndParam(double& _v) const {
	if (DB_IMP_SPLINE(this->m_pImpl)->knots.length() > 0) {
		_v = DB_IMP_SPLINE(this->m_pImpl)->knots[DB_IMP_SPLINE(this->m_pImpl)->knots.length() - 1];
		return Acad::ErrorStatus::eOk;
	}
	_v = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getStartPoint(GePoint3d& point) const {
	if (DB_IMP_SPLINE(this->m_pImpl)->controlPoints.length() > 0) {
		point = DB_IMP_SPLINE(this->m_pImpl)->controlPoints[0];
		return Acad::ErrorStatus::eOk;
	}
	if (DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length() > 0) {
		point = DB_IMP_SPLINE(this->m_pImpl)->fitPoints[0];
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbSpline::getEndPoint(GePoint3d& point) const {
	int numCtrl = DB_IMP_SPLINE(this->m_pImpl)->controlPoints.length();
	if (numCtrl > 0) {
		point = DB_IMP_SPLINE(this->m_pImpl)->controlPoints[numCtrl - 1];
		return Acad::ErrorStatus::eOk;
	}
	int numFit = DB_IMP_SPLINE(this->m_pImpl)->fitPoints.length();
	if (numFit > 0) {
		point = DB_IMP_SPLINE(this->m_pImpl)->fitPoints[numFit - 1];
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbSpline::getPointAtParam(double, GePoint3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getParamAtPoint(const GePoint3d&, double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getDistAtParam(double, double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getParamAtDist(double, double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getDistAtPoint(const GePoint3d&, double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getPointAtDist(double, GePoint3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getFirstDeriv(double, GeVector3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getFirstDeriv(const GePoint3d&, GeVector3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getSecondDeriv(double, GeVector3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getSecondDeriv(const GePoint3d&, GeVector3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getClosestPointTo(const GePoint3d&, GePoint3d&, bool) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getClosestPointTo(const GePoint3d&, const GeVector3d&, GePoint3d&, bool) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getOrthoProjectedCurve(const GePlane&, DbCurve*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getProjectedCurve(const GePlane&, const GeVector3d&, DbCurve*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getOffsetCurves(double, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getSpline(DbSpline*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getSplitCurves(const GeDoubleArray&, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::extend(double) { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::extend(bool, const GePoint3d&) { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getArea(double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::reverseCurve() { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol) { return Acad::ErrorStatus::eFail; }

Acad::ErrorStatus DbSpline::subGetGeomExtents(DbExtents& extents) const { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbSpline::subTransformBy(const GeMatrix3d& xform) { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbSpline::subGetGripPoints(DbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize, const GeVector3d& curViewDir, const int bitflags) const { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbSpline::subGetOsnapPoints(Db::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const GePoint3d& pickPoint, const GePoint3d& lastPoint, const GeMatrix3d& viewXform, GePoint3dArray& snapPoints, DbIntArray& geomIds) const { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbSpline::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbSpline::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbSpline::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {
	if (pEnt == NULL) { return Acad::ErrorStatus::eFail; }
	return Acad::ErrorStatus::eOk;
}

