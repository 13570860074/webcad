#include "DbLeader.h"
#include "GePlane.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>


DbLeader::DbLeader() {
	this->m_pImpl = new DbLeaderImpl();
}
DbLeader::~DbLeader() {

}


Acad::ErrorStatus DbLeader::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	DB_IMP_LEADER(this->m_pImpl)->normal = normal;

	bool hasArrowHead;
	pFiler->readBool(&hasArrowHead);
	DB_IMP_LEADER(this->m_pImpl)->hasArrowHead = hasArrowHead;

	bool isSplined;
	pFiler->readBool(&isSplined);
	DB_IMP_LEADER(this->m_pImpl)->isSplined = isSplined;

	int annoType;
	pFiler->readInt32(&annoType);
	DB_IMP_LEADER(this->m_pImpl)->annoType = annoType;

	GeVector3d annotationOffset;
	pFiler->readVector3d(&annotationOffset);
	DB_IMP_LEADER(this->m_pImpl)->annotationOffset = annotationOffset;

	// 读取顶点
	DB_IMP_LEADER(this->m_pImpl)->vertices.removeAll();
	unsigned int numVerts = 0;
	pFiler->readUInt32(&numVerts);
	for (unsigned int i = 0; i < numVerts; i++) {
		GePoint3d pt;
		pFiler->readPoint3d(&pt);
		DB_IMP_LEADER(this->m_pImpl)->vertices.append(pt);
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeVector3d(DB_IMP_LEADER(this->m_pImpl)->normal);
	pFiler->writeBool(DB_IMP_LEADER(this->m_pImpl)->hasArrowHead);
	pFiler->writeBool(DB_IMP_LEADER(this->m_pImpl)->isSplined);
	pFiler->writeInt32(DB_IMP_LEADER(this->m_pImpl)->annoType);
	pFiler->writeVector3d(DB_IMP_LEADER(this->m_pImpl)->annotationOffset);

	// 写入顶点
	pFiler->writeUInt32(DB_IMP_LEADER(this->m_pImpl)->vertices.length());
	for (int i = 0; i < DB_IMP_LEADER(this->m_pImpl)->vertices.length(); i++) {
		pFiler->writePoint3d(DB_IMP_LEADER(this->m_pImpl)->vertices[i]);
	}

	return Acad::ErrorStatus::eOk;
}
bool DbLeader::subWorldDraw(GiWorldDraw* pWd) const {
	int nVerts = this->numVertices();
	if (nVerts < 2) {
		return true;
	}

	// 绘制引线折线
	const AcArray<GePoint3d>& vertices = DB_IMP_LEADER(this->m_pImpl)->vertices;
	pWd->geometry().polyline(nVerts, &vertices[0]);

	// 绘制箭头
	if (this->hasArrowHead() && nVerts >= 2) {
		GePoint3d tip = vertices[0];
		GeVector3d dir = vertices[1] - vertices[0];
		double len = dir.length();
		if (len > 1e-10) {
			dir.normalize();
			double arrowSize = len * 0.05;
			if (arrowSize > 2.5) arrowSize = 2.5;
			if (arrowSize < 0.5) arrowSize = 0.5;

			// 计算箭头两侧点
			GeVector3d normal = DB_IMP_LEADER(this->m_pImpl)->normal;
			GeVector3d perp = dir.crossProduct(normal);
			if (perp.length() < 1e-10) {
				perp = dir.crossProduct(GeVector3d::kZAxis);
			}
			if (perp.length() > 1e-10) {
				perp.normalize();
				GePoint3d arrowPts[3];
				arrowPts[0] = tip;
				arrowPts[1] = tip + dir * arrowSize + perp * (arrowSize * 0.3);
				arrowPts[2] = tip + dir * arrowSize - perp * (arrowSize * 0.3);
				pWd->geometry().polygon(3, arrowPts);
			}
		}
	}

	return true;
}


// --- 属性访问 ---

GeVector3d DbLeader::normal() const {
	return DB_IMP_LEADER(this->m_pImpl)->normal;
}

