#include "GeMath.h"
#include "GeLine2d.h"
#include "GePoint2d.h"
#include "GePoint3dArray.h"

GePoint2d GeMath::vertical(const GePoint2d& pt, const GePoint2d& begin, const GePoint2d& end, const GeTol& tol)
{
	GePoint2d retVal;

	double dx = begin.x - end.x;
	double dy = begin.y - end.y;
	if (abs(dx) < tol.equalPoint() && abs(dy) < tol.equalPoint())
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
	if (abs((lineEndPoint.x - lineStartPoint.x)) < 0.00001)
	{
		if (pt.x < lineStartPoint.x)
		{
			return 0;
		}
		return 1;
	}
	double m = (lineEndPoint.y - lineStartPoint.y) / (lineEndPoint.x - lineStartPoint.x);
	double b = lineStartPoint.y - m * lineStartPoint.x;
	if (pt.y > m * pt.x + b)
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

	double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_xx = 0.0;
	for (int i = 0; i < pts.length(); i++) {
		sum_x += pts.at(i).x;
		sum_y += pts.at(i).y;
		sum_xy += pts.at(i).x * pts.at(i).y;
		sum_xx += pts.at(i).x * pts.at(i).x;
	}

	double mean_x = sum_x / n;
	double mean_y = sum_y / n;

	// Calculate the slope and intercept using the least squares formulas  
	double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
	double intercept = mean_y - slope * mean_x;

	// 计算直线
	GePoint2d p1, p2;
	p1.x = pts.at(0).x;
	p1.y = slope * p1.x + intercept;
	p2.x = pts.at(pts.length() - 1).x;
	p2.y = slope * p2.x + intercept;

	// 获得包围盒
	GePoint2d minPoint, maxPoint;
	extents(pts, minPoint, maxPoint);

	// 计算基点
	GePoint2d basePoint = minPoint;
	basePoint.x -= (maxPoint.x - minPoint.x);
	basePoint.y -= (maxPoint.y - minPoint.y);
	basePoint = GeMath::vertical(basePoint, p1, p2);

	// 计算开始坐标
	double minVal = 0.0;
	double maxVal = 0.0;
	for (int i = 0; i < pts.length(); i++) {
		GePoint2d ps = GeMath::vertical(pts.at(i), p1, p2);
		double dist = basePoint.distanceTo(ps);
		if (i == 0) {
			minVal = dist;
			maxVal = dist;
			_startPoint.set(ps.x, ps.y);
			_endPoint.set(ps.x, ps.y);
			continue;
		}
		if (dist < minVal) {
			minVal = dist;
			_startPoint.set(ps.x, ps.y);
		}
		if (dist > maxVal) {
			maxVal = dist;
			_endPoint.set(ps.x, ps.y);
		}
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
		if (dist > tol) {
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

	// 得到三个点
	GePoint2d p1, p2, p3;
	if (pts.length() <= 5) {
		p1 = pts.at(0);
		p2 = pts.at(int(pts.length() / 2.0));
		p3 = pts.at(pts.length() - 1);
	}
	else {
		p1 = pts.at(int(pts.length() / 5.0) * 1);
		p2 = pts.at(int(pts.length() / 2.0));
		p3 = pts.at(int(pts.length() / 5.0) * 4);
	}

	// 拟合得到圆心和半径
	GeCircArc2d circArc;
	circArc.set(p1, p2, p3);
	_center.set(circArc.center().x, circArc.center().y);
	_radius = circArc.radius();

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