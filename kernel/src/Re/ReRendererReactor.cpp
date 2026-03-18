#include "ReRendererReactor.h"
#include "ReImpl.h"

enum ReRendererReactorEvent
{
	kBeginAnimationFrame,
	kEndAnimationFrame,
	kBeginRenderer,
	kEndRenderer,
	kClearCanvas,
	kClearWorkCanvas,
	kBeginRendererViewport,
	kEndRendererViewport,
	kRendererViewport,
};

void ReRendererReactor::beginAnimationFrame()
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(ReRendererReactorEvent::kBeginAnimationFrame);
}
void ReRendererReactor::endAnimationFrame()
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(ReRendererReactorEvent::kEndAnimationFrame);
}
void ReRendererReactor::beginRenderer()
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(ReRendererReactorEvent::kBeginRenderer);
}
void ReRendererReactor::endRenderer()
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(ReRendererReactorEvent::kEndRenderer);
}
void ReRendererReactor::clearWorkCanvas() {
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(ReRendererReactorEvent::kClearWorkCanvas);
}
void ReRendererReactor::clearCanvas()
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(ReRendererReactorEvent::kClearCanvas);
}
void ReRendererReactor::beginRendererViewport(DbObject *_pViewport)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(ReRendererReactorEvent::kBeginRendererViewport, _pViewport);
}
void ReRendererReactor::endRendererViewport(DbObject *_pViewport)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(ReRendererReactorEvent::kEndRendererViewport, _pViewport);
}
void ReRendererReactor::rendererViewport(ReRenderer* _pRenderer)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(ReRendererReactorEvent::kRendererViewport, _pRenderer);
}

#if EMSDK
ReRendererReactor::ReRendererReactor()
{
	this->m_pImpl = new ReRendererReactorImpl();
}
ReRendererReactor::~ReRendererReactor()
{
}
void ReRendererReactor::onBeginAnimationFrame(int ptr)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->on(ReRendererReactorEvent::kBeginAnimationFrame, ptr);
}
void ReRendererReactor::onEndAnimationFrame(int ptr)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->on(ReRendererReactorEvent::kEndAnimationFrame, ptr);
}
void ReRendererReactor::onBeginRenderer(int ptr)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->on(ReRendererReactorEvent::kBeginRenderer, ptr);
}
void ReRendererReactor::onEndRenderer(int ptr)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->on(ReRendererReactorEvent::kEndRenderer, ptr);
}
void ReRendererReactor::onClearCanvas(int ptr)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->on(ReRendererReactorEvent::kClearCanvas, ptr);
}
void ReRendererReactor::onClearWorkCanvas(int ptr) {
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->on(ReRendererReactorEvent::kClearWorkCanvas, ptr);
}
void ReRendererReactor::onBeginRendererViewport(int ptr)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->on(ReRendererReactorEvent::kBeginRendererViewport, ptr);
}
void ReRendererReactor::onEndRendererViewport(int ptr)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->on(ReRendererReactorEvent::kEndRendererViewport, ptr);
}
void ReRendererReactor::onRendererViewport(int ptr)
{
	RE_IMP_RERENDERERREACTOR(this->m_pImpl)->pEmsdkEvent->on(ReRendererReactorEvent::kRendererViewport, ptr);
}
#endif
