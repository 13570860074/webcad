#include "DbOsnapPointCompute.h"
#include "GiEntityGeometry.h"
#include "arxHeaders.h"

/* 捕捉最近点 */
GePoint3dArray osnap_near_to_lintseg(const GePoint3d& pickPoint, const DbLine* pline, const GeMatrix3d& viewXform)
{
	GeLineSeg3d thisLine(pline->startPoint(), pline->endPoint());

	GePoint3dArray snapPoints;

	// 获得视口法向
	GeVector3d normal = GeVector3d::kZAxis;
	normal.transformBy(viewXform);
	normal.normalize();
	GePlane plane(GePoint3d::kOrigin, normal);

	// 直线和点投影到平面上
	GePoint3d p1 = thisLine.startPoint();
	GePoint3d p2 = thisLine.endPoint();
	p1.orthoProject(plane);
	p2.orthoProject(plane);
	GePoint3d pos = pickPoint.orthoProject(plane);

	// 得到平面上的最近点
	GePoint3d planeClosest = GeLineSeg3d(p1, p2).closestPointTo(pos);

	// 如果点在线段上则返回
	if (thisLine.isOn(planeClosest) == true)
	{
		snapPoints.append(planeClosest);
		return snapPoints;
	}

	// 得到投影点
	GeLine3d line = GeLine3d(planeClosest, normal);
	GePoint3d intersect;
	if (line.intersectWith(thisLine, intersect) == true)
	{
		snapPoints.append(intersect);
	}

	return snapPoints;
}
GePoint3dArray osnap_near_to_ray(const GePoint3d& pickPoint, const DbRay* pRay, const GeMatrix3d& viewXform)
{
	GeRay3d thisLine(pRay->basePoint(), pRay->unitDir());

	GePoint3dArray snapPoints;

	// 获得视口法向
	GeVector3d normal = GeVector3d::kZAxis;
	normal.transformBy(viewXform);
	normal.normalize();
	GePlane plane(GePoint3d::kOrigin, normal);

	// 直线和点投影到平面上
	GePoint3d p1 = thisLine.pointOnLine();
	GePoint3d p2 = thisLine.pointOnLine() + thisLine.direction();
	p1.orthoProject(plane);
	p2.orthoProject(plane);
	GePoint3d pos = pickPoint.orthoProject(plane);

	// 得到平面上的最近点
	GePoint3d planeClosest = GeRay3d(p1, p2).closestPointTo(pos);

	// 如果点在线段上则返回
	if (thisLine.isOn(planeClosest) == true)
	{
		snapPoints.append(planeClosest);
		return snapPoints;
	}

	// 得到投影点
	GeLine3d line = GeLine3d(planeClosest, normal);
	GePoint3d intersect;
	if (line.intersectWith(thisLine, intersect) == true)
	{
		snapPoints.append(intersect);
	}

	return snapPoints;
}
GePoint3dArray osnap_near_to_line(const GePoint3d& pickPoint, const DbXline* pXline, const GeMatrix3d& viewXform)
{
	GeLine3d thisLine(pXline->basePoint(), pXline->unitDir());

	GePoint3dArray snapPoints;

	// 获得视口法向
	GeVector3d normal = GeVector3d::kZAxis;
	normal.transformBy(viewXform);
	normal.normalize();
	GePlane plane(GePoint3d::kOrigin, normal);

	// 直线和点投影到平面上
	GePoint3d p1 = thisLine.pointOnLine();
	GePoint3d p2 = thisLine.pointOnLine() + thisLine.direction();
	p1.orthoProject(plane);
	p2.orthoProject(plane);
	GePoint3d pos = pickPoint.orthoProject(plane);

	// 得到平面上的最近点
	GePoint3d planeClosest = GeLine3d(p1, p2).closestPointTo(pos);

	// 如果点在线段上则返回
	if (thisLine.isOn(planeClosest) == true)
	{
		snapPoints.append(planeClosest);
		return snapPoints;
	}

	// 得到投影点
	GeLine3d line = GeLine3d(planeClosest, normal);
	GePoint3d intersect;
	if (line.intersectWith(thisLine, intersect) == true)
	{
		snapPoints.append(intersect);
	}

	return snapPoints;
}
GePoint3dArray osnap_near_to_circle(const GePoint3d& pickPoint, const DbCircle* pCircle, const GeMatrix3d& viewXform)
{
	GeCircArc3d circArc = GeCircArc3d(pCircle->center(), pCircle->normal(), pCircle->radius());
	circArc.setAngles(0.0, PI * 2.0);

	GePoint3dArray snapPoints;

	GePoint3d point = circArc.closestPointTo(pickPoint);
	snapPoints.append(point);

	return snapPoints;
}
GePoint3dArray osnap_near_to_arc(const GePoint3d& pickPoint, const DbArc* pArc, const GeMatrix3d& viewXform)
{
	GeCircArc3d circArc = GeCircArc3d(pArc->center(), pArc->normal(), pArc->radius());
	circArc.setAngles(pArc->startAngle(), pArc->endAngle());

	GePoint3dArray snapPoints;

	GePoint3d point = circArc.closestPointTo(pickPoint);
	snapPoints.append(point);

	return snapPoints;
}
GePoint3dArray osnap_near_to_polyline(const GePoint3d& pickPoint, const DbPolyline* pPolyline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{
			GeLineSeg3d line;
			pPolyline->getLineSegAt(i, line);

			DbLine* pLine = new DbLine();
			pLine->setStartPoint(line.startPoint());
			pLine->setEndPoint(line.endPoint());
			GePoint3dArray temps = osnap_near_to_lintseg(pickPoint, pLine, viewXform);
			for (int u = 0; u < temps.length(); u++)
			{
				snapPoints.append(temps.at(u));
			}
			delete pLine;
		}
		else if (segType == DbPolyline::kArc)
		{
			GeCircArc3d circArc;
			pPolyline->getArcSegAt(i, circArc);

			DbArc* pArc = new DbArc();
			pArc->setCenter(circArc.center());
			pArc->setNormal(circArc.normal());
			pArc->setRadius(circArc.radius());
			pArc->setStartAngle(circArc.startAng());
			pArc->setEndAngle(circArc.endAng());
			GePoint3dArray temps = osnap_near_to_arc(pickPoint, pArc, viewXform);
			for (int u = 0; u < temps.length(); u++)
			{
				snapPoints.append(temps.at(u));
			}
			delete pArc;
		}
	}

	return snapPoints;
}

