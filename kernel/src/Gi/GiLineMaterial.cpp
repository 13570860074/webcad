#include "GiLineMaterial.h"
#include "CmColor.h"
#include "GiImpl.h"


GiLineMaterial::GiLineMaterial()
{
	this->m_pImpl = new GiLineMaterialImpl();
}
GiLineMaterial::~GiLineMaterial()
{

}

Gi::MaterialType GiLineMaterial::type()const {
	return Gi::MaterialType::kLineBasic;
}
double GiLineMaterial::lineWidth()const {
	return GI_IMP_LINEMATERIAL(this->m_pImpl)->lineWidth;
}
void GiLineMaterial::setLineWidth(const double _lineWidth) {
	GI_IMP_LINEMATERIAL(this->m_pImpl)->lineWidth = _lineWidth;
}