#ifndef GILINESEGMENTGEOMETRY_H
#define GILINESEGMENTGEOMETRY_H

#include "GePoint3d.h"
#include "GePoint2d.h"
#include "GiEntityGeometry.h"
#include "GiEntityGeometryStake.h"

class GiLineSegmentGeometry : public GiEntityGeometry
{
public:
	GiLineSegmentGeometry();
	virtual ~GiLineSegmentGeometry();
	RX_DECLARE_MEMBERS(GiLineSegmentGeometry);

public:
	virtual Gi::EntityGeometryType type();

	virtual GiLineSegmentGeometry* clone()const;

	GePoint3d startPoint() const;
	void setStartPoint(const GePoint3d &point);

	GePoint3d endPoint() const;
	void setEndPoint(const GePoint3d &point);

	virtual GePoint2d displayCoord(unsigned char index) const;
	virtual void setDisplayCoord(unsigned char index, const GePoint2d &pt) const;

	double xMin();
	double xMax();
	double yMin();
	double yMax();
	double zMin();
	double zMax();
};

#endif
