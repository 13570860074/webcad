#ifndef GIPOINTGEOMETRY_H
#define GIPOINTGEOMETRY_H

#include "GePoint3d.h"
#include "GiEntityGeometry.h"

class GiPointGeometry : public GiEntityGeometry
{
public:
	GiPointGeometry();
	virtual ~GiPointGeometry();
	RX_DECLARE_MEMBERS(GiPointGeometry);

public:
	/* 类型 */
	virtual Gi::EntityGeometryType type();

	/* clone */
	virtual GiPointGeometry* clone() const;

	/* 获得坐标 */
	GePoint3d position() const;

	/* 设置坐标 */
	void setPosition(const GePoint3d &_vertex);

	GePoint2d displayCoord(unsigned char index) const;
	void setDisplayCoord(unsigned char index, const GePoint2d &pt) const;
};

#endif