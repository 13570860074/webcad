#ifndef GIVIEWPORTGEOMETRY_H
#define GIVIEWPORTGEOMETRY_H

#include "GiGeometry.h"

class GiViewportGeometry : public GiGeometry
{
public:
	GiViewportGeometry();
	virtual ~GiViewportGeometry();

	// 在眼睛坐标系（Eye Coordinates）中绘制折线
	virtual void polylineEye(const Adesk::UInt32 nbPoints, const GePoint3d* pPoints) = 0;

	// 在眼睛坐标系中绘制多边形
	virtual void polygonEye(const Adesk::UInt32 nbPoints, const GePoint3d* pPoints) = 0;

	// 在设备坐标系（Device Coordinates）中绘制折线
	virtual void polylineDc(const Adesk::UInt32 nbPoints, const GePoint3d* pPoints) = 0;

	// 在设备坐标系中绘制多边形
	virtual void polygonDc(const Adesk::UInt32 nbPoints, const GePoint3d* pPoints) = 0;
};



#endif
