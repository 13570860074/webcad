#include "GiPointMaterial.h"
#include "CmColor.h"
#include "GiImpl.h"


GiPointMaterial::GiPointMaterial()
{
	this->m_pImpl = new GiPointMaterialImpl();
}
GiPointMaterial::~GiPointMaterial()
{

}

Gi::MaterialType GiPointMaterial::type() const {
	return Gi::MaterialType::kPointBasic;
}
unsigned char GiPointMaterial::size()const {
	return GI_IMP_POINTMATERIAL(this->m_pImpl)->size;
}
void GiPointMaterial::setSize(unsigned char _size) {
	GI_IMP_POINTMATERIAL(this->m_pImpl)->size = _size;
}