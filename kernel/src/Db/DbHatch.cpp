#include "DbHatch.h"
#include "GePlane.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>


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
	int nLoops = this->numLoops();
	if (nLoops == 0) {
		return true;
	}

	// OCS→WCS变换
	GeVector3d norm = this->normal();
	double elev = this->elevation();
	GeMatrix3d mat;
	mat.setToPlaneToWorld(norm);

	// 遍历每个边界环进行绘制
	for (int li = 0; li < nLoops; li++) {
		GePoint2dArray vertices;
		GeDoubleArray bulges;
		this->getLoopAt(li, vertices, bulges);

		int nVerts = vertices.length();
		if (nVerts < 2) {
			continue;
		}

		// 转换2D顶点到3D
		GePoint3dArray pts3d;
		for (int vi = 0; vi < nVerts; vi++) {
			GePoint3d pt(vertices[vi].x, vertices[vi].y, 0.0);
			pt.transformBy(mat);
			pt += norm * elev;
			pts3d.append(pt);
		}

		// 绘制每条边 (最后一条边连回起点)
		for (int vi = 0; vi < nVerts; vi++) {
			int nextVi = (vi + 1) % nVerts;
			GePoint3d startPt = pts3d[vi];
			GePoint3d endPt = pts3d[nextVi];

			double bulge = (vi < bulges.length()) ? bulges[vi] : 0.0;

			if (std::fabs(bulge) < 1e-10) {
				// 直线段
				pWd->geometry().line(startPt, endPt);
			} else {
				// 弧线段: bulge = tan(includedAngle / 4)
				double includedAngle = std::atan(bulge) * 4.0;
				GePoint3d midPt = startPt + (endPt - startPt) * 0.5;
				double dist = startPt.distanceTo(endPt) * 0.5;
				if (dist < 1e-10) {
					continue;
				}
				double radius = dist / std::sin(includedAngle * 0.5);
				if (std::fabs(radius) < 1e-10) {
					continue;
				}

				// 通过三点式circularArc绘制
				GeVector3d chordDir = endPt - startPt;
				chordDir.normalize();
				GeVector3d perpDir = chordDir.crossProduct(norm);
				if (perpDir.length() < 1e-10) {
					pWd->geometry().line(startPt, endPt);
					continue;
				}
				perpDir.normalize();
				double sagitta = dist * bulge;
				GePoint3d arcMidPt = midPt + perpDir * sagitta;

				pWd->geometry().circularArc(startPt, arcMidPt, endPt);
			}
		}

		// 实心填充: 用polygon绘制
		if (this->isSolidFill() && nVerts >= 3) {
			// 检查边界是否全为直线段(无弧段)
			bool allStraight = true;
			for (int vi = 0; vi < nVerts && vi < bulges.length(); vi++) {
				if (std::fabs(bulges[vi]) > 1e-10) {
					allStraight = false;
					break;
				}
			}
			if (allStraight) {
				pWd->geometry().polygon(nVerts, &pts3d[0]);
			}
		}
	}

	return true;
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