int DbLeader::numVertices() const {
	return DB_IMP_LEADER(this->m_pImpl)->vertices.length();
}
bool DbLeader::appendVertex(const GePoint3d& vertex) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->vertices.append(vertex);
		return true;
	}
	return false;
}
void DbLeader::removeLastVertex() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
		if (n > 0) {
			DB_IMP_LEADER(this->m_pImpl)->vertices.removeAt(n - 1);
		}
	}
}
GePoint3d DbLeader::firstVertex() const {
	if (DB_IMP_LEADER(this->m_pImpl)->vertices.length() > 0) {
		return DB_IMP_LEADER(this->m_pImpl)->vertices[0];
	}
	return GePoint3d::kOrigin;
}
GePoint3d DbLeader::lastVertex() const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	if (n > 0) {
		return DB_IMP_LEADER(this->m_pImpl)->vertices[n - 1];
	}
	return GePoint3d::kOrigin;
}
GePoint3d DbLeader::vertexAt(int index) const {
	if (index >= 0 && index < DB_IMP_LEADER(this->m_pImpl)->vertices.length()) {
		return DB_IMP_LEADER(this->m_pImpl)->vertices[index];
	}
	return GePoint3d::kOrigin;
}
bool DbLeader::setVertexAt(int index, const GePoint3d& vertex) {
	if (index < 0 || index >= DB_IMP_LEADER(this->m_pImpl)->vertices.length()) {
		return false;
	}
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->vertices[index] = vertex;
		return true;
	}
	return false;
}

bool DbLeader::hasArrowHead() const {
	return DB_IMP_LEADER(this->m_pImpl)->hasArrowHead;
}
void DbLeader::enableArrowHead() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->hasArrowHead = true;
	}
}
void DbLeader::disableArrowHead() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->hasArrowHead = false;
	}
}

bool DbLeader::isSplined() const {
	return DB_IMP_LEADER(this->m_pImpl)->isSplined;
}
void DbLeader::setToSplineLeader() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->isSplined = true;
	}
}
void DbLeader::setToStraightLeader() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->isSplined = false;
	}
}

DbObjectId DbLeader::dimensionStyle() const {
	return DB_IMP_LEADER(this->m_pImpl)->dimStyleId;
}
Acad::ErrorStatus DbLeader::setDimensionStyle(const DbObjectId& dimStyleId) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->dimStyleId = dimStyleId;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

DbObjectId DbLeader::annotationObjId() const {
	return DB_IMP_LEADER(this->m_pImpl)->annotationId;
}
Acad::ErrorStatus DbLeader::attachAnnotation(const DbObjectId& annoId) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->annotationId = annoId;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}
Acad::ErrorStatus DbLeader::detachAnnotation() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->annotationId = DbObjectId();
		DB_IMP_LEADER(this->m_pImpl)->annoType = DbLeader::kNoAnno;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}
GeVector3d DbLeader::annotationOffset() const {
	return DB_IMP_LEADER(this->m_pImpl)->annotationOffset;
}
Acad::ErrorStatus DbLeader::setAnnotationOffset(const GeVector3d& offset) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_LEADER(this->m_pImpl)->annotationOffset = offset;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

DbLeader::AnnoType DbLeader::annoType() const {
	return static_cast<AnnoType>(DB_IMP_LEADER(this->m_pImpl)->annoType);
}


// --- DbCurve 虚函数实现 ---

