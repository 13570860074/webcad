#include "DbExtents2d.h"
#include "GePoint2dArray.h"

DbExtents2d::DbExtents2d() {
	this->set(GePoint2d::kOrigin, GePoint2d::kOrigin);
}
DbExtents2d::DbExtents2d(const DbExtents2d& src) {
	this->set(src.minPoint(), src.maxPoint());
}
DbExtents2d::DbExtents2d(const GePoint2d& min, const GePoint2d& max) {
	this->set(min, max);
}

Acad::ErrorStatus DbExtents2d::set(const GePoint2d& min, const GePoint2d& max) {
	this->mMinPoint.set(min.x, min.y);
	this->mMaxPoint.set(max.x, max.y);
	return Acad::ErrorStatus::eOk;
}

void DbExtents2d::addPoint(const GePoint2d& pt) {
	GePoint2dArray points;
	points.append(this->minPoint());
	points.append(this->maxPoint());
	points.append(pt);

	for (int i = 0; i < points.length(); i++) {
		if (i == 0) {
			this->mMinPoint.set(points[i].x, points[i].y);
			this->mMaxPoint.set(points[i].x, points[i].y);
			continue;
		}

		if (points[i].x < this->mMinPoint.x) {
			this->mMinPoint.x = points[i].x;
		}
		else if (points[i].y < this->mMinPoint.y) {
			this->mMinPoint.y = points[i].y;
		}
		else if (points[i].x > this->mMaxPoint.x) {
			this->mMaxPoint.x = points[i].x;
		}
		else if (points[i].y > this->mMaxPoint.y) {
			this->mMaxPoint.y = points[i].y;
		}
	}
}
void DbExtents2d::addExt(const DbExtents2d& src) {
	GePoint2dArray points;
	points.append(this->minPoint());
	points.append(this->maxPoint());
	points.append(src.minPoint());
	points.append(src.maxPoint());

	for (int i = 0; i < points.length(); i++) {
		if (i == 0) {
			this->mMinPoint.set(points[i].x, points[i].y);
			this->mMaxPoint.set(points[i].x, points[i].y);
			continue;
		}

		if (points[i].x < this->mMinPoint.x) {
			this->mMinPoint.x = points[i].x;
		}
		else if (points[i].y < this->mMinPoint.y) {
			this->mMinPoint.y = points[i].y;
		}
		else if (points[i].x > this->mMaxPoint.x) {
			this->mMaxPoint.x = points[i].x;
		}
		else if (points[i].y > this->mMaxPoint.y) {
			this->mMaxPoint.y = points[i].y;
		}
	}
}

void DbExtents2d::expandBy(const GeVector2d& vec) {
	this->mMinPoint += vec;
	this->mMaxPoint += vec;
}
void DbExtents2d::transformBy(const GeMatrix2d& mat) {
	this->mMinPoint.transformBy(mat);
	this->mMaxPoint.transformBy(mat);
}

bool DbExtents2d::operator==(const DbExtents2d& other) {
	if (this->minPoint().isEqualTo(other.minPoint()) == false) {
		return false;
	}
	if (this->maxPoint().isEqualTo(other.maxPoint()) == false) {
		return false;
	}
	return true;
}