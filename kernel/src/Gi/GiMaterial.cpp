#include "GiMaterial.h"
#include "GiEntity.h"
#include "CmColor.h"
#include "GiImpl.h"



GiMaterial::~GiMaterial()
{

}

Gi::MaterialType GiMaterial::type() const {
	return  Gi::MaterialType::kUnknownMaterial;
}
unsigned int GiMaterial::color() const {
	return GI_IMP_MATERIAL(this->m_pImpl)->color;
}
unsigned char GiMaterial::transparency() const {
	return GI_IMP_MATERIAL(this->m_pImpl)->transparency;
}
unsigned char GiMaterial::colorIndex() const {
	return GI_IMP_MATERIAL(this->m_pImpl)->colorIndex;
}


void GiMaterial::setColor(const unsigned int _color) {
	GI_IMP_MATERIAL(this->m_pImpl)->color = _color;

	std::vector<int> rgb = CmColorBase::converArgbToRgb(_color);
	CmColor color;
	color.setRGB(rgb[0], rgb[1], rgb[2]);
	GI_IMP_MATERIAL(this->m_pImpl)->colorIndex = color.colorIndex();
}
void GiMaterial::setTransparency(unsigned char _transparency) {
	GI_IMP_MATERIAL(this->m_pImpl)->transparency = _transparency;
}
void GiMaterial::setColorIndex(unsigned char _colorIndex) {
	GI_IMP_MATERIAL(this->m_pImpl)->colorIndex = _colorIndex;

	CmColor color;
	color.setColorIndex(_colorIndex);
	GI_IMP_MATERIAL(this->m_pImpl)->color = CmColorBase::converRgbToArgb(color.red(), color.green(), color.blue());
}