/* 捕捉中点 */
GePoint3dArray osnap_mid_to_lintseg(const GePoint3d& pickPoint, const DbLine* pLine, const GeMatrix3d& viewXform)
{
	GeLineSeg3d thisLine(pLine->startPoint(), pLine->endPoint());

	GePoint3dArray snapPoints;

	GePoint3d startPoint = thisLine.startPoint();
	GePoint3d endPoint = thisLine.endPoint();
	GePoint3d midPoint = startPoint + (endPoint - startPoint) / 2.0;
	snapPoints.append(midPoint);

	return snapPoints;
}
GePoint3dArray osnap_mid_to_ray(const GePoint3d& pickPoint, const DbRay* ray, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;
	return snapPoints;
}
GePoint3dArray osnap_mid_to_line(const GePoint3d& pickPoint, const DbXline* xline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;
	return snapPoints;
}
GePoint3dArray osnap_mid_to_circle(const GePoint3d& pickPoint, const DbCircle* circle, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;
	return snapPoints;
}
GePoint3dArray osnap_mid_to_arc(const GePoint3d& pickPoint, const DbArc* pArc, const GeMatrix3d& viewXform)
{
	GeCircArc3d thisCircArc(pArc->center(), pArc->normal(), pArc->radius());
	thisCircArc.setAngles(pArc->startAngle(), pArc->endAngle());

	GePoint3dArray snapPoints;
	GePoint3d pt = thisCircArc.evalPoint(thisCircArc.startAng() + (thisCircArc.endAng() - thisCircArc.startAng()) / 2.0);
	snapPoints.append(pt);
	return snapPoints;
}
GePoint3dArray osnap_mid_to_polyline(const GePoint3d& pickPoint, const DbPolyline* pPolyline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{
			GeLineSeg3d lineseg;
			pPolyline->getLineSegAt(i, lineseg);
			GePoint3d pt = lineseg.startPoint() + (lineseg.endPoint() - lineseg.startPoint()) / 2.0;
			snapPoints.append(pt);
		}
		else if (segType == DbPolyline::kArc)
		{
			GeCircArc3d circArc;
			pPolyline->getArcSegAt(i, circArc);
			GePoint3d pt = circArc.evalPoint(circArc.startAng() + (circArc.endAng() - circArc.startAng()) / 2.0);
			snapPoints.append(pt);
		}
	}

	return snapPoints;
}

/* 捕捉圆心 */
GePoint3dArray osnap_center_to_circle(const GePoint3d& pickPoint, const DbCircle* pCircle, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;
	snapPoints.append(pCircle->center());
	return snapPoints;
}
GePoint3dArray osnap_center_to_arc(const GePoint3d& pickPoint, const DbArc* pArc, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;
	snapPoints.append(pArc->center());
	return snapPoints;
}
GePoint3dArray osnap_center_to_polyline(const GePoint3d& pickPoint, const DbPolyline* pPolyline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kArc)
		{
			GeCircArc3d circArc;
			pPolyline->getArcSegAt(i, circArc);
			snapPoints.append(circArc.center());
		}
	}

	return snapPoints;
}

/* 捕捉现象点 */
GePoint3dArray osnap_quad_to_circle(const GePoint3d& pickPoint, const DbCircle* pCircle, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	GePoint3d startPoint;
	pCircle->getStartPoint(startPoint);

	GePoint3d pos;
	pCircle->getPointAtParam(0, pos);
	snapPoints.append(pos);

	pCircle->getPointAtParam(PI / 2.0 * 1, pos);
	snapPoints.append(pos);

	pCircle->getPointAtParam(PI / 2.0 * 2, pos);
	snapPoints.append(pos);

	pCircle->getPointAtParam(PI / 2.0 * 3, pos);
	snapPoints.append(pos);

	return snapPoints;
}
GePoint3dArray osnap_quad_to_arc(const GePoint3d& pickPoint, const DbArc* pArc, const GeMatrix3d& viewXform)
{
	GeCircArc3d thisCircArc(pArc->center(), pArc->normal(), pArc->radius());
	thisCircArc.setAngles(pArc->startAngle(), pArc->endAngle());

	GePoint3dArray snapPoints;

	GePoint3d pos = thisCircArc.evalPoint(0.0);
	if (thisCircArc.isOn(pos) == true)
	{
		snapPoints.append(pos);
	}

	pos = thisCircArc.evalPoint(PI / 2.0);
	if (thisCircArc.isOn(pos) == true)
	{
		snapPoints.append(pos);
	}

	pos = thisCircArc.evalPoint(PI);
	if (thisCircArc.isOn(pos) == true)
	{
		snapPoints.append(pos);
	}

	pos = thisCircArc.evalPoint(PI / 2.0 * 3.0);
	if (thisCircArc.isOn(pos) == true)
	{
		snapPoints.append(pos);
	}

	return snapPoints;
}
GePoint3dArray osnap_quad_to_polyline(const GePoint3d& pickPoint, const DbPolyline* pPolyline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kArc)
		{
			GeCircArc3d circArc;
			pPolyline->getArcSegAt(i, circArc);

			GePoint3d pos = circArc.evalPoint(0.0);
			if (circArc.isOn(pos) == true)
			{
				snapPoints.append(pos);
			}

			pos = circArc.evalPoint(PI / 2.0);
			if (circArc.isOn(pos) == true)
			{
				snapPoints.append(pos);
			}

			pos = circArc.evalPoint(PI);
			if (circArc.isOn(pos) == true)
			{
				snapPoints.append(pos);
			}

			pos = circArc.evalPoint(PI / 2.0 * 3.0);
			if (circArc.isOn(pos) == true)
			{
				snapPoints.append(pos);
			}
		}
	}

	return snapPoints;
}

