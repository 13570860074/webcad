#include "DbExtents.h"
#include "GePoint3dArray.h"

DbExtents::DbExtents() {
	this->set(GePoint3d::kOrigin, GePoint3d::kOrigin);
}
DbExtents::DbExtents(const DbExtents& src) {
	this->set(src.minPoint(), src.maxPoint());
}
DbExtents::DbExtents(const GePoint3d& min, const GePoint3d& max) {
	this->set(min, max);
}

Acad::ErrorStatus DbExtents::set(const GePoint3d& min, const GePoint3d& max) {
	this->mMinPoint.set(min.x, min.y, min.z);
	this->mMaxPoint.set(max.x, max.y, max.z);
	return Acad::ErrorStatus::eOk;
}

void DbExtents::addPoint(const GePoint3d& pt) {
	GePoint3dArray points;
	points.append(this->minPoint());
	points.append(this->maxPoint());
	points.append(pt);

	for (int i = 0; i < points.length(); i++) {
		if (i == 0) {
			this->mMinPoint.set(points[i].x, points[i].y, points[i].z);
			this->mMaxPoint.set(points[i].x, points[i].y, points[i].z);
			continue;
		}

		if (points[i].x < this->mMinPoint.x) {
			this->mMinPoint.x = points[i].x;
		}
		else if (points[i].y < this->mMinPoint.y) {
			this->mMinPoint.y = points[i].y;
		}
		else if (points[i].z < this->mMinPoint.z) {
			this->mMinPoint.z = points[i].z;
		}
		else if (points[i].x > this->mMaxPoint.x) {
			this->mMaxPoint.x = points[i].x;
		}
		else if (points[i].y > this->mMaxPoint.y) {
			this->mMaxPoint.y = points[i].y;
		}
		else if (points[i].z > this->mMaxPoint.z) {
			this->mMaxPoint.z = points[i].z;
		}
	}
}
void DbExtents::addExt(const DbExtents& src) {
	this->addPoint(src.minPoint());
	this->addPoint(src.maxPoint());
}

void DbExtents::expandBy(const GeVector3d& vec) {
	this->mMinPoint += vec;
	this->mMaxPoint += vec;
}
void DbExtents::transformBy(const GeMatrix3d& mat) {
	this->mMinPoint.transformBy(mat);
	this->mMaxPoint.transformBy(mat);
}

bool DbExtents::operator==(const DbExtents& other) {
	if (this->minPoint().isEqualTo(other.minPoint()) == false) {
		return false;
	}
	if (this->maxPoint().isEqualTo(other.maxPoint()) == false) {
		return false;
	}
	return true;
}