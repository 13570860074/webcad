#include "DbLeader.h"
#include "GePlane.h"
#include "DbImpl.h"


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
	return false;
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
	int idx = (int)param;
	if (idx < 0 || idx >= DB_IMP_LEADER(this->m_pImpl)->vertices.length()) {
		return Acad::ErrorStatus::eFail;
	}
	point = DB_IMP_LEADER(this->m_pImpl)->vertices[idx];
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::getParamAtPoint(const GePoint3d&, double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getDistAtParam(double, double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getParamAtDist(double, double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getDistAtPoint(const GePoint3d&, double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getPointAtDist(double, GePoint3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getFirstDeriv(double, GeVector3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getFirstDeriv(const GePoint3d&, GeVector3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getSecondDeriv(double, GeVector3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getSecondDeriv(const GePoint3d&, GeVector3d&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getClosestPointTo(const GePoint3d&, GePoint3d&, bool) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getClosestPointTo(const GePoint3d&, const GeVector3d&, GePoint3d&, bool) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getOrthoProjectedCurve(const GePlane&, DbCurve*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getProjectedCurve(const GePlane&, const GeVector3d&, DbCurve*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getOffsetCurves(double, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getSpline(DbSpline*&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getSplitCurves(const GeDoubleArray&, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::extend(double) { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::extend(bool, const GePoint3d&) { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getArea(double&) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::reverseCurve() { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const { return Acad::ErrorStatus::eFail; }
Acad::ErrorStatus DbLeader::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol) { return Acad::ErrorStatus::eFail; }

Acad::ErrorStatus DbLeader::subGetGeomExtents(DbExtents& extents) const { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbLeader::subTransformBy(const GeMatrix3d& xform) { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbLeader::subGetGripPoints(DbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize, const GeVector3d& curViewDir, const int bitflags) const { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbLeader::subGetOsnapPoints(Db::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const GePoint3d& pickPoint, const GePoint3d& lastPoint, const GeMatrix3d& viewXform, GePoint3dArray& snapPoints, DbIntArray& geomIds) const { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbLeader::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbLeader::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbLeader::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {
	if (pEnt == NULL) { return Acad::ErrorStatus::eFail; }
	return Acad::ErrorStatus::eOk;
}