/* 捕捉端点 */
GePoint3dArray osnap_end_to_lintseg(const GePoint3d& pickPoint, const DbLine* pLine, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	GePoint3d startPoint = pLine->startPoint();
	snapPoints.append(startPoint);
	GePoint3d endPoint = pLine->endPoint();
	snapPoints.append(endPoint);

	return snapPoints;
}
GePoint3dArray osnap_end_to_ray(const GePoint3d& pickPoint, const DbRay* pRay, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;
	snapPoints.append(pRay->basePoint());
	return snapPoints;
}
GePoint3dArray osnap_end_to_line(const GePoint3d& pickPoint, const DbXline* pXline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;
	snapPoints.append(pXline->basePoint());
	return snapPoints;
}
GePoint3dArray osnap_end_to_circle(const GePoint3d& pickPoint, const DbCircle* circle, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;
	return snapPoints;
}
GePoint3dArray osnap_end_to_arc(const GePoint3d& pickPoint, const DbArc* pArc, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	GePoint3d p;
	if (pArc->getStartPoint(p) == Acad::eOk)
	{
		snapPoints.append(p);
	}
	if (pArc->getEndPoint(p) == Acad::eOk)
	{
		snapPoints.append(p);
	}

	return snapPoints;
}
GePoint3dArray osnap_end_to_polyline(const GePoint3d& pickPoint, const DbPolyline* pPolyline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		GePoint3d pt;
		pPolyline->getPointAt(i, pt);
		snapPoints.append(pt);
	}

	return snapPoints;
}

/* 捕捉垂点 */
GePoint3dArray osnap_perp_to_lintseg(const GePoint3d& pickPoint, const GePoint3d& lastPoint, const DbLine* pLine, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	if (lastPoint.isEqualTo(pickPoint) == false)
	{
		GePoint3d vertical = GeLine3d::vertical(lastPoint, GeLine3d(pLine->startPoint(), pLine->endPoint()));
		GeLineSeg3d line(pLine->startPoint(), pLine->endPoint());
		if (line.isOn(vertical) == true)
		{
			snapPoints.append(vertical);
		}
	}

	return snapPoints;
}
GePoint3dArray osnap_perp_to_ray(const GePoint3d& pickPoint, const GePoint3d& lastPoint, const DbRay* pRay, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	if (lastPoint.isEqualTo(pickPoint) == false)
	{
		GePoint3d vertical = GeLine3d::vertical(lastPoint, GeLine3d(pRay->basePoint(), pRay->unitDir()));
		GeRay3d line(pRay->basePoint(), pRay->unitDir());
		if (line.isOn(vertical) == true)
		{
			snapPoints.append(vertical);
		}
	}

	return snapPoints;
}
GePoint3dArray osnap_perp_to_line(const GePoint3d& pickPoint, const GePoint3d& lastPoint, const DbXline* pXline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	if (lastPoint.isEqualTo(pickPoint) == false)
	{
		GePoint3d vertical = GeLine3d::vertical(lastPoint, GeLine3d(pXline->basePoint(), pXline->unitDir()));
		GeLine3d line(pXline->basePoint(), pXline->unitDir());
		if (line.isOn(vertical) == true)
		{
			snapPoints.append(vertical);
		}
	}

	return snapPoints;
}
GePoint3dArray osnap_perp_to_circle(const GePoint3d& pickPoint, const GePoint3d& lastPoint, const DbCircle* pCircle, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	if (lastPoint.isEqualTo(pickPoint) == false)
	{
		GeCircArc3d circArc;
		circArc.set(pCircle->center(), pCircle->normal(), pCircle->radius());
		GeLine3d line(pCircle->center(), lastPoint);

		int intn = 0;
		GePoint3d p1;
		GePoint3d p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			snapPoints.append(p1);
		}
		else if (intn == 2)
		{
			if (pickPoint.distanceTo(p1) < pickPoint.distanceTo(p2))
			{
				snapPoints.append(p1);
			}
			else
			{
				snapPoints.append(p2);
			}
		}
	}

	return snapPoints;
}
GePoint3dArray osnap_perp_to_arc(const GePoint3d& pickPoint, const GePoint3d& lastPoint, const DbArc* pArc, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	if (lastPoint.isEqualTo(pickPoint) == false)
	{
		GeCircArc3d circArc;
		circArc.set(pArc->center(), pArc->normal(), pArc->radius());
		circArc.setAngles(pArc->startAngle(), pArc->endAngle());
		GeLine3d line(pArc->center(), lastPoint);

		int intn = 0;
		GePoint3d p1;
		GePoint3d p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			if (circArc.isOn(p1) == true)
			{
				snapPoints.append(p1);
			}
		}
		else if (intn == 2)
		{
			if (pickPoint.distanceTo(p1) < pickPoint.distanceTo(p2))
			{
				if (circArc.isOn(p1) == true)
				{
					snapPoints.append(p1);
				}
			}
			else
			{
				if (circArc.isOn(p2) == true)
				{
					snapPoints.append(p2);
				}
			}
		}
	}

	return snapPoints;
}
GePoint3dArray osnap_perp_to_polyline(const GePoint3d& pickPoint, const GePoint3d& lastPoint, const DbPolyline* pPolyline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	if (lastPoint.isEqualTo(pickPoint) == false)
	{
		for (int i = 0; i < pPolyline->numVerts(); i++)
		{
			DbPolyline::SegType segType = pPolyline->segType(i);
			if (segType == DbPolyline::kLine)
			{
				GeLineSeg3d lineseg;
				pPolyline->getLineSegAt(i, lineseg);

				GePoint3d vertical = GeLine3d::vertical(lastPoint, lineseg);
				if (lineseg.isOn(vertical) == true)
				{
					snapPoints.append(vertical);
				}
			}
			else if (segType == DbPolyline::kArc)
			{
				GeCircArc3d circArc;
				pPolyline->getArcSegAt(i, circArc);
				GeLine3d line(circArc.center(), lastPoint);
				int intn = 0;
				GePoint3d p1;
				GePoint3d p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					if (circArc.isOn(p1) == true)
					{
						snapPoints.append(p1);
					}
				}
				else if (intn == 2)
				{
					if (pickPoint.distanceTo(p1) < pickPoint.distanceTo(p2))
					{
						if (circArc.isOn(p1) == true)
						{
							snapPoints.append(p1);
						}
					}
					else
					{
						if (circArc.isOn(p2) == true)
						{
							snapPoints.append(p2);
						}
					}
				}
			}
		}
	}

	return snapPoints;
}

/* 捕捉切点 */
GePoint3dArray osnap_tan_to_circle(const GePoint3d& pickPoint, const GePoint3d& lastPoint, const DbCircle* pCircle, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	if (lastPoint.isEqualTo(pickPoint) == false)
	{
		// 如果点在圆内部或不在一个平面,则退出
		GeCircArc3d circArc = GeCircArc3d(pCircle->center(), pCircle->normal(), pCircle->radius());
		if (circArc.isInside(lastPoint) == true)
		{
			return snapPoints;
		}

		// 获得对边和斜边的夹角
		double angle = acos(pCircle->radius() / lastPoint.distanceTo(pCircle->center()));

		// 获得两个个切点
		GeVector3d vec = lastPoint - pCircle->center();
		vec.normalize();
		vec.rotateBy(angle, pCircle->normal());
		GePoint3d tangentPoint1 = pCircle->center();
		tangentPoint1 += (vec * pCircle->radius());

		vec = lastPoint - pCircle->center();
		vec.normalize();
		vec.rotateBy(0 - angle, pCircle->normal());
		GePoint3d tangentPoint2 = pCircle->center();
		tangentPoint2 += (vec * pCircle->radius());

		if (pickPoint.distanceTo(tangentPoint1) < pickPoint.distanceTo(tangentPoint2))
		{
			snapPoints.append(tangentPoint1);
		}
		else
		{
			snapPoints.append(tangentPoint2);
		}
	}

	return snapPoints;
}
GePoint3dArray osnap_tan_to_arc(const GePoint3d& pickPoint, const GePoint3d& lastPoint, const DbArc* pArc, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	if (lastPoint.isEqualTo(pickPoint) == false)
	{
		// 如果点在圆内部或不在一个平面,则退出
		GeCircArc3d circArc = GeCircArc3d(pArc->center(), pArc->normal(), pArc->radius());
		if (circArc.isInside(lastPoint) == true)
		{
			return snapPoints;
		}

		// 获得对边和斜边的夹角
		double angle = acos(pArc->radius() / lastPoint.distanceTo(pArc->center()));

		// 获得两个个切点
		GeVector3d vec = lastPoint - pArc->center();
		vec.normalize();
		vec.rotateBy(angle, pArc->normal());
		GePoint3d tangentPoint1 = pArc->center();
		tangentPoint1 += (vec * pArc->radius());

		vec = lastPoint - pArc->center();
		vec.normalize();
		vec.rotateBy(0 - angle, pArc->normal());
		GePoint3d tangentPoint2 = pArc->center();
		tangentPoint2 += (vec * pArc->radius());

		circArc.set(pArc->center(), pArc->normal(), pArc->radius());
		circArc.setAngles(pArc->startAngle(), pArc->endAngle());
		if (pickPoint.distanceTo(tangentPoint1) < pickPoint.distanceTo(tangentPoint2))
		{
			if (circArc.isOn(tangentPoint1) == true)
			{
				snapPoints.append(tangentPoint1);
			}
		}
		else
		{
			if (circArc.isOn(tangentPoint2) == true)
			{
				snapPoints.append(tangentPoint2);
			}
		}
	}

	return snapPoints;
}
GePoint3dArray osnap_tan_to_polyline(const GePoint3d& pickPoint, const GePoint3d& lastPoint, const DbPolyline* pPolyline, const GeMatrix3d& viewXform)
{
	GePoint3dArray snapPoints;

	if (lastPoint.isEqualTo(pickPoint) == false)
	{
		for (int i = 0; i < pPolyline->numVerts(); i++)
		{
			DbPolyline::SegType segType = pPolyline->segType(i);
			if (segType == DbPolyline::kLine)
			{
				GeLineSeg3d lineseg;
				pPolyline->getLineSegAt(i, lineseg);
			}
			else if (segType == DbPolyline::kArc)
			{
				GeCircArc3d circArc;
				pPolyline->getArcSegAt(i, circArc);
			}
		}
	}

	return snapPoints;
}

