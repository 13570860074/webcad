#include "GiLinePixelDashedMaterial.h"
#include "CmColor.h"
#include "GiImpl.h"


GiLinePixelDashedMaterial::GiLinePixelDashedMaterial()
{
	this->m_pImpl = new GiLinePixelDashedMaterialImpl();
}
GiLinePixelDashedMaterial::~GiLinePixelDashedMaterial()
{

}

Gi::MaterialType GiLinePixelDashedMaterial::type() const {
	return Gi::MaterialType::kLinePixelDashed;
}
double GiLinePixelDashedMaterial::pixelRatio() const {
	return GI_IMP_LINEPIXELDASHEDMATERIAL(this->m_pImpl)->pixelRatio;
}
void GiLinePixelDashedMaterial::setPixelRatio(double val) {
	GI_IMP_LINEPIXELDASHEDMATERIAL(this->m_pImpl)->pixelRatio = val;
}
double GiLinePixelDashedMaterial::dashSize() const{
	return GI_IMP_LINEPIXELDASHEDMATERIAL(this->m_pImpl)->dashSize;
}
void GiLinePixelDashedMaterial::setDashSize(const double val) {
	GI_IMP_LINEPIXELDASHEDMATERIAL(this->m_pImpl)->dashSize = val;
}

double GiLinePixelDashedMaterial::gapSize() const {
	return GI_IMP_LINEPIXELDASHEDMATERIAL(this->m_pImpl)->gapSize;
}
void GiLinePixelDashedMaterial::setGapSize(const double val) {
	GI_IMP_LINEPIXELDASHEDMATERIAL(this->m_pImpl)->gapSize = val;
}
double GiLinePixelDashedMaterial::lineWidth() const {
	return GI_IMP_LINEPIXELDASHEDMATERIAL(this->m_pImpl)->lineWidth;
}
void GiLinePixelDashedMaterial::setLineWidth(const double _lineWidth) {
	GI_IMP_LINEPIXELDASHEDMATERIAL(this->m_pImpl)->lineWidth = _lineWidth;
}