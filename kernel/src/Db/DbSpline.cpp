#include "DbSpline.h"
#include "GePlane.h"
#include "DbImpl.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include <cmath>


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
namespace
{
	// De Boor算法：查找参数t所在的节点区间索引
	int findKnotSpan(int n, int degree, double t, const GeDoubleArray& knots)
	{
		if (t >= knots[n]) {
			return n - 1;
		}
		if (t <= knots[degree]) {
			return degree;
		}
		int low = degree;
		int high = n;
		int mid = (low + high) / 2;
		while (t < knots[mid] || t >= knots[mid + 1]) {
			if (t < knots[mid]) {
				high = mid;
			} else {
				low = mid;
			}
			mid = (low + high) / 2;
		}
		return mid;
	}

	// De Boor算法：计算B样条/NURBS曲线上参数t处的点
	GePoint3d deBoorEval(int degree, const GeDoubleArray& knots,
		const GePoint3dArray& ctrlPts, const GeDoubleArray& weights,
		bool rational, double t)
	{
		int n = ctrlPts.length();
		int span = findKnotSpan(n, degree, t, knots);

		// 初始化De Boor点
		GePoint3d d[16]; // degree最大支持15阶
		double w[16];
		for (int j = 0; j <= degree; j++) {
			int idx = span - degree + j;
			if (idx < 0) idx = 0;
			if (idx >= n) idx = n - 1;
			d[j] = ctrlPts[idx];
			w[j] = (rational && idx < weights.length()) ? weights[idx] : 1.0;
			if (rational) {
				// 转换为齐次坐标
				d[j] = GePoint3d(d[j].x * w[j], d[j].y * w[j], d[j].z * w[j]);
			}
		}

		// De Boor递推
		for (int r = 1; r <= degree; r++) {
			for (int j = degree; j >= r; j--) {
				int idx = span - degree + j;
				double denom = knots[idx + degree + 1 - r] - knots[idx];
				double alpha = 0.0;
				if (std::fabs(denom) > 1e-14) {
					alpha = (t - knots[idx]) / denom;
				}
				GeVector3d v = d[j] - d[j - 1];
				d[j] = d[j - 1] + v * alpha;
				w[j] = w[j - 1] * (1.0 - alpha) + w[j] * alpha;
			}
		}

		if (rational && std::fabs(w[degree]) > 1e-14) {
			return GePoint3d(d[degree].x / w[degree], d[degree].y / w[degree], d[degree].z / w[degree]);
		}
		return d[degree];
	}
}

