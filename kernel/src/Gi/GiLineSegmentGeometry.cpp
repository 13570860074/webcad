#include "GiLineSegmentGeometry.h"
#include "GiImpl.h"

GiLineSegmentGeometry::GiLineSegmentGeometry()
{
	this->m_pImpl = new GiLineSegmentGeometryImpl();
}
GiLineSegmentGeometry::~GiLineSegmentGeometry()
{
}

Gi::EntityGeometryType GiLineSegmentGeometry::type()
{
	return Gi::EntityGeometryType::kLineSegmentGeometry;
}
GiLineSegmentGeometry* GiLineSegmentGeometry::clone() const {
	GiLineSegmentGeometry* p = new GiLineSegmentGeometry();
	GiEntityGeometry::clone((GiEntityGeometry*&)p);
	p->setStartPoint(this->startPoint());
	p->setEndPoint(this->endPoint());
	return p;
}
GePoint3d GiLineSegmentGeometry::startPoint() const
{
	return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint;
}

void GiLineSegmentGeometry::setStartPoint(const GePoint3d &point)
{
	GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint = point;
}

GePoint3d GiLineSegmentGeometry::endPoint() const
{
	return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint;
}

void GiLineSegmentGeometry::setEndPoint(const GePoint3d &point)
{
	GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint = point;
}

GePoint2d GiLineSegmentGeometry::displayCoord(unsigned char index) const
{
	if (index == 0)
	{
		return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPointDisplayCoord;
	}
	else if (index == 1)
	{
		return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPointDisplayCoord;
	}
	return GePoint2d(0, 0);
}
void GiLineSegmentGeometry::setDisplayCoord(unsigned char index, const GePoint2d &pt) const
{
	if (index == 0)
	{
		GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPointDisplayCoord = pt;
	}
	else if (index == 1)
	{
		GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPointDisplayCoord = pt;
	}
}

double GiLineSegmentGeometry::xMin()
{
	if (GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.x < GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.x)
	{
		return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.x;
	}
	return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.x;
}
double GiLineSegmentGeometry::xMax()
{
	if (GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.x > GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.x)
	{
		return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.x;
	}
	return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.x;
}
double GiLineSegmentGeometry::yMin()
{
	if (GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.y < GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.y)
	{
		return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.y;
	}
	return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.y;
}
double GiLineSegmentGeometry::yMax()
{
	if (GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.y > GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.y)
	{
		return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.y;
	}
	return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.y;
}
double GiLineSegmentGeometry::zMin()
{
	if (GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.z < GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.z)
	{
		return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.z;
	}
	return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.z;
}
double GiLineSegmentGeometry::zMax()
{
	if (GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.z > GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.z)
	{
		return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->startPoint.z;
	}
	return GI_IMP_LINESEGMENTGEOMETRY(this->m_pImpl)->endPoint.z;
}