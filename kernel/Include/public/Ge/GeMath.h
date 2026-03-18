#ifndef GEMATH_H
#define GEMATH_H

#include <math.h>
#include "GeGbl.h"
#include "GeTol.h"
#include "GePoint2d.h"
#include "GePoint3d.h"
#include "GeCircArc2d.h"
#include "GeLinearEnt2d.h"
#include "GePoint2dArray.h"
#include "GePoint3dArray.h"

class GeMath
{
public:
	GeMath()
	{
	}
	~GeMath()
	{
	}

public:
	// 求点到直线的垂点
	static GePoint2d vertical(const GePoint2d& pt, const GePoint2d& begin, const GePoint2d& end, const GeTol& tol = GeContext::gTol);

	// 获得点在直线的方向
	static int lineDirection(const GePoint2d& pt, const GePoint2d& lineStartPoint, const GePoint2d& lineEndPoint);

	// 获得点的中点
	static GePoint3d mindPoint(const GePoint3d& pt1, const GePoint3d& pt2);
	static GePoint3d mindPoint(const GePoint3d& pt1, const GePoint3d& pt2, const GePoint3d& pt3);
	static GePoint3d mindPoint(const GePoint3d& pt1, const GePoint3d& pt2, const GePoint3d& pt3, const GePoint3d& pt4);
	static GePoint3d mindPoint(const GePoint3dArray& pts);

	// 获得包围盒
	static void extents(const GePoint2dArray& pts, GePoint2d& _minPoint, GePoint2d& _maxPoint);

	// 直线拟合
	static bool lineFitting(const GePoint2dArray& pts, GePoint2d& _startPoint, GePoint2d& _endPoint);
	static bool lineFitting(const GePoint2dArray& pts, GePoint2d& _startPoint, GePoint2d& _endPoint, double tol);

	// 圆拟合
	static bool circleFitting(const GePoint2dArray& pts, GePoint2d& _center, double& _radius, double tol);
};

#endif
