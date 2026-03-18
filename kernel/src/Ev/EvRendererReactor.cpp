
#include "EvRendererReactor.h"
#include "EvEventManager.h"
#include "ReCamera.h"
#include "kernel.h"

EvRendererReactor::EvRendererReactor()
{
}
EvRendererReactor::~EvRendererReactor()
{
}

void EvRendererReactor::beginAnimationFrame()
{
	::kernel()->acevEventManager()->_emit(Ev::RendererReactor::kBeginAnimationFrame);
}
void EvRendererReactor::endAnimationFrame()
{
	::kernel()->acevEventManager()->_emit(Ev::RendererReactor::kEndAnimationFrame);
}