bool DbLeader::isClosed() const { return false; }
bool DbLeader::isPeriodic() const { return false; }
bool DbLeader::isPlanar() const { return true; }
Acad::ErrorStatus DbLeader::getPlane(GePlane& plane, Db::Planarity& planarity) const {
	planarity = Db::Planarity::kPlanar;
	GeVector3d norm = DB_IMP_LEADER(this->m_pImpl)->normal;
	if (DB_IMP_LEADER(this->m_pImpl)->vertices.length() > 0) {
		plane.set(DB_IMP_LEADER(this->m_pImpl)->vertices[0], norm);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getStartParam(double& _v) const {
	_v = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getEndParam(double& _v) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	_v = (n > 1) ? (double)(n - 1) : 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getStartPoint(GePoint3d& point) const {
	if (DB_IMP_LEADER(this->m_pImpl)->vertices.length() > 0) {
		point = DB_IMP_LEADER(this->m_pImpl)->vertices[0];
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLeader::getEndPoint(GePoint3d& point) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	if (n > 0) {
		point = DB_IMP_LEADER(this->m_pImpl)->vertices[n - 1];
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbLeader::getPointAtParam(double param, GePoint3d& point) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	if (n == 0) return Acad::ErrorStatus::eFail;
	int idx = (int)param;
	double frac = param - (double)idx;
	if (idx < 0) { point = DB_IMP_LEADER(this->m_pImpl)->vertices[0]; return Acad::ErrorStatus::eOk; }
	if (idx >= n - 1) { point = DB_IMP_LEADER(this->m_pImpl)->vertices[n - 1]; return Acad::ErrorStatus::eOk; }
	GePoint3d p0 = DB_IMP_LEADER(this->m_pImpl)->vertices[idx];
	GePoint3d p1 = DB_IMP_LEADER(this->m_pImpl)->vertices[idx + 1];
	point = p0 + (p1 - p0) * frac;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getParamAtPoint(const GePoint3d& point, double& param) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	if (n < 2) return Acad::ErrorStatus::eFail;
	double minDist = 1e300;
	for (int i = 0; i < n - 1; i++) {
		GePoint3d p0 = DB_IMP_LEADER(this->m_pImpl)->vertices[i];
		GePoint3d p1 = DB_IMP_LEADER(this->m_pImpl)->vertices[i + 1];
		GeVector3d seg = p1 - p0;
		double len = seg.length();
		if (len < 1e-10) continue;
		double t = (point - p0).dotProduct(seg) / (len * len);
		if (t < 0.0) t = 0.0;
		if (t > 1.0) t = 1.0;
		GePoint3d proj = p0 + seg * t;
		double d = point.distanceTo(proj);
		if (d < minDist) { minDist = d; param = (double)i + t; }
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getDistAtParam(double param, double& dist) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	if (n < 2) return Acad::ErrorStatus::eFail;
	dist = 0.0;
	int seg = (int)param;
	double frac = param - (double)seg;
	for (int i = 0; i < seg && i < n - 1; i++) {
		dist += DB_IMP_LEADER(this->m_pImpl)->vertices[i].distanceTo(DB_IMP_LEADER(this->m_pImpl)->vertices[i + 1]);
	}
	if (seg >= 0 && seg < n - 1) {
		dist += DB_IMP_LEADER(this->m_pImpl)->vertices[seg].distanceTo(DB_IMP_LEADER(this->m_pImpl)->vertices[seg + 1]) * frac;
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getParamAtDist(double targetDist, double& param) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	if (n < 2) return Acad::ErrorStatus::eFail;
	double accum = 0.0;
	for (int i = 0; i < n - 1; i++) {
		double segLen = DB_IMP_LEADER(this->m_pImpl)->vertices[i].distanceTo(DB_IMP_LEADER(this->m_pImpl)->vertices[i + 1]);
		if (accum + segLen >= targetDist) {
			double remainder = targetDist - accum;
			param = (double)i + (segLen > 1e-10 ? remainder / segLen : 0.0);
			return Acad::ErrorStatus::eOk;
		}
		accum += segLen;
	}
	param = (double)(n - 1);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getDistAtPoint(const GePoint3d& point, double& dist) const {
	double param;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::eOk) return es;
	return this->getDistAtParam(param, dist);
}
Acad::ErrorStatus DbLeader::getPointAtDist(double dist, GePoint3d& point) const {
	double param;
	Acad::ErrorStatus es = this->getParamAtDist(dist, param);
	if (es != Acad::eOk) return es;
	return this->getPointAtParam(param, point);
}
Acad::ErrorStatus DbLeader::getFirstDeriv(double param, GeVector3d& firstDeriv) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	if (n < 2) return Acad::ErrorStatus::eFail;
	int idx = (int)param;
	if (idx < 0) idx = 0;
	if (idx >= n - 1) idx = n - 2;
	firstDeriv = DB_IMP_LEADER(this->m_pImpl)->vertices[idx + 1] - DB_IMP_LEADER(this->m_pImpl)->vertices[idx];
	firstDeriv.normalize();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getFirstDeriv(const GePoint3d& point, GeVector3d& firstDeriv) const {
	double param;
	Acad::ErrorStatus es = this->getParamAtPoint(point, param);
	if (es != Acad::eOk) return es;
	return this->getFirstDeriv(param, firstDeriv);
}
Acad::ErrorStatus DbLeader::getSecondDeriv(double param, GeVector3d& secDeriv) const {
	secDeriv = GeVector3d(0, 0, 0);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getSecondDeriv(const GePoint3d& point, GeVector3d& secDeriv) const {
	secDeriv = GeVector3d(0, 0, 0);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getClosestPointTo(const GePoint3d& point, GePoint3d& closest, bool extend) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	if (n == 0) return Acad::ErrorStatus::eFail;
	if (n == 1) { closest = DB_IMP_LEADER(this->m_pImpl)->vertices[0]; return Acad::ErrorStatus::eOk; }
	double minDist = 1e300;
	for (int i = 0; i < n - 1; i++) {
		GePoint3d p0 = DB_IMP_LEADER(this->m_pImpl)->vertices[i];
		GePoint3d p1 = DB_IMP_LEADER(this->m_pImpl)->vertices[i + 1];
		GeVector3d seg = p1 - p0;
		double len = seg.length();
		if (len < 1e-10) continue;
		double t = (point - p0).dotProduct(seg) / (len * len);
		if (t < 0.0) t = 0.0;
		if (t > 1.0) t = 1.0;
		GePoint3d proj = p0 + seg * t;
		double d = point.distanceTo(proj);
		if (d < minDist) { minDist = d; closest = proj; }
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getClosestPointTo(const GePoint3d& point, const GeVector3d& projDir, GePoint3d& closest, bool extend) const {
	return this->getClosestPointTo(point, closest, extend);
}
Acad::ErrorStatus DbLeader::getOrthoProjectedCurve(const GePlane&, DbCurve*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getProjectedCurve(const GePlane&, const GeVector3d&, DbCurve*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getOffsetCurves(double, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getSpline(DbSpline*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getSplitCurves(const GeDoubleArray&, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::extend(double) { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::extend(bool, const GePoint3d&) { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getArea(double& area) const { area = 0.0; return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbLeader::reverseCurve() {
	DbLeaderImpl* imp = DB_IMP_LEADER(this->m_pImpl);
	int n = imp->vertices.length();
	for (int i = 0; i < n / 2; i++) {
		GePoint3d tmp = imp->vertices[i];
		imp->vertices[i] = imp->vertices[n - 1 - i];
		imp->vertices[n - 1 - i] = tmp;
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol) { return Acad::ErrorStatus::eFail; }

Acad::ErrorStatus DbLeader::subGetGeomExtents(DbExtents& extents) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	for (int i = 0; i < n; i++) {
		extents.addPoint(DB_IMP_LEADER(this->m_pImpl)->vertices[i]);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::subTransformBy(const GeMatrix3d& xform) {
	DbLeaderImpl* imp = DB_IMP_LEADER(this->m_pImpl);
	for (int i = 0; i < imp->vertices.length(); i++) {
		imp->vertices[i].transformBy(xform);
	}
	imp->normal.transformBy(xform);
	imp->normal.normalize();
	imp->annotationOffset.transformBy(xform);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::subGetGripPoints(DbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize, const GeVector3d& curViewDir, const int bitflags) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	for (int i = 0; i < n; i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(DB_IMP_LEADER(this->m_pImpl)->vertices[i]);
		grips.append(grip);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::subGetOsnapPoints(Db::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const GePoint3d& pickPoint, const GePoint3d& lastPoint, const GeMatrix3d& viewXform, GePoint3dArray& snapPoints, DbIntArray& geomIds) const {
	int n = DB_IMP_LEADER(this->m_pImpl)->vertices.length();
	if (osnapMode == Db::kOsModeEnd) {
		for (int i = 0; i < n; i++) {
			snapPoints.append(DB_IMP_LEADER(this->m_pImpl)->vertices[i]);
		}
	} else if (osnapMode == Db::kOsModeNear) {
		GePoint3d closest;
		if (this->getClosestPointTo(pickPoint, closest, false) == Acad::eOk) {
			snapPoints.append(closest);
		}
	} else if (osnapMode == Db::kOsModeMid && n >= 2) {
		for (int i = 0; i < n - 1; i++) {
			GePoint3d mid = DB_IMP_LEADER(this->m_pImpl)->vertices[i] +
				(DB_IMP_LEADER(this->m_pImpl)->vertices[i + 1] - DB_IMP_LEADER(this->m_pImpl)->vertices[i]) * 0.5;
			snapPoints.append(mid);
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	DbLeaderImpl* imp = DB_IMP_LEADER(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		int idx = indices[i];
		if (idx >= 0 && idx < imp->vertices.length()) {
			imp->vertices[idx] += offset;
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {
	if (pEnt == NULL) { return Acad::ErrorStatus::eFail; }
	return Acad::ErrorStatus::eOk;
}