Acad::ErrorStatus DbHatch::subGetGeomExtents(DbExtents& extents) const {
	GeVector3d norm = this->normal();
	double elev = this->elevation();
	GeMatrix3d mat;
	mat.setToPlaneToWorld(norm);

	int nLoops = this->numLoops();
	for (int li = 0; li < nLoops; li++) {
		GePoint2dArray vertices;
		GeDoubleArray bulges;
		this->getLoopAt(li, vertices, bulges);
		int nVerts = vertices.length();
		for (int vi = 0; vi < nVerts; vi++) {
			GePoint3d pt(vertices[vi].x, vertices[vi].y, 0.0);
			pt.transformBy(mat);
			pt += norm * elev;
			extents.addPoint(pt);

			// 弧线段的弧顶也需要计入
			double bulge = (vi < bulges.length()) ? bulges[vi] : 0.0;
			if (std::fabs(bulge) > 1e-10) {
				int nextVi = (vi + 1) % nVerts;
				GePoint3d startPt(vertices[vi].x, vertices[vi].y, 0.0);
				GePoint3d endPt(vertices[nextVi].x, vertices[nextVi].y, 0.0);
				startPt.transformBy(mat);
				startPt += norm * elev;
				endPt.transformBy(mat);
				endPt += norm * elev;
				GePoint3d midPt = startPt + (endPt - startPt) * 0.5;
				GeVector3d chordDir = endPt - startPt;
				chordDir.normalize();
				GeVector3d perpDir = chordDir.crossProduct(norm);
				if (perpDir.length() > 1e-10) {
					perpDir.normalize();
					double dist = startPt.distanceTo(endPt) * 0.5;
					double sagitta = dist * bulge;
					GePoint3d arcMidPt = midPt + perpDir * sagitta;
					extents.addPoint(arcMidPt);
				}
			}
		}
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbHatch::subTransformBy(const GeMatrix3d& xform) {
	DbHatchImpl* imp = DB_IMP_HATCH(this->m_pImpl);

	// 变换法向量和原点以确定新的OCS
	GeVector3d oldNorm = imp->normal;
	GeMatrix3d oldMat;
	oldMat.setToPlaneToWorld(oldNorm);
	GePoint3d ocsOrigin(0, 0, imp->elevation);
	ocsOrigin.transformBy(oldMat);

	// 变换每个loop的顶点: OCS→WCS→变换→投影回新OCS
	// 先确定新的法向量
	GeVector3d newNorm = oldNorm;
	newNorm.transformBy(xform);
	newNorm.normalize();
	imp->normal = newNorm;

	// 新的OCS→WCS逆矩阵
	GeMatrix3d newMat;
	newMat.setToPlaneToWorld(newNorm);
	GeMatrix3d newMatInv = newMat;
	newMatInv.invert();

	for (int li = 0; li < imp->loops.length(); li++) {
		DbHatchLoopImpl* loop = imp->loops[li];
		for (int vi = 0; vi < loop->vertices.length(); vi++) {
			GePoint3d pt(loop->vertices[vi].x, loop->vertices[vi].y, 0.0);
			pt.transformBy(oldMat);
			pt += oldNorm * imp->elevation;
			pt.transformBy(xform);
			pt.transformBy(newMatInv);
			loop->vertices[vi] = GePoint2d(pt.x, pt.y);
		}
	}

	// 变换种子点
	for (int si = 0; si < imp->seedPoints.length(); si++) {
		GePoint3d pt(imp->seedPoints[si].x, imp->seedPoints[si].y, 0.0);
		pt.transformBy(oldMat);
		pt += oldNorm * imp->elevation;
		pt.transformBy(xform);
		pt.transformBy(newMatInv);
		imp->seedPoints[si] = GePoint2d(pt.x, pt.y);
	}

	// 计算新的elevation
	GePoint3d elevPt = ocsOrigin;
	elevPt.transformBy(xform);
	elevPt.transformBy(newMatInv);
	imp->elevation = elevPt.z;

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbHatch::subGetGripPoints(DbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize, const GeVector3d& curViewDir, const int bitflags) const {
	GeVector3d norm = this->normal();
	double elev = this->elevation();
	GeMatrix3d mat;
	mat.setToPlaneToWorld(norm);

	// 种子点作为夹点
	DbHatchImpl* imp = DB_IMP_HATCH(this->m_pImpl);
	for (int si = 0; si < imp->seedPoints.length(); si++) {
		GePoint3d pt(imp->seedPoints[si].x, imp->seedPoints[si].y, 0.0);
		pt.transformBy(mat);
		pt += norm * elev;
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(pt);
		grips.append(grip);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbHatch::subGetOsnapPoints(Db::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const GePoint3d& pickPoint, const GePoint3d& lastPoint, const GeMatrix3d& viewXform, GePoint3dArray& snapPoints, DbIntArray& geomIds) const {
	if (osnapMode != Db::kOsModeEnd && osnapMode != Db::kOsModeNear) {
		return Acad::ErrorStatus::eOk;
	}

	GeVector3d norm = this->normal();
	double elev = this->elevation();
	GeMatrix3d mat;
	mat.setToPlaneToWorld(norm);

	int nLoops = this->numLoops();
	for (int li = 0; li < nLoops; li++) {
		GePoint2dArray vertices;
		GeDoubleArray bulges;
		this->getLoopAt(li, vertices, bulges);
		int nVerts = vertices.length();
		if (osnapMode == Db::kOsModeEnd) {
			for (int vi = 0; vi < nVerts; vi++) {
				GePoint3d pt(vertices[vi].x, vertices[vi].y, 0.0);
				pt.transformBy(mat);
				pt += norm * elev;
				snapPoints.append(pt);
			}
		} else if (osnapMode == Db::kOsModeNear) {
			// 找最近边界点
			double minDist = 1e300;
			GePoint3d nearPt;
			for (int vi = 0; vi < nVerts; vi++) {
				int nextVi = (vi + 1) % nVerts;
				GePoint3d p0(vertices[vi].x, vertices[vi].y, 0.0);
				GePoint3d p1(vertices[nextVi].x, vertices[nextVi].y, 0.0);
				p0.transformBy(mat);
				p0 += norm * elev;
				p1.transformBy(mat);
				p1 += norm * elev;
				// 投影到线段上
				GeVector3d seg = p1 - p0;
				double len = seg.length();
				if (len < 1e-10) continue;
				double t = (pickPoint - p0).dotProduct(seg) / (len * len);
				if (t < 0.0) t = 0.0;
				if (t > 1.0) t = 1.0;
				GePoint3d proj = p0 + seg * t;
				double d = pickPoint.distanceTo(proj);
				if (d < minDist) {
					minDist = d;
					nearPt = proj;
				}
			}
			if (minDist < 1e300) {
				snapPoints.append(nearPt);
			}
		}
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbHatch::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	DbHatchImpl* imp = DB_IMP_HATCH(this->m_pImpl);
	GeVector3d norm = imp->normal;
	GeMatrix3d mat;
	mat.setToPlaneToWorld(norm);
	GeMatrix3d matInv = mat;
	matInv.invert();

	for (int i = 0; i < indices.length(); i++) {
		int idx = indices[i];
		if (idx >= 0 && idx < imp->seedPoints.length()) {
			GePoint3d pt(imp->seedPoints[idx].x, imp->seedPoints[idx].y, 0.0);
			pt.transformBy(mat);
			pt += norm * imp->elevation;
			pt += offset;
			pt.transformBy(matInv);
			imp->seedPoints[idx] = GePoint2d(pt.x, pt.y);
		}
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbHatch::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbHatch::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {
	if (pEnt == NULL) { return Acad::ErrorStatus::eFail; }
	return Acad::ErrorStatus::eOk;
}
