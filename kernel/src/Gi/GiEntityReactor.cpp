#include "GiEntity.h"
#include "GiEntityReactor.h"
#include "GiImpl.h"


enum GiEntityReactorEvent
{
	kCreateEntity,
	kRemoveEntity,
	kUpdateVertexs,
	kTransformation,
	kVisibility,
};



void GiEntityReactor::createEntity(GiEntity* pEntity) {
	GI_IMP_GIENTITYREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(GiEntityReactorEvent::kCreateEntity, pEntity);
}
void GiEntityReactor::updateVertexs(GiEntity* pEntity) {
	GI_IMP_GIENTITYREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(GiEntityReactorEvent::kUpdateVertexs, pEntity);
}
void GiEntityReactor::transformation(GiEntity* pEntity) {
	GI_IMP_GIENTITYREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(GiEntityReactorEvent::kTransformation, pEntity);
}
void GiEntityReactor::setVisibility(GiEntity* pEntity, bool isVisibility) {
	GI_IMP_GIENTITYREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(GiEntityReactorEvent::kVisibility, pEntity, isVisibility);
}



#if EMSDK
GiEntityReactor::GiEntityReactor() {
	this->m_pImpl = new GiEntityReactorImpl();
}
GiEntityReactor::~GiEntityReactor() {

}

void GiEntityReactor::onCreateEntity(int ptr) {
	GI_IMP_GIENTITYREACTOR(this->m_pImpl)->pEmsdkEvent->on(GiEntityReactorEvent::kCreateEntity, ptr);
}
void GiEntityReactor::onUpdateVertexs(int ptr) {
	GI_IMP_GIENTITYREACTOR(this->m_pImpl)->pEmsdkEvent->on(GiEntityReactorEvent::kUpdateVertexs, ptr);
}
void GiEntityReactor::onTransformation(int ptr) {
	GI_IMP_GIENTITYREACTOR(this->m_pImpl)->pEmsdkEvent->on(GiEntityReactorEvent::kTransformation, ptr);
}
void GiEntityReactor::onVisibility(int ptr) {
	GI_IMP_GIENTITYREACTOR(this->m_pImpl)->pEmsdkEvent->on(GiEntityReactorEvent::kVisibility, ptr);
}
#endif
