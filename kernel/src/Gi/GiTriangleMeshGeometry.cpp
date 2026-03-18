#include "GiTriangleMeshGeometry.h"
#include "GiImpl.h"

GiTriangleMeshGeometry::GiTriangleMeshGeometry()
{
	this->m_pImpl = new GiTriangleMeshGeometryImpl();
}
GiTriangleMeshGeometry::~GiTriangleMeshGeometry()
{
}
GiTriangleMeshGeometry* GiTriangleMeshGeometry::clone() const {
	GiTriangleMeshGeometry* p = new GiTriangleMeshGeometry();
	GiEntityGeometry::clone((GiEntityGeometry*&)p);
	p->setVertex(0, this->vertex(0));
	p->setVertex(1, this->vertex(1));
	p->setVertex(2, this->vertex(2));
	return p;
}
Gi::EntityGeometryType GiTriangleMeshGeometry::type()
{
	return Gi::EntityGeometryType::kTriangleMeshGeometry;
}
GePoint3d GiTriangleMeshGeometry::vertex(int _index) const
{
	if (_index == 0)
	{
		return GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->vertex1;
	}
	else if (_index == 1)
	{
		return GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->vertex2;
	}
	return GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->vertex3;
}
void GiTriangleMeshGeometry::setVertex(int _index, const GePoint3d &_vertex)
{
	if (_index == 0)
	{
		GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->vertex1.set(_vertex.x, _vertex.y, _vertex.z);
	}
	else if (_index == 1)
	{
		GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->vertex2.set(_vertex.x, _vertex.y, _vertex.z);
	}
	else if (_index == 2)
	{
		GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->vertex3.set(_vertex.x, _vertex.y, _vertex.z);
	}
}
GePoint2d GiTriangleMeshGeometry::displayCoord(unsigned char index) const
{
	if (index == 0)
	{
		return GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->displayCoordVertex1;
	}
	else if (index == 1)
	{
		return GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->displayCoordVertex2;
	}
	else if (index == 2)
	{
		return GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->displayCoordVertex3;
	}
	return GePoint2d(0, 0);
}
void GiTriangleMeshGeometry::setDisplayCoord(unsigned char index, const GePoint2d &pt) const
{
	if (index == 0)
	{
		GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->displayCoordVertex1 = pt;
	}
	else if (index == 1)
	{
		GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->displayCoordVertex2 = pt;
	}
	else if (index == 2)
	{
		GI_IMP_TRIANGLEMESHGEOMETRY(this->m_pImpl)->displayCoordVertex3 = pt;
	}
}
