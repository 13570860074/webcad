#include "GeMath.h"
#include "GeLine2d.h"
#include "GePoint2d.h"
#include "GePoint3dArray.h"
#include <cmath>

GePoint2d GeMath::vertical(const GePoint2d& pt, const GePoint2d& begin, const GePoint2d& end, const GeTol& tol)
{
	GePoint2d retVal;

	double dx = begin.x - end.x;
	double dy = begin.y - end.y;
	if (std::fabs(dx) < tol.equalPoint() && std::fabs(dy) < tol.equalPoint())
	{
		retVal = begin;
		return retVal;
	}

	double u = (pt.x - begin.x) * (begin.x - end.x) + (pt.y - begin.y) * (begin.y - end.y);
	u = u / ((dx * dx) + (dy * dy));

	retVal.x = begin.x + u * dx;
	retVal.y = begin.y + u * dy;

	return retVal;
}


int GeMath::lineDirection(const GePoint2d& pt, const GePoint2d& lineStartPoint, const GePoint2d& lineEndPoint)
{
	double dx = lineEndPoint.x - lineStartPoint.x;
	double dy = lineEndPoint.y - lineStartPoint.y;
	if (std::fabs(dx) <= GeContext::gTol.equalPoint() && std::fabs(dy) <= GeContext::gTol.equalPoint())
	{
		return 1;
	}

	double crossVal = dx * (pt.y - lineStartPoint.y) - dy * (pt.x - lineStartPoint.x);
	if (crossVal > GeContext::gTol.equalPoint())
	{
		return 0;
	}
	return 1;
}

