#include "GiPointGeometry.h"
#include "GiImpl.h"

GiPointGeometry::GiPointGeometry()
{
	this->m_pImpl = new GiPointGeometryImpl();
}
GiPointGeometry::~GiPointGeometry()
{
}
GiPointGeometry* GiPointGeometry::clone() const {
	GiPointGeometry* p = new GiPointGeometry();
	GiEntityGeometry::clone((GiEntityGeometry*&)p);
	p->setPosition(this->position());
	return p;
}
Gi::EntityGeometryType GiPointGeometry::type()
{
	return Gi::EntityGeometryType::kPointGeometry;
}
GePoint3d GiPointGeometry::position() const
{
	return GI_IMP_POINTGEOMETRY(this->m_pImpl)->position;
}
void GiPointGeometry::setPosition(const GePoint3d &_vertex)
{
	GI_IMP_POINTGEOMETRY(this->m_pImpl)->position.set(_vertex.x, _vertex.y, _vertex.z);
}
GePoint2d GiPointGeometry::displayCoord(unsigned char index) const
{
	return GI_IMP_POINTGEOMETRY(this->m_pImpl)->positionDisplayCoord;
}
void GiPointGeometry::setDisplayCoord(unsigned char index, const GePoint2d &pt) const
{
	GI_IMP_POINTGEOMETRY(this->m_pImpl)->positionDisplayCoord = pt;
}
