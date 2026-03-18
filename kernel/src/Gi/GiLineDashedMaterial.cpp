#include "GiLineDashedMaterial.h"
#include "CmColor.h"
#include "GiImpl.h"


GiLineDashedMaterial::GiLineDashedMaterial()
{
	this->m_pImpl = new GiLineDashedMaterialImpl();
}
GiLineDashedMaterial::~GiLineDashedMaterial()
{

}

Gi::MaterialType GiLineDashedMaterial::type() const {
	return Gi::MaterialType::kLineDashed;
}
double GiLineDashedMaterial::dashSize() const {
	return GI_IMP_LINEDASHEDMATERIAL(this->m_pImpl)->dashSize;
}
void GiLineDashedMaterial::setDashSize(const double val) {
	GI_IMP_LINEDASHEDMATERIAL(this->m_pImpl)->dashSize = val;
}

double GiLineDashedMaterial::gapSize() const {
	return GI_IMP_LINEDASHEDMATERIAL(this->m_pImpl)->gapSize;
}
void GiLineDashedMaterial::setGapSize(const double val) {
	GI_IMP_LINEDASHEDMATERIAL(this->m_pImpl)->gapSize = val;
}
double GiLineDashedMaterial::lineWidth() const {
	return GI_IMP_LINEDASHEDMATERIAL(this->m_pImpl)->lineWidth;
}
void GiLineDashedMaterial::setLineWidth(const double _lineWidth) {
	GI_IMP_LINEDASHEDMATERIAL(this->m_pImpl)->lineWidth = _lineWidth;
}