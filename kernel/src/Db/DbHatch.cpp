#include "DbHatch.h"
#include "GePlane.h"
#include "DbImpl.h"


DbHatch::DbHatch() {
	this->m_pImpl = new DbHatchImpl();
}
DbHatch::~DbHatch() {

}


Acad::ErrorStatus DbHatch::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	double elevation;
	pFiler->readDouble(&elevation);
	DB_IMP_HATCH(this->m_pImpl)->elevation = elevation;

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	DB_IMP_HATCH(this->m_pImpl)->normal = normal;

	int patternType;
	pFiler->readInt32(&patternType);
	DB_IMP_HATCH(this->m_pImpl)->patternType = patternType;

	AcString patternName;
	pFiler->readString(patternName);
	DB_IMP_HATCH(this->m_pImpl)->patternName = patternName;

	double patternAngle;
	pFiler->readDouble(&patternAngle);
	DB_IMP_HATCH(this->m_pImpl)->patternAngle = patternAngle;

	double patternScale;
	pFiler->readDouble(&patternScale);
	DB_IMP_HATCH(this->m_pImpl)->patternScale = patternScale;

	double patternSpace;
	pFiler->readDouble(&patternSpace);
	DB_IMP_HATCH(this->m_pImpl)->patternSpace = patternSpace;

	bool patternDouble;
	pFiler->readBool(&patternDouble);
	DB_IMP_HATCH(this->m_pImpl)->patternDouble = patternDouble;

	int hatchStyle;
	pFiler->readInt32(&hatchStyle);
	DB_IMP_HATCH(this->m_pImpl)->hatchStyle = hatchStyle;

	bool associative;
	pFiler->readBool(&associative);
	DB_IMP_HATCH(this->m_pImpl)->associative = associative;

	// 清除已有循环
	for (int i = DB_IMP_HATCH(this->m_pImpl)->loops.length() - 1; i >= 0; i--) {
		delete DB_IMP_HATCH(this->m_pImpl)->loops.at(i);
	}
	DB_IMP_HATCH(this->m_pImpl)->loops.removeAll();

	// 读取边界环
	unsigned int numLoops = 0;
	pFiler->readUInt32(&numLoops);
	for (unsigned int li = 0; li < numLoops; li++) {
		DbHatchLoopImpl* loop = new DbHatchLoopImpl();

		int loopType;
		pFiler->readInt32(&loopType);
		loop->loopType = loopType;

		unsigned int numVerts = 0;
		pFiler->readUInt32(&numVerts);
		for (unsigned int vi = 0; vi < numVerts; vi++) {
			GePoint2d pt;
			pFiler->readPoint2d(&pt);
			loop->vertices.append(pt);
		}

		unsigned int numBulges = 0;
		pFiler->readUInt32(&numBulges);
		for (unsigned int bi = 0; bi < numBulges; bi++) {
			double bulge;
			pFiler->readDouble(&bulge);
			loop->bulges.append(bulge);
		}

		DB_IMP_HATCH(this->m_pImpl)->loops.append(loop);
	}

	// 读取种子点
	DB_IMP_HATCH(this->m_pImpl)->seedPoints.removeAll();
	unsigned int numSeeds = 0;
	pFiler->readUInt32(&numSeeds);
	for (unsigned int si = 0; si < numSeeds; si++) {
		GePoint2d pt;
		pFiler->readPoint2d(&pt);
		DB_IMP_HATCH(this->m_pImpl)->seedPoints.append(pt);
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbHatch::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeDouble(DB_IMP_HATCH(this->m_pImpl)->elevation);
	pFiler->writeVector3d(DB_IMP_HATCH(this->m_pImpl)->normal);
	pFiler->writeInt32(DB_IMP_HATCH(this->m_pImpl)->patternType);
	pFiler->writeString(DB_IMP_HATCH(this->m_pImpl)->patternName);
	pFiler->writeDouble(DB_IMP_HATCH(this->m_pImpl)->patternAngle);
	pFiler->writeDouble(DB_IMP_HATCH(this->m_pImpl)->patternScale);
	pFiler->writeDouble(DB_IMP_HATCH(this->m_pImpl)->patternSpace);
	pFiler->writeBool(DB_IMP_HATCH(this->m_pImpl)->patternDouble);
	pFiler->writeInt32(DB_IMP_HATCH(this->m_pImpl)->hatchStyle);
	pFiler->writeBool(DB_IMP_HATCH(this->m_pImpl)->associative);

	// 写入边界环
	pFiler->writeUInt32(DB_IMP_HATCH(this->m_pImpl)->loops.length());
	for (int li = 0; li < DB_IMP_HATCH(this->m_pImpl)->loops.length(); li++) {
		DbHatchLoopImpl* loop = DB_IMP_HATCH(this->m_pImpl)->loops[li];
		pFiler->writeInt32(loop->loopType);

		pFiler->writeUInt32(loop->vertices.length());
		for (int vi = 0; vi < loop->vertices.length(); vi++) {
			pFiler->writePoint2d(loop->vertices[vi]);
		}

		pFiler->writeUInt32(loop->bulges.length());
		for (int bi = 0; bi < loop->bulges.length(); bi++) {
			pFiler->writeDouble(loop->bulges[bi]);
		}
	}

	// 写入种子点
	pFiler->writeUInt32(DB_IMP_HATCH(this->m_pImpl)->seedPoints.length());
	for (int si = 0; si < DB_IMP_HATCH(this->m_pImpl)->seedPoints.length(); si++) {
		pFiler->writePoint2d(DB_IMP_HATCH(this->m_pImpl)->seedPoints[si]);
	}

	return Acad::ErrorStatus::eOk;
}
bool DbHatch::subWorldDraw(GiWorldDraw* pWd) const {
	return false;
}