GePoint3dArray osnap_inters_lintseg_to_lintseg(const DbLine* pLine1, const DbLine* pLine2, Db::Intersect intType)
{

	GePoint3dArray inters;
	if (pLine1 == pLine2) {
		return inters;
	}

	GeLineSeg3d thisLine(pLine1->startPoint(), pLine1->endPoint());
	GeLineSeg3d argLine(pLine2->startPoint(), pLine2->endPoint());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLine3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLine3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_lintseg_to_ray(const DbLine* pLine, const DbRay* pRay, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLineSeg3d thisLine(pLine->startPoint(), pLine->endPoint());
	GeRay3d argLine(pRay->basePoint(), pRay->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeRay3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeRay3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_lintseg_to_line(const DbLine* pLine, const DbXline* pXline, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLineSeg3d thisLine(pLine->startPoint(), pLine->endPoint());
	GeLine3d argLine(pXline->basePoint(), pXline->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_lintseg_to_circle(const DbLine* pLine, const DbCircle* pCircle, Db::Intersect intType)
{

	GePoint3dArray inters;

	GeLineSeg3d thisLine(pLine->startPoint(), pLine->endPoint());
	GeCircArc3d argCircArc(pCircle->center(), pCircle->normal(), pCircle->radius());
	argCircArc.setAngles(0.0, PI * 2.0);

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_lintseg_to_arc(const DbLine* pLine, const DbArc* pArc, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLineSeg3d thisLine(pLine->startPoint(), pLine->endPoint());
	GeCircArc3d argCircArc(pArc->center(), pArc->normal(), pArc->radius());
	argCircArc.setAngles(pArc->startAngle(), pArc->endAngle());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_lintseg_to_polyline(const DbLine* pLine, const DbPolyline* pPolyline, Db::Intersect intType)
{

	GePoint3dArray inters;

	GeLineSeg3d thisLine;
	thisLine.set(pLine->startPoint(), pLine->endPoint());

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{
			GeLineSeg3d argLine;
			pPolyline->getLineSegAt(i, argLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{
			GeCircArc3d argCircArc;
			pPolyline->getArcSegAt(i, argCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}


GePoint3dArray osnap_inters_ray_to_lintseg(const DbRay* pRay, const DbLine* pLine, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeRay3d thisLine(pRay->basePoint(), pRay->unitDir());
	GeLineSeg3d argLine(pLine->startPoint(), pLine->endPoint());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeRay3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeRay3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_ray_to_ray(const DbRay* pRay1, const DbRay* pRay2, Db::Intersect intType)
{
	GePoint3dArray inters;
	if (pRay1 == pRay2) {
		return inters;
	}

	GeRay3d thisLine(pRay1->basePoint(), pRay1->unitDir());
	GeRay3d argLine(pRay2->basePoint(), pRay2->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeRay3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeRay3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeRay3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeRay3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_ray_to_line(const DbRay* pRay, const DbXline* pXline, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeRay3d thisLine(pRay->basePoint(), pRay->unitDir());
	GeLine3d argLine(pXline->basePoint(), pXline->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeRay3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeRay3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_ray_to_circle(const DbRay* pRay, const DbCircle* pCircle, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeRay3d thisLine(pRay->basePoint(), pRay->unitDir());
	GeCircArc3d argCircArc(pCircle->center(), pCircle->normal(), pCircle->radius());
	argCircArc.setAngles(0.0, PI * 2.0);

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeRay3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeRay3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_ray_to_arc(const DbRay* pRay, const DbArc* pArc, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeRay3d thisLine(pRay->basePoint(), pRay->unitDir());
	GeCircArc3d argCircArc(pArc->center(), pArc->normal(), pArc->radius());
	argCircArc.setAngles(pArc->startAngle(), pArc->endAngle());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeRay3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeRay3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_ray_to_polyline(const DbRay* pRay, const DbPolyline* pPolyline, Db::Intersect intType)
{

	GePoint3dArray inters;

	GeRay3d thisLine(pRay->basePoint(), pRay->unitDir());

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{

			GeLineSeg3d argLine;
			pPolyline->getLineSegAt(i, argLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeRay3d line1(thisLine.pointOnLine(), thisLine.direction());
				GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
				GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeRay3d line1(thisLine.pointOnLine(), thisLine.direction());
				GeLine3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
				GeLine3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{

			GeCircArc3d argCircArc;
			pPolyline->getArcSegAt(i, argCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeRay3d line(thisLine.pointOnLine(), thisLine.direction());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeRay3d line(thisLine.pointOnLine(), thisLine.direction());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}

GePoint3dArray osnap_inters_line_to_lintseg(const DbXline* pXline, const DbLine* pLine, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLine3d thisLine(pXline->basePoint(), pXline->unitDir());
	GeLineSeg3d argLine(pLine->startPoint(), pLine->endPoint());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.startPoint(), argLine.endPoint());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_line_to_ray(const DbXline* pXline, const DbRay* pRay, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLine3d thisLine(pXline->basePoint(), pXline->unitDir());
	GeRay3d argLine(pRay->basePoint(), pRay->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeRay3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeRay3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_line_to_line(const DbXline* pXline1, const DbXline* pXline2, Db::Intersect intType)
{
	GePoint3dArray inters;
	if (pXline1 == pXline2) {
		return inters;
	}

	GeLine3d thisLine(pXline1->basePoint(), pXline1->unitDir());
	GeLine3d argLine(pXline2->basePoint(), pXline2->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
		GeLine3d line2(argLine.pointOnLine(), argLine.direction());
		GePoint3d p1;
		if (line1.intersectWith(line2, p1) == true)
		{
			inters.append(p1);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_line_to_circle(const DbXline* pXline, const DbCircle* pCircle, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLine3d thisLine(pXline->basePoint(), pXline->unitDir());
	GeCircArc3d argCircArc(pCircle->center(), pCircle->normal(), pCircle->radius());
	argCircArc.setAngles(0.0, PI * 2.0);

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_line_to_arc(const DbXline* pXline, const DbArc* pArc, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLine3d thisLine(pXline->basePoint(), pXline->unitDir());
	GeCircArc3d argCircArc(pArc->center(), pArc->normal(), pArc->radius());
	argCircArc.setAngles(pArc->startAngle(), pArc->endAngle());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
		GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_line_to_polyline(const DbXline* pXline, const DbPolyline* pPolyline, Db::Intersect intType)
{

	GePoint3dArray inters;

	GeLine3d thisLine(pXline->basePoint(), pXline->unitDir());

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{

			GeLineSeg3d argLine;
			pPolyline->getLineSegAt(i, argLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
				GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
				GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
				GeLine3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line1(thisLine.pointOnLine(), thisLine.direction());
				GeLine3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d intersect;
				if (line1.intersectWith(line2, intersect) == true)
				{
					inters.append(intersect);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{

			GeCircArc3d argCircArc;
			pPolyline->getArcSegAt(i, argCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line(thisLine.pointOnLine(), thisLine.direction());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}

GePoint3dArray osnap_inters_circle_to_lintseg(const DbCircle* pCircle, const DbLine* pLine, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc = GeCircArc3d(pCircle->center(), pCircle->normal(), pCircle->radius());
	thisCircArc.setAngles(0.0, PI * 2.0);
	GeLineSeg3d argLine = GeLineSeg3d(pLine->startPoint(), pLine->endPoint());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLine3d line(argLine.startPoint(), argLine.endPoint());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLine3d line(argLine.startPoint(), argLine.endPoint());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_circle_to_ray(const DbCircle* pCircle, const DbRay* pRay, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc = GeCircArc3d(pCircle->center(), pCircle->normal(), pCircle->radius());
	thisCircArc.setAngles(0.0, PI * 2.0);
	GeRay3d argLine = GeRay3d(pRay->basePoint(), pRay->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeRay3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeRay3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_circle_to_line(const DbCircle* pCircle, const DbXline* pXline, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc = GeCircArc3d(pCircle->center(), pCircle->normal(), pCircle->radius());
	thisCircArc.setAngles(0.0, PI * 2.0);
	GeLine3d argLine = GeLine3d(pXline->basePoint(), pXline->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_circle_to_circle(const DbCircle* pCircle1, const DbCircle* pCircle2, Db::Intersect intType)
{
	GePoint3dArray inters;
	if (pCircle1 == pCircle2) {
		return inters;
	}

	GeCircArc3d thisCircArc = GeCircArc3d(pCircle1->center(), pCircle1->normal(), pCircle1->radius());
	thisCircArc.setAngles(0.0, PI * 2.0);
	GeCircArc3d argCircArc = GeCircArc3d(pCircle2->center(), pCircle2->normal(), pCircle2->radius());
	argCircArc.setAngles(0.0, PI * 2.0);

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(0.0, PI * 2.0);
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(0.0, PI * 2.0);
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_circle_to_arc(const DbCircle* pCircle, const DbArc* pArc, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc = GeCircArc3d(pCircle->center(), pCircle->normal(), pCircle->radius());
	thisCircArc.setAngles(0.0, PI * 2.0);
	GeCircArc3d argCircArc = GeCircArc3d(pArc->center(), pArc->normal(), pArc->radius());
	argCircArc.setAngles(pArc->startAngle(), pArc->endAngle());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(0.0, PI * 2.0);
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(0.0, PI * 2.0);
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_circle_to_polyline(const DbCircle* pCircle, const DbPolyline* pPolyline, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc;
	thisCircArc.set(pCircle->center(), pCircle->normal(), pCircle->radius());
	thisCircArc.setAngles(0.0, PI * 2.0);

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{
			GeLineSeg3d argLine;
			pPolyline->getLineSegAt(i, argLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeLine3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				GeLine3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{

			GeCircArc3d argCircArc;
			pPolyline->getArcSegAt(i, argCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(0.0, PI * 2.0);
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(0.0, PI * 2.0);
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}

GePoint3dArray osnap_inters_arc_to_lintseg(const DbArc* pArc, const DbLine* pLine, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc = GeCircArc3d(pArc->center(), pArc->normal(), pArc->radius());
	thisCircArc.setAngles(pArc->startAngle(), pArc->endAngle());
	GeLineSeg3d argLine = GeLineSeg3d(pLine->startPoint(), pLine->endPoint());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLine3d line(argLine.startPoint(), argLine.endPoint());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLine3d line(argLine.startPoint(), argLine.endPoint());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_arc_to_ray(const DbArc* pArc, const DbRay* pRay, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc = GeCircArc3d(pArc->center(), pArc->normal(), pArc->radius());
	thisCircArc.setAngles(pArc->startAngle(), pArc->endAngle());
	GeRay3d argLine = GeRay3d(pRay->basePoint(), pRay->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeRay3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeRay3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_arc_to_line(const DbArc* pArc, const DbXline* pXline, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc = GeCircArc3d(pArc->center(), pArc->normal(), pArc->radius());
	thisCircArc.setAngles(pArc->startAngle(), pArc->endAngle());
	GeLine3d argLine = GeLine3d(pXline->basePoint(), pXline->unitDir());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc.setAngles(0.0, PI * 2.0);
		GeLine3d line(argLine.pointOnLine(), argLine.direction());
		int intn = 0;
		GePoint3d p1, p2;
		circArc.intersectWith(line, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_arc_to_circle(const DbArc* pArc, const DbCircle* pCircle2, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc = GeCircArc3d(pArc->center(), pArc->normal(), pArc->radius());
	thisCircArc.setAngles(pArc->startAngle(), pArc->endAngle());
	GeCircArc3d argCircArc = GeCircArc3d(pCircle2->center(), pCircle2->normal(), pCircle2->radius());
	argCircArc.setAngles(0.0, PI * 2.0);

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(0.0, PI * 2.0);
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(0.0, PI * 2.0);
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_arc_to_arc(const DbArc* pArc1, const DbArc* pArc2, Db::Intersect intType)
{
	GePoint3dArray inters;
	if (pArc1 == pArc2) {
		return inters;
	}

	GeCircArc3d thisCircArc = GeCircArc3d(pArc1->center(), pArc1->normal(), pArc1->radius());
	thisCircArc.setAngles(pArc1->startAngle(), pArc1->endAngle());
	GeCircArc3d argCircArc = GeCircArc3d(pArc2->center(), pArc2->normal(), pArc2->radius());
	argCircArc.setAngles(pArc2->startAngle(), pArc2->endAngle());

	if (intType == Db::Intersect::kOnBothOperands)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendThis)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(0.0, PI * 2.0);
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendArg)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}
	else if (intType == Db::Intersect::kExtendBoth)
	{
		GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
		circArc1.setAngles(0.0, PI * 2.0);
		GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
		circArc2.setAngles(0.0, PI * 2.0);
		int intn = 0;
		GePoint3d p1, p2;
		circArc1.intersectWith(circArc2, intn, p1, p2);
		if (intn == 1)
		{
			inters.append(p1);
		}
		else if (intn == 2)
		{
			inters.append(p1);
			inters.append(p2);
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_arc_to_polyline(const DbArc* pArc, const DbPolyline* pPolyline, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d thisCircArc;
	thisCircArc.set(pArc->center(), pArc->normal(), pArc->radius());
	thisCircArc.setAngles(pArc->startAngle(), pArc->endAngle());

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{

			GeLineSeg3d argLine;
			pPolyline->getLineSegAt(i, argLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeLine3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				GeLine3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{

			GeCircArc3d argCircArc;
			pPolyline->getArcSegAt(i, argCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(0.0, PI * 2.0);
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(0.0, PI * 2.0);
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}

GePoint3dArray osnap_inters_polyline_to_lintseg(const DbPolyline* pPolyline, const DbLine* pLine, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLineSeg3d argLine;
	argLine.set(pLine->startPoint(), pLine->endPoint());

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{

			GeLineSeg3d thisLine;
			pPolyline->getLineSegAt(i, thisLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.startPoint(), argLine.endPoint());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{

			GeCircArc3d thisCircArc;
			pPolyline->getArcSegAt(i, thisCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(0.0, PI * 2.0);
				GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeLine3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(0.0, PI * 2.0);
				GeLine3d line(argLine.startPoint(), argLine.endPoint());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_polyline_to_ray(const DbPolyline* pPolyline, const DbRay* pRay, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLineSeg3d argLine;
	argLine.set(pRay->basePoint(), pRay->unitDir());

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{

			GeLineSeg3d thisLine;
			pPolyline->getLineSegAt(i, thisLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeRay3d line2(argLine.pointOnLine(), argLine.direction());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeRay3d line2(argLine.pointOnLine(), argLine.direction());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.pointOnLine(), argLine.direction());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.pointOnLine(), argLine.direction());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{

			GeCircArc3d thisCircArc;
			pPolyline->getArcSegAt(i, thisCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeRay3d line(argLine.pointOnLine(), argLine.direction());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(0.0, PI * 2.0);
				GeRay3d line(argLine.pointOnLine(), argLine.direction());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeLine3d line(argLine.pointOnLine(), argLine.direction());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(0.0, PI * 2.0);
				GeLine3d line(argLine.pointOnLine(), argLine.direction());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_polyline_to_line(const DbPolyline* pPolyline, const DbXline* pXline, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeLine3d argLine;
	argLine.set(pXline->basePoint(), pXline->unitDir());

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{

			GeLineSeg3d thisLine;
			pPolyline->getLineSegAt(i, thisLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.pointOnLine(), argLine.direction());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.pointOnLine(), argLine.direction());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.pointOnLine(), argLine.direction());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
				GeLine3d line2(argLine.pointOnLine(), argLine.direction());
				GePoint3d p1;
				if (line1.intersectWith(line2, p1) == true)
				{
					inters.append(p1);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{

			GeCircArc3d thisCircArc;
			pPolyline->getArcSegAt(i, thisCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeLine3d line(argLine.pointOnLine(), argLine.direction());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(0.0, PI * 2.0);
				GeLine3d line(argLine.pointOnLine(), argLine.direction());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeLine3d line(argLine.pointOnLine(), argLine.direction());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				thisCircArc.setAngles(0.0, PI * 2.0);
				GeLine3d line(argLine.pointOnLine(), argLine.direction());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_polyline_to_circle(const DbPolyline* pPolyline, const DbCircle* pCircle, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d argCircArc;
	argCircArc.set(pCircle->center(), pCircle->normal(), pCircle->radius());
	argCircArc.setAngles(0.0, PI * 2.0);

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{

			GeLineSeg3d thisLine;
			pPolyline->getLineSegAt(i, thisLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), circArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), circArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{

			GeCircArc3d thisCircArc;
			pPolyline->getArcSegAt(i, thisCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(0.0, PI * 2.0);
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(0.0, PI * 2.0);
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_polyline_to_arc(const DbPolyline* pPolyline, const DbArc* pArc, Db::Intersect intType)
{
	GePoint3dArray inters;

	GeCircArc3d argCircArc;
	argCircArc.set(pArc->center(), pArc->normal(), pArc->radius());
	argCircArc.setAngles(pArc->startAngle(), pArc->endAngle());

	for (int i = 0; i < pPolyline->numVerts(); i++)
	{
		DbPolyline::SegType segType = pPolyline->segType(i);
		if (segType == DbPolyline::kLine)
		{

			GeLineSeg3d thisLine;
			pPolyline->getLineSegAt(i, thisLine);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), circArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(argCircArc.startAng(), circArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
				GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc.intersectWith(line, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
		else if (segType == DbPolyline::kArc)
		{

			GeCircArc3d thisCircArc;
			pPolyline->getArcSegAt(i, thisCircArc);

			if (intType == Db::Intersect::kOnBothOperands)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendThis)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(0.0, PI * 2.0);
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendArg)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
			else if (intType == Db::Intersect::kExtendBoth)
			{
				GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
				circArc1.setAngles(0.0, PI * 2.0);
				GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
				circArc2.setAngles(0.0, PI * 2.0);
				int intn = 0;
				GePoint3d p1, p2;
				circArc1.intersectWith(circArc2, intn, p1, p2);
				if (intn == 1)
				{
					inters.append(p1);
				}
				else if (intn == 2)
				{
					inters.append(p1);
					inters.append(p2);
				}
			}
		}
	}

	return inters;
}
GePoint3dArray osnap_inters_polyline_to_polyline(const DbPolyline* pPolyline1, const DbPolyline* pPolyline2, Db::Intersect intType)
{

	GePoint3dArray inters;

	if (pPolyline1 == pPolyline2)
	{
		for (int i = 0; i < pPolyline1->numVerts() - 1; i++)
		{
			DbPolyline::SegType segType = pPolyline1->segType(i);

			if (segType == DbPolyline::SegType::kLine)
			{

				GeLineSeg3d thisLine;
				pPolyline1->getLineSegAt(i, thisLine);

				for (int u = i + 1; u < pPolyline2->numVerts(); u++)
				{
					segType = pPolyline2->segType(u);
					if (segType == DbPolyline::SegType::kLine)
					{

						GeLineSeg3d argLine;
						pPolyline2->getLineSegAt(u, argLine);

						if (intType == Db::Intersect::kOnBothOperands)
						{
							GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
							GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
							GePoint3d p1;
							if (line1.intersectWith(line2, p1) == true)
							{
								inters.append(p1);
							}
						}
						else if (intType == Db::Intersect::kExtendThis)
						{
							GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
							GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
							GePoint3d p1;
							if (line1.intersectWith(line2, p1) == true)
							{
								inters.append(p1);
							}
						}
						else if (intType == Db::Intersect::kExtendArg)
						{
							GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
							GeLine3d line2(argLine.startPoint(), argLine.endPoint());
							GePoint3d p1;
							if (line1.intersectWith(line2, p1) == true)
							{
								inters.append(p1);
							}
						}
						else if (intType == Db::Intersect::kExtendBoth)
						{
							GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
							GeLine3d line2(argLine.startPoint(), argLine.endPoint());
							GePoint3d p1;
							if (line1.intersectWith(line2, p1) == true)
							{
								inters.append(p1);
							}
						}
					}
					else if (segType == DbPolyline::SegType::kArc)
					{

						GeCircArc3d argCircArc;
						pPolyline2->getArcSegAt(u, argCircArc);

						if (intType == Db::Intersect::kOnBothOperands)
						{
							GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
							GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendThis)
						{
							GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
							GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendArg)
						{
							GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
							GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc.setAngles(0.0, PI * 2.0);
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendBoth)
						{
							GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
							GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc.setAngles(0.0, PI * 2.0);
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
					}
				}
			}
			else if (segType == DbPolyline::SegType::kArc)
			{

				GeCircArc3d thisCircArc;
				pPolyline1->getArcSegAt(i, thisCircArc);

				for (int u = i + 1; u < pPolyline2->numVerts(); u++)
				{
					segType = pPolyline2->segType(u);
					if (segType == DbPolyline::SegType::kLine)
					{

						GeLineSeg3d argLine;
						pPolyline2->getLineSegAt(u, argLine);

						if (intType == Db::Intersect::kOnBothOperands)
						{
							GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
							GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendThis)
						{
							GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc.setAngles(0.0, PI * 2.0);
							GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendArg)
						{
							GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
							GeLine3d line(argLine.startPoint(), argLine.endPoint());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendBoth)
						{
							GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc.setAngles(0.0, PI * 2.0);
							GeLine3d line(argLine.startPoint(), argLine.endPoint());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
					}
					else if (segType == DbPolyline::SegType::kArc)
					{

						GeCircArc3d argCircArc;
						pPolyline2->getArcSegAt(u, argCircArc);

						if (intType == Db::Intersect::kOnBothOperands)
						{
							GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
							GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
							int intn = 0;
							GePoint3d p1, p2;
							circArc1.intersectWith(circArc2, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendThis)
						{
							GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc1.setAngles(0.0, PI * 2.0);
							GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
							int intn = 0;
							GePoint3d p1, p2;
							circArc1.intersectWith(circArc2, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendArg)
						{
							GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc2.setAngles(0.0, PI * 2.0);
							int intn = 0;
							GePoint3d p1, p2;
							circArc1.intersectWith(circArc2, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendBoth)
						{
							GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc1.setAngles(0.0, PI * 2.0);
							GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc2.setAngles(0.0, PI * 2.0);
							int intn = 0;
							GePoint3d p1, p2;
							circArc1.intersectWith(circArc2, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < pPolyline1->numVerts(); i++)
		{
			DbPolyline::SegType segType = pPolyline1->segType(i);

			if (segType == DbPolyline::SegType::kLine)
			{

				GeLineSeg3d thisLine;
				pPolyline1->getLineSegAt(i, thisLine);

				for (int u = 0; u < pPolyline2->numVerts(); u++)
				{
					segType = pPolyline2->segType(u);
					if (segType == DbPolyline::SegType::kLine)
					{

						GeLineSeg3d argLine;
						pPolyline2->getLineSegAt(u, argLine);

						if (intType == Db::Intersect::kOnBothOperands)
						{
							GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
							GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
							GePoint3d p1;
							if (line1.intersectWith(line2, p1) == true)
							{
								inters.append(p1);
							}
						}
						else if (intType == Db::Intersect::kExtendThis)
						{
							GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
							GeLineSeg3d line2(argLine.startPoint(), argLine.endPoint());
							GePoint3d p1;
							if (line1.intersectWith(line2, p1) == true)
							{
								inters.append(p1);
							}
						}
						else if (intType == Db::Intersect::kExtendArg)
						{
							GeLineSeg3d line1(thisLine.startPoint(), thisLine.endPoint());
							GeLine3d line2(argLine.startPoint(), argLine.endPoint());
							GePoint3d p1;
							if (line1.intersectWith(line2, p1) == true)
							{
								inters.append(p1);
							}
						}
						else if (intType == Db::Intersect::kExtendBoth)
						{
							GeLine3d line1(thisLine.startPoint(), thisLine.endPoint());
							GeLine3d line2(argLine.startPoint(), argLine.endPoint());
							GePoint3d p1;
							if (line1.intersectWith(line2, p1) == true)
							{
								inters.append(p1);
							}
						}
					}
					else if (segType == DbPolyline::SegType::kArc)
					{

						GeCircArc3d argCircArc;
						pPolyline2->getArcSegAt(u, argCircArc);

						if (intType == Db::Intersect::kOnBothOperands)
						{
							GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
							GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendThis)
						{
							GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
							GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc.setAngles(argCircArc.startAng(), argCircArc.endAng());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendArg)
						{
							GeLineSeg3d line(thisLine.startPoint(), thisLine.endPoint());
							GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc.setAngles(0.0, PI * 2.0);
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendBoth)
						{
							GeLine3d line(thisLine.startPoint(), thisLine.endPoint());
							GeCircArc3d circArc(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc.setAngles(0.0, PI * 2.0);
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
					}
				}
			}
			else if (segType == DbPolyline::SegType::kArc)
			{

				GeCircArc3d thisCircArc;
				pPolyline1->getArcSegAt(i, thisCircArc);

				for (int u = 0; u < pPolyline2->numVerts(); u++)
				{
					segType = pPolyline2->segType(u);
					if (segType == DbPolyline::SegType::kLine)
					{

						GeLineSeg3d argLine;
						pPolyline2->getLineSegAt(u, argLine);

						if (intType == Db::Intersect::kOnBothOperands)
						{
							GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
							GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendThis)
						{
							GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc.setAngles(0.0, PI * 2.0);
							GeLineSeg3d line(argLine.startPoint(), argLine.endPoint());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendArg)
						{
							GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
							GeLine3d line(argLine.startPoint(), argLine.endPoint());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendBoth)
						{
							GeCircArc3d circArc(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc.setAngles(0.0, PI * 2.0);
							GeLine3d line(argLine.startPoint(), argLine.endPoint());
							int intn = 0;
							GePoint3d p1, p2;
							circArc.intersectWith(line, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
					}
					else if (segType == DbPolyline::SegType::kArc)
					{

						GeCircArc3d argCircArc;
						pPolyline2->getArcSegAt(u, argCircArc);

						if (intType == Db::Intersect::kOnBothOperands)
						{
							GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc1.setAngles(thisCircArc.startAng(), thisCircArc.endAng());
							GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
							int intn = 0;
							GePoint3d p1, p2;
							circArc1.intersectWith(circArc2, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendThis)
						{
							GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc1.setAngles(0.0, PI * 2.0);
							GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc2.setAngles(argCircArc.startAng(), argCircArc.endAng());
							int intn = 0;
							GePoint3d p1, p2;
							circArc1.intersectWith(circArc2, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendArg)
						{
							GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc2.setAngles(0.0, PI * 2.0);
							int intn = 0;
							GePoint3d p1, p2;
							circArc1.intersectWith(circArc2, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
						else if (intType == Db::Intersect::kExtendBoth)
						{
							GeCircArc3d circArc1(thisCircArc.center(), thisCircArc.normal(), thisCircArc.radius());
							circArc1.setAngles(0.0, PI * 2.0);
							GeCircArc3d circArc2(argCircArc.center(), argCircArc.normal(), argCircArc.radius());
							circArc2.setAngles(0.0, PI * 2.0);
							int intn = 0;
							GePoint3d p1, p2;
							circArc1.intersectWith(circArc2, intn, p1, p2);
							if (intn == 1)
							{
								inters.append(p1);
							}
							else if (intn == 2)
							{
								inters.append(p1);
								inters.append(p2);
							}
						}
					}
				}
			}
		}
	}

	return inters;
}
