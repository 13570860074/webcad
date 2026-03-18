#include "ReRenderer.h"
#include "DbObjectId.h"
#include "GiEntity.h"
#include "ReImpl.h"


ReRenderer::ReRenderer() {
	this->m_pImpl = new ReRendererIpml();
}
ReRenderer::~ReRenderer() {

}

bool ReRenderer::isWorkArea() {
	return RE_IMP_RENDERER(this->m_pImpl)->isWorkArea;
}
DbObjectId ReRenderer::viewport()const {
	return RE_IMP_RENDERER(this->m_pImpl)->viewport;
}
void ReRenderer::setViewport(const DbObjectId& id) {
	RE_IMP_RENDERER(this->m_pImpl)->viewport = id;
}
ReCamera * ReRenderer::camera() {
	return RE_IMP_RENDERER(this->m_pImpl)->camera;
}
void ReRenderer::setCamera(ReCamera* camera) {
	RE_IMP_RENDERER(this->m_pImpl)->camera = camera;
}
unsigned int ReRenderer::numEntity() {
	return RE_IMP_RENDERER(this->m_pImpl)->numEntity;
}

#if EMSDK
emscripten::val ReRenderer::entityIds() {
	return val(typed_memory_view(RE_IMP_RENDERER(this->m_pImpl)->numEntity, RE_IMP_RENDERER(this->m_pImpl)->entityIds));
}
#else
unsigned int* ReRenderer::entityIds() {
	return RE_IMP_RENDERER(this->m_pImpl)->entityIds;
}
#endif
void ReRenderer::setEntitys(AcArray<GiEntity*>* entitys) {
	if (RE_IMP_RENDERER(this->m_pImpl)->entityIds != NULL) {
		delete[]RE_IMP_RENDERER(this->m_pImpl)->entityIds;
		RE_IMP_RENDERER(this->m_pImpl)->entityIds = NULL;
	}
	RE_IMP_RENDERER(this->m_pImpl)->numEntity = entitys->length();
	RE_IMP_RENDERER(this->m_pImpl)->entityIds = new unsigned int[RE_IMP_RENDERER(this->m_pImpl)->numEntity];
	for (int i = 0; i < entitys->length(); i++) {
		RE_IMP_RENDERER(this->m_pImpl)->entityIds[i] = entitys->at(i)->objectId().asOldId();
	}
}