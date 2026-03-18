#include "GiMeshMaterial.h"
#include "CmColor.h"
#include "GiImpl.h"


GiMeshMaterial::GiMeshMaterial()
{
	this->m_pImpl = new GiMeshMaterialImpl();
}
GiMeshMaterial::~GiMeshMaterial()
{

}

Gi::MaterialType GiMeshMaterial::type() const {
	return Gi::MaterialType::kMeshBasic;
}