// --- 属性访问 ---

double DbHatch::elevation() const {
	return DB_IMP_HATCH(this->m_pImpl)->elevation;
}
Acad::ErrorStatus DbHatch::setElevation(double elevation) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->elevation = elevation;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

GeVector3d DbHatch::normal() const {
	return DB_IMP_HATCH(this->m_pImpl)->normal;
}
Acad::ErrorStatus DbHatch::setNormal(const GeVector3d& normal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->normal = normal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

DbHatch::HatchPatternType DbHatch::patternType() const {
	return static_cast<HatchPatternType>(DB_IMP_HATCH(this->m_pImpl)->patternType);
}

AcString DbHatch::patternName() const {
	return DB_IMP_HATCH(this->m_pImpl)->patternName;
}
Acad::ErrorStatus DbHatch::setPattern(HatchPatternType patType, const char* patName) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->patternType = patType;
		DB_IMP_HATCH(this->m_pImpl)->patternName = patName;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

double DbHatch::patternAngle() const {
	return DB_IMP_HATCH(this->m_pImpl)->patternAngle;
}
Acad::ErrorStatus DbHatch::setPatternAngle(double angle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->patternAngle = angle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

double DbHatch::patternScale() const {
	return DB_IMP_HATCH(this->m_pImpl)->patternScale;
}
Acad::ErrorStatus DbHatch::setPatternScale(double scale) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->patternScale = scale;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

double DbHatch::patternSpace() const {
	return DB_IMP_HATCH(this->m_pImpl)->patternSpace;
}
Acad::ErrorStatus DbHatch::setPatternSpace(double space) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->patternSpace = space;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

bool DbHatch::patternDouble() const {
	return DB_IMP_HATCH(this->m_pImpl)->patternDouble;
}
Acad::ErrorStatus DbHatch::setPatternDouble(bool isDouble) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->patternDouble = isDouble;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

DbHatch::HatchStyle DbHatch::hatchStyle() const {
	return static_cast<HatchStyle>(DB_IMP_HATCH(this->m_pImpl)->hatchStyle);
}
Acad::ErrorStatus DbHatch::setHatchStyle(HatchStyle style) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->hatchStyle = style;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

bool DbHatch::associative() const {
	return DB_IMP_HATCH(this->m_pImpl)->associative;
}
Acad::ErrorStatus DbHatch::setAssociative(bool isAssociative) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->associative = isAssociative;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

bool DbHatch::isSolidFill() const {
	AcString name = DB_IMP_HATCH(this->m_pImpl)->patternName;
	if (name == "SOLID") { return true; }
	return false;
}

int DbHatch::numLoops() const {
	return DB_IMP_HATCH(this->m_pImpl)->loops.length();
}
int DbHatch::loopTypeAt(int loopIndex) const {
	if (loopIndex < 0 || loopIndex >= DB_IMP_HATCH(this->m_pImpl)->loops.length()) {
		return 0;
	}
	return DB_IMP_HATCH(this->m_pImpl)->loops[loopIndex]->loopType;
}
Acad::ErrorStatus DbHatch::getLoopAt(int loopIndex, GePoint2dArray& vertices, GeDoubleArray& bulges) const {
	if (loopIndex < 0 || loopIndex >= DB_IMP_HATCH(this->m_pImpl)->loops.length()) {
		return Acad::ErrorStatus::eInvalidIndex;
	}
	DbHatchLoopImpl* loop = DB_IMP_HATCH(this->m_pImpl)->loops[loopIndex];
	vertices = loop->vertices;
	bulges = loop->bulges;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbHatch::appendLoop(int loopType, const GePoint2dArray& vertices, const GeDoubleArray& bulges) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DbHatchLoopImpl* loop = new DbHatchLoopImpl();
		loop->loopType = loopType;
		loop->vertices = vertices;
		loop->bulges = bulges;
		DB_IMP_HATCH(this->m_pImpl)->loops.append(loop);
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}
Acad::ErrorStatus DbHatch::removeLoopAt(int loopIndex) {
	if (loopIndex < 0 || loopIndex >= DB_IMP_HATCH(this->m_pImpl)->loops.length()) {
		return Acad::ErrorStatus::eInvalidIndex;
	}
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		delete DB_IMP_HATCH(this->m_pImpl)->loops[loopIndex];
		DB_IMP_HATCH(this->m_pImpl)->loops.removeAt(loopIndex);
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}

int DbHatch::numSeedPoints() const {
	return DB_IMP_HATCH(this->m_pImpl)->seedPoints.length();
}
GePoint2d DbHatch::getSeedPointAt(int index) const {
	if (index >= 0 && index < DB_IMP_HATCH(this->m_pImpl)->seedPoints.length()) {
		return DB_IMP_HATCH(this->m_pImpl)->seedPoints[index];
	}
	return GePoint2d(0, 0);
}
Acad::ErrorStatus DbHatch::appendSeedPoint(const GePoint2d& point) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_HATCH(this->m_pImpl)->seedPoints.append(point);
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eNotOpenForWrite;
}


// --- Override methods ---

Acad::ErrorStatus DbHatch::subGetGeomExtents(DbExtents& extents) const { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbHatch::subTransformBy(const GeMatrix3d& xform) { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbHatch::subGetGripPoints(DbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize, const GeVector3d& curViewDir, const int bitflags) const { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbHatch::subGetOsnapPoints(Db::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const GePoint3d& pickPoint, const GePoint3d& lastPoint, const GeMatrix3d& viewXform, GePoint3dArray& snapPoints, DbIntArray& geomIds) const { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbHatch::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbHatch::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) { return Acad::ErrorStatus::eOk; }
Acad::ErrorStatus DbHatch::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {
	if (pEnt == NULL) { return Acad::ErrorStatus::eFail; }
	return Acad::ErrorStatus::eOk;
}
