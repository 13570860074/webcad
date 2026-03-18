#include "GiMaterial.h"
#include "GiMaterialReactor.h"
#include "GiImpl.h"


enum GiMaterialReactorEvent
{
	kCreateMaterial,
	kRemoveMaterial,
};



void GiMaterialReactor::createMaterial(GiMaterial* pMaterial) {
	GI_IMP_GIMATERIALREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(GiMaterialReactorEvent::kCreateMaterial, pMaterial);
}
void GiMaterialReactor::removeMaterial(GiObjectId id) {
	GI_IMP_GIMATERIALREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(GiMaterialReactorEvent::kRemoveMaterial, id);
}


#if EMSDK
GiMaterialReactor::GiMaterialReactor() {
	this->m_pImpl = new GiMaterialReactorImpl();
}
GiMaterialReactor::~GiMaterialReactor() {

}
void GiMaterialReactor::onCreateMaterial(int ptr) {
	GI_IMP_GIMATERIALREACTOR(this->m_pImpl)->pEmsdkEvent->on(GiMaterialReactorEvent::kCreateMaterial, ptr);
}
void GiMaterialReactor::onRemoveMaterial(int ptr) {
	GI_IMP_GIMATERIALREACTOR(this->m_pImpl)->pEmsdkEvent->on(GiMaterialReactorEvent::kRemoveMaterial, ptr);
}
#endif