bool DbSpline::subWorldDraw(GiWorldDraw* pWd) const {
	int nCtrlPts = this->numControlPoints();
	if (nCtrlPts < 2) {
		return true;
	}

	int deg = this->degree();
	bool rational = this->isRational();
	const GePoint3dArray& ctrlPts = DB_IMP_SPLINE(this->m_pImpl)->controlPoints;
	const GeDoubleArray& knots = DB_IMP_SPLINE(this->m_pImpl)->knots;
	const GeDoubleArray& weights = DB_IMP_SPLINE(this->m_pImpl)->weights;

	// 检查节点向量有效性
	if (knots.length() < nCtrlPts + deg + 1) {
		// 节点向量不足，退化为控制点折线
		pWd->geometry().polyline(nCtrlPts, &ctrlPts[0]);
		return true;
	}

	// 参数范围
	double tStart = knots[deg];
	double tEnd = knots[nCtrlPts];
	if (tEnd <= tStart) {
		return true;
	}

	// 自适应采样数
	int numSamples = nCtrlPts * 8;
	if (numSamples < 50) numSamples = 50;
	if (numSamples > 500) numSamples = 500;

	GePoint3dArray pts;
	for (int i = 0; i <= numSamples; i++) {
		double t = tStart + (tEnd - tStart) * i / numSamples;
		if (i == numSamples) t = tEnd - 1e-10; // 避免越界
		GePoint3d pt = deBoorEval(deg, knots, ctrlPts, weights, rational, t);
		pts.append(pt);
	}

	pWd->geometry().polyline(pts.length(), &pts[0]);
	return true;
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

Acad::ErrorStatus DbSpline::getPointAtParam(double param, GePoint3d& point) const {
	int nCtrl = this->numControlPoints();
	if (nCtrl < 2) return Acad::ErrorStatus::eFail;
	int deg = this->degree();
	const GePoint3dArray& ctrlPts = DB_IMP_SPLINE(this->m_pImpl)->controlPoints;
	const GeDoubleArray& knots = DB_IMP_SPLINE(this->m_pImpl)->knots;
	const GeDoubleArray& weights = DB_IMP_SPLINE(this->m_pImpl)->weights;
	if (knots.length() < nCtrl + deg + 1) return Acad::ErrorStatus::eFail;
	double t = param;
	double tEnd = knots[nCtrl];
	if (t > tEnd - 1e-10) t = tEnd - 1e-10;
	point = deBoorEval(deg, knots, ctrlPts, weights, this->isRational(), t);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getParamAtPoint(const GePoint3d& point, double& param) const {
	int nCtrl = this->numControlPoints();
	if (nCtrl < 2) return Acad::ErrorStatus::eFail;
	int deg = this->degree();
	const GeDoubleArray& knots = DB_IMP_SPLINE(this->m_pImpl)->knots;
	if (knots.length() < nCtrl + deg + 1) return Acad::ErrorStatus::eFail;
	double tStart = knots[deg];
	double tEnd = knots[nCtrl];
	// 采样找最近参数
	int N = nCtrl * 16;
	if (N < 100) N = 100;
	double bestDist = 1e30;
	double bestT = tStart;
	for (int i = 0; i <= N; i++) {
		double t = tStart + (tEnd - tStart) * i / N;
		if (t > tEnd - 1e-10) t = tEnd - 1e-10;
		GePoint3d pt;
		this->getPointAtParam(t, pt);
		double d = (pt - point).length();
		if (d < bestDist) { bestDist = d; bestT = t; }
	}
	param = bestT;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getDistAtParam(double param, double& dist) const {
	int nCtrl = this->numControlPoints();
	if (nCtrl < 2) return Acad::ErrorStatus::eFail;
	int deg = this->degree();
	const GeDoubleArray& knots = DB_IMP_SPLINE(this->m_pImpl)->knots;
	if (knots.length() < nCtrl + deg + 1) return Acad::ErrorStatus::eFail;
	double tStart = knots[deg];
	// 数值积分弧长: Gauss-Legendre或简单梯形法
	int N = 200;
	double dt = (param - tStart) / N;
	dist = 0.0;
	GePoint3d prev;
	this->getPointAtParam(tStart, prev);
	for (int i = 1; i <= N; i++) {
		double t = tStart + dt * i;
		GePoint3d cur;
		this->getPointAtParam(t, cur);
		dist += (cur - prev).length();
		prev = cur;
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getParamAtDist(double dist, double& param) const {
	int nCtrl = this->numControlPoints();
	if (nCtrl < 2) return Acad::ErrorStatus::eFail;
	int deg = this->degree();
	const GeDoubleArray& knots = DB_IMP_SPLINE(this->m_pImpl)->knots;
	if (knots.length() < nCtrl + deg + 1) return Acad::ErrorStatus::eFail;
	double tStart = knots[deg];
	double tEnd = knots[nCtrl];
	// 二分法查找
	double lo = tStart, hi = tEnd;
	for (int iter = 0; iter < 50; iter++) {
		double mid = (lo + hi) * 0.5;
		double d;
		this->getDistAtParam(mid, d);
		if (d < dist) lo = mid;
		else hi = mid;
	}
	param = (lo + hi) * 0.5;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getDistAtPoint(const GePoint3d& point, double& dist) const {
	double param;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::eOk) return es;
	return this->getDistAtParam(param, dist);
}
Acad::ErrorStatus DbSpline::getPointAtDist(double dist, GePoint3d& point) const {
	double param;
	Acad::ErrorStatus es = this->getParamAtDist(dist, param);
	if (es != Acad::eOk) return es;
	return this->getPointAtParam(param, point);
}
Acad::ErrorStatus DbSpline::getFirstDeriv(double param, GeVector3d& firstDeriv) const {
	// 中心差分近似
	double h = 1e-7;
	GePoint3d p1, p2;
	Acad::ErrorStatus es1 = this->getPointAtParam(param - h, p1);
	Acad::ErrorStatus es2 = this->getPointAtParam(param + h, p2);
	if (es1 != Acad::eOk || es2 != Acad::eOk) return Acad::ErrorStatus::eFail;
	firstDeriv = (p2 - p1) * (1.0 / (2.0 * h));
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getFirstDeriv(const GePoint3d& point, GeVector3d& firstDeriv) const {
	double param;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::eOk) return es;
	return this->getFirstDeriv(param, firstDeriv);
}
Acad::ErrorStatus DbSpline::getSecondDeriv(double param, GeVector3d& secDeriv) const {
	// 中心差分二阶导
	double h = 1e-5;
	GePoint3d p0, p1, p2;
	Acad::ErrorStatus es0 = this->getPointAtParam(param - h, p0);
	Acad::ErrorStatus es1 = this->getPointAtParam(param, p1);
	Acad::ErrorStatus es2 = this->getPointAtParam(param + h, p2);
	if (es0 != Acad::eOk || es1 != Acad::eOk || es2 != Acad::eOk) return Acad::ErrorStatus::eFail;
	// f''(t) ≈ (f(t+h) - 2f(t) + f(t-h)) / h²
	secDeriv = ((p2 - p1) - (p1 - p0)) * (1.0 / (h * h));
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getSecondDeriv(const GePoint3d& point, GeVector3d& secDeriv) const {
	double param;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::eOk) return es;
	return this->getSecondDeriv(param, secDeriv);
}
Acad::ErrorStatus DbSpline::getClosestPointTo(const GePoint3d& givenPnt, GePoint3d& pointOnCurve, bool extend) const {
	int nCtrl = this->numControlPoints();
	if (nCtrl < 2) return Acad::ErrorStatus::eFail;
	int deg = this->degree();
	const GeDoubleArray& knots = DB_IMP_SPLINE(this->m_pImpl)->knots;
	if (knots.length() < nCtrl + deg + 1) return Acad::ErrorStatus::eFail;
	double tStart = knots[deg];
	double tEnd = knots[nCtrl];
	// 粗采样找最近
	int N = nCtrl * 16;
	if (N < 100) N = 100;
	double bestDist = 1e30;
	double bestT = tStart;
	for (int i = 0; i <= N; i++) {
		double t = tStart + (tEnd - tStart) * i / N;
		if (t > tEnd - 1e-10) t = tEnd - 1e-10;
		GePoint3d pt;
		this->getPointAtParam(t, pt);
		double d = (pt - givenPnt).length();
		if (d < bestDist) { bestDist = d; bestT = t; }
	}
	// Newton细化
	for (int iter = 0; iter < 20; iter++) {
		GePoint3d pt;
		this->getPointAtParam(bestT, pt);
		GeVector3d deriv;
		if (this->getFirstDeriv(bestT, deriv) != Acad::eOk) break;
		GeVector3d diff = pt - givenPnt;
		double dot = diff.dotProduct(deriv);
		double denom = deriv.dotProduct(deriv);
		if (std::fabs(denom) < 1e-20) break;
		double dt = -dot / denom;
		bestT += dt;
		if (bestT < tStart) bestT = tStart;
		if (bestT > tEnd - 1e-10) bestT = tEnd - 1e-10;
		if (std::fabs(dt) < 1e-12) break;
	}
	this->getPointAtParam(bestT, pointOnCurve);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getClosestPointTo(const GePoint3d& givenPnt, const GeVector3d& direction, GePoint3d& pointOnCurve, bool extend) const {
	return this->getClosestPointTo(givenPnt, pointOnCurve, extend);
}
Acad::ErrorStatus DbSpline::getOrthoProjectedCurve(const GePlane&, DbCurve*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getProjectedCurve(const GePlane&, const GeVector3d&, DbCurve*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getOffsetCurves(double, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getSpline(DbSpline*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getSplitCurves(const GeDoubleArray&, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::extend(double) { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::extend(bool, const GePoint3d&) { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::getArea(double& area) const {
	// Shoelace数值积分
	int nCtrl = this->numControlPoints();
	if (nCtrl < 2) return Acad::ErrorStatus::eFail;
	int deg = this->degree();
	const GeDoubleArray& knots = DB_IMP_SPLINE(this->m_pImpl)->knots;
	if (knots.length() < nCtrl + deg + 1) return Acad::ErrorStatus::eFail;
	double tStart = knots[deg];
	double tEnd = knots[nCtrl];
	int N = nCtrl * 16;
	if (N < 100) N = 100;
	area = 0.0;
	GePoint3d prev;
	this->getPointAtParam(tStart, prev);
	for (int i = 1; i <= N; i++) {
		double t = tStart + (tEnd - tStart) * i / N;
		if (t > tEnd - 1e-10) t = tEnd - 1e-10;
		GePoint3d cur;
		this->getPointAtParam(t, cur);
		area += prev.x * cur.y - cur.x * prev.y;
		prev = cur;
	}
	area = std::fabs(area) * 0.5;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::reverseCurve() {
	DbSplineImpl* imp = DB_IMP_SPLINE(this->m_pImpl);
	// 反转控制点
	int nc = imp->controlPoints.length();
	for (int i = 0; i < nc / 2; i++) {
		GePoint3d tmp = imp->controlPoints[i];
		imp->controlPoints[i] = imp->controlPoints[nc - 1 - i];
		imp->controlPoints[nc - 1 - i] = tmp;
	}
	// 反转权重
	int nw = imp->weights.length();
	for (int i = 0; i < nw / 2; i++) {
		double tmp = imp->weights[i];
		imp->weights[i] = imp->weights[nw - 1 - i];
		imp->weights[nw - 1 - i] = tmp;
	}
	// 反转节点向量
	int nk = imp->knots.length();
	if (nk > 0) {
		double kMax = imp->knots[nk - 1];
		double kMin = imp->knots[0];
		GeDoubleArray newKnots;
		for (int i = nk - 1; i >= 0; i--) {
			newKnots.append(kMax + kMin - imp->knots[i]);
		}
		imp->knots = newKnots;
	}
	// 反转拟合点
	int nf = imp->fitPoints.length();
	for (int i = 0; i < nf / 2; i++) {
		GePoint3d tmp = imp->fitPoints[i];
		imp->fitPoints[i] = imp->fitPoints[nf - 1 - i];
		imp->fitPoints[nf - 1 - i] = tmp;
	}
	// 交换起终切线
	GeVector3d tmpV = imp->startTangent;
	imp->startTangent = imp->endTangent.negate();
	imp->endTangent = tmpV.negate();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbSpline::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol) { return Acad::ErrorStatus::eFail; }

Acad::ErrorStatus DbSpline::subGetGeomExtents(DbExtents& extents) const {
	int nCtrl = this->numControlPoints();
	if (nCtrl < 2) return Acad::ErrorStatus::eOk;
	int deg = this->degree();
	const GeDoubleArray& knots = DB_IMP_SPLINE(this->m_pImpl)->knots;
	if (knots.length() < nCtrl + deg + 1) return Acad::ErrorStatus::eOk;
	double tStart = knots[deg];
	double tEnd = knots[nCtrl];
	int N = nCtrl * 8;
	if (N < 64) N = 64;
	for (int i = 0; i <= N; i++) {
		double t = tStart + (tEnd - tStart) * i / N;
		if (t > tEnd - 1e-10) t = tEnd - 1e-10;
		GePoint3d pt;
		this->getPointAtParam(t, pt);
		extents.addPoint(pt);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::subTransformBy(const GeMatrix3d& xform) {
	DbSplineImpl* imp = DB_IMP_SPLINE(this->m_pImpl);
	for (int i = 0; i < imp->controlPoints.length(); i++) {
		imp->controlPoints[i].transformBy(xform);
	}
	for (int i = 0; i < imp->fitPoints.length(); i++) {
		imp->fitPoints[i].transformBy(xform);
	}
	imp->startTangent.transformBy(xform);
	imp->endTangent.transformBy(xform);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::subGetGripPoints(DbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize, const GeVector3d& curViewDir, const int bitflags) const {
	const GePoint3dArray& ctrlPts = DB_IMP_SPLINE(this->m_pImpl)->controlPoints;
	for (int i = 0; i < ctrlPts.length(); i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(ctrlPts[i]);
		grips.append(grip);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::subGetOsnapPoints(Db::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const GePoint3d& pickPoint, const GePoint3d& lastPoint, const GeMatrix3d& viewXform, GePoint3dArray& snapPoints, DbIntArray& geomIds) const {
	switch (osnapMode) {
	case Db::OsnapMode::kOsModeEnd: {
		GePoint3d sp, ep;
		if (this->getStartPoint(sp) == Acad::eOk) snapPoints.append(sp);
		if (this->getEndPoint(ep) == Acad::eOk) snapPoints.append(ep);
		break;
	}
	case Db::OsnapMode::kOsModeNear: {
		GePoint3d pt;
		if (this->getClosestPointTo(pickPoint, pt, false) == Acad::eOk) snapPoints.append(pt);
		break;
	}
	default:
		break;
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	DbSplineImpl* imp = DB_IMP_SPLINE(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		int idx = indices[i];
		if (idx >= 0 && idx < imp->controlPoints.length()) {
			imp->controlPoints[idx] += offset;
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {
	if (pEnt == NULL) { return Acad::ErrorStatus::eFail; }
	return Acad::ErrorStatus::eOk;
}