GePoint3d GeMath::mindPoint(const GePoint3d& pt1, const GePoint3d& pt2)
{
	GePoint3dArray pts;
	pts.append(pt1);
	pts.append(pt2);
	return GeMath::mindPoint(pts);
}
GePoint3d GeMath::mindPoint(const GePoint3d& pt1, const GePoint3d& pt2, const GePoint3d& pt3)
{

	GePoint3dArray pts;
	pts.append(pt1);
	pts.append(pt2);
	pts.append(pt3);
	return GeMath::mindPoint(pts);
}
GePoint3d GeMath::mindPoint(const GePoint3d& pt1, const GePoint3d& pt2, const GePoint3d& pt3, const GePoint3d& pt4)
{

	GePoint3dArray pts;
	pts.append(pt1);
	pts.append(pt2);
	pts.append(pt3);
	pts.append(pt4);
	return GeMath::mindPoint(pts);
}
GePoint3d GeMath::mindPoint(const GePoint3dArray& pts)
{
	GePoint3d minPoint, maxPoint;
	for (int i = 0; i < pts.length(); i++)
	{
		if (i == 0)
		{
			minPoint.x = pts.at(i).x;
			minPoint.y = pts.at(i).y;
			minPoint.z = pts.at(i).z;
			maxPoint.x = pts.at(i).x;
			maxPoint.y = pts.at(i).y;
			maxPoint.z = pts.at(i).z;
			continue;
		}

		if (pts.at(i).x < minPoint.x)
		{
			minPoint.x = pts.at(i).x;
		}
		if (pts.at(i).y < minPoint.y)
		{
			minPoint.y = pts.at(i).y;
		}
		if (pts.at(i).z < minPoint.z)
		{
			minPoint.z = pts.at(i).z;
		}

		if (pts.at(i).x > maxPoint.x)
		{
			maxPoint.x = pts.at(i).x;
		}
		if (pts.at(i).y > maxPoint.y)
		{
			maxPoint.y = pts.at(i).y;
		}
		if (pts.at(i).z > maxPoint.z)
		{
			maxPoint.z = pts.at(i).z;
		}
	}
	return minPoint + (maxPoint - minPoint) / 2.0;
}
void GeMath::extents(const GePoint2dArray& pts, GePoint2d& _minPoint, GePoint2d& _maxPoint) {
	if (pts.length() <= 0) {
		_minPoint = GePoint2d::kOrigin;
		_maxPoint = GePoint2d::kOrigin;
		return;
	}

	for (int i = 0; i < pts.length(); i++) {
		if (i == 0) {
			_minPoint = pts.at(0);
			_maxPoint = pts.at(0);
			continue;
		}

		if (pts.at(i).x < _minPoint.x) {
			_minPoint.x = pts.at(i).x;
		}
		if (pts.at(i).y < _minPoint.y) {
			_minPoint.y = pts.at(i).y;
		}

		if (pts.at(i).x > _maxPoint.x) {
			_maxPoint.x = pts.at(i).x;
		}
		if (pts.at(i).y > _maxPoint.y) {
			_maxPoint.y = pts.at(i).y;
		}
	}
}
bool GeMath::lineFitting(const GePoint2dArray& pts, GePoint2d& _startPoint, GePoint2d& _endPoint) {
	int n = pts.length();
	if (n < 2) {
		return false;
	}

	double sum_x = 0.0;
	double sum_y = 0.0;
	for (int i = 0; i < pts.length(); i++) {
		sum_x += pts.at(i).x;
		sum_y += pts.at(i).y;
	}

	double mean_x = sum_x / n;
	double mean_y = sum_y / n;
	double sxx = 0.0;
	double syy = 0.0;
	double sxy = 0.0;
	for (int i = 0; i < pts.length(); i++) {
		double cx = pts.at(i).x - mean_x;
		double cy = pts.at(i).y - mean_y;
		sxx += cx * cx;
		syy += cy * cy;
		sxy += cx * cy;
	}

	if (sxx <= GeContext::gTol.equalPoint() && syy <= GeContext::gTol.equalPoint()) {
		return false;
	}

	double theta = 0.5 * std::atan2(2.0 * sxy, sxx - syy);
	double dirX = std::cos(theta);
	double dirY = std::sin(theta);

	double minVal = 0.0;
	double maxVal = 0.0;
	for (int i = 0; i < pts.length(); i++) {
		double proj = (pts.at(i).x - mean_x) * dirX + (pts.at(i).y - mean_y) * dirY;
		if (i == 0) {
			minVal = proj;
			maxVal = proj;
			continue;
		}
		if (proj < minVal) {
			minVal = proj;
		}
		if (proj > maxVal) {
			maxVal = proj;
		}
	}

	_startPoint.set(mean_x + minVal * dirX, mean_y + minVal * dirY);
	_endPoint.set(mean_x + maxVal * dirX, mean_y + maxVal * dirY);

	if (_startPoint.distanceTo(_endPoint) <= GeContext::gTol.equalPoint()) {
		return false;
	}

	return true;
}
bool GeMath::lineFitting(const GePoint2dArray& pts, GePoint2d& _startPoint, GePoint2d& _endPoint, double tol) {
	if (GeMath::lineFitting(pts, _startPoint, _endPoint) == false) {
		return false;
	}
	for (int i = 0; i < pts.length(); i++) {
		GePoint2d ps = GeMath::vertical(pts.at(i), _startPoint, _endPoint);
		double dist = pts.at(i).distanceTo(ps);
		if (dist > std::fabs(tol)) {
			return false;
		}
	}
	return true;
}
bool GeMath::circleFitting(const GePoint2dArray& pts, GePoint2d& _center, double& _radius, double tol) {

	int n = pts.length();
	if (n < 3) {
		return false;
	}

	// 选取三个尽量分散且不共线的点
	GePoint2d p1, p2, p3;
	bool hasValidSample = false;
	for (int i = 0; i < n - 2 && hasValidSample == false; ++i) {
		for (int j = i + 1; j < n - 1 && hasValidSample == false; ++j) {
			for (int k = j + 1; k < n; ++k) {
				double crossVal = (pts.at(j).x - pts.at(i).x) * (pts.at(k).y - pts.at(i).y) -
					(pts.at(j).y - pts.at(i).y) * (pts.at(k).x - pts.at(i).x);
				if (std::fabs(crossVal) > GeContext::gTol.equalPoint()) {
					p1 = pts.at(i);
					p2 = pts.at(j);
					p3 = pts.at(k);
					hasValidSample = true;
					break;
				}
			}
		}
	}

	if (hasValidSample == false) {
		return false;
	}

	// 拟合得到圆心和半径
	GeCircArc2d circArc;
	circArc.set(p1, p2, p3);
	_center.set(circArc.center().x, circArc.center().y);
	_radius = circArc.radius();
	if (std::fabs(_radius) <= GeContext::gTol.equalPoint()) {
		return false;
	}

	double tolAbs = std::fabs(tol);
	if (tolAbs > 0.0) {
		for (int i = 0; i < pts.length(); i++) {
			double dist = pts.at(i).distanceTo(_center);
			if (std::fabs(dist - _radius) > tolAbs) {
				return false;
			}
		}
	}

	// 判断是否在精度范围内
	//double minDist = radius - tol;
	//double maxDist = radius + tol;
	//for (int i = 0; i < pts.length(); i++) {
	//	double dist = pts.at(0).distanceTo(center);
	//	if (dist < minDist || dist > maxDist) {
	//		return false;
	//	}
	//}

	//// 确定起始和终止角度
	//for (int i = 1; i < pts.length(); i++) {

	//}

	return true;
}