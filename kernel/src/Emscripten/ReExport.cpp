#include "export.h"
#include "RxExport.h"
#include "AcIterator.h"
#include "RxObject.h"
#include "ReRenderer.h"
#include "ReRendererReactor.h"
#include "ReRendererManager.h"
#include "ReCamera.h"
#include "ReOrthogonalCamera.h"


#if EMSDK




EMSCRIPTEN_BINDINGS(Re) {


	//ReCamera
	emscripten::class_<ReCamera, base<RxObject>>("ReCamera")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(ReCamera)
		.function("information", &ReCamera::information)
		.function("matrix", &ReCamera::matrix)
		.function("setMatrix", &ReCamera::setMatrix);

	//ReOrthogonalCamera
	emscripten::class_<ReOrthogonalCamera, base<ReCamera>>("ReOrthogonalCamera")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(ReOrthogonalCamera)
		.function("information", &ReOrthogonalCamera::information)
		.function("top", &ReOrthogonalCamera::top)
		.function("bottom", &ReOrthogonalCamera::bottom)
		.function("left", &ReOrthogonalCamera::left)
		.function("right", &ReOrthogonalCamera::right)
		.function("setTop", &ReOrthogonalCamera::setTop)
		.function("setBottom", &ReOrthogonalCamera::setBottom)
		.function("setLeft", &ReOrthogonalCamera::setLeft)
		.function("setRight", &ReOrthogonalCamera::setRight)
		.function("lowerLeftCorner", &ReOrthogonalCamera::lowerLeftCorner)
		.function("setLowerLeftCorner", &ReOrthogonalCamera::setLowerLeftCorner)
		.function("upperRightCorner", &ReOrthogonalCamera::upperRightCorner)
		.function("setUpperRightCorner", &ReOrthogonalCamera::setUpperRightCorner);

	//ReRenderer
	emscripten::class_<ReRenderer, base<RxObject>>("ReRenderer")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(ReRenderer)
		.function("isWorkArea", &ReRenderer::isWorkArea)
		.function("camera", &ReRenderer::camera, allow_raw_pointers())
		.function("setCamera", &ReRenderer::setCamera, allow_raw_pointers());

	//ReRendererManager
	emscripten::class_<ReRendererManager, base<RxObject>>("ReRendererManager")
		.constructor<>()
		.function("animationFrame", select_overload<void()>(&ReRendererManager::animationFrame))
		.function("addReactor", &ReRendererManager::addReactor, allow_raw_pointers());

	//ReRendererReactor
	emscripten::class_<ReRendererReactor, base<RxObject>>("ReRendererReactor")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(ReRendererReactor)
		.function("onBeginAnimationFrame", select_overload<void(int)>(&ReRendererReactor::onBeginAnimationFrame), allow_raw_pointers())
		.function("onEndAnimationFrame", select_overload<void(int)>(&ReRendererReactor::onEndAnimationFrame), allow_raw_pointers())
		.function("onBeginRenderer", select_overload<void(int)>(&ReRendererReactor::onBeginRenderer), allow_raw_pointers())
		.function("onEndRenderer", select_overload<void(int)>(&ReRendererReactor::onEndRenderer), allow_raw_pointers())
		.function("onClearCanvas", select_overload<void(int)>(&ReRendererReactor::onClearCanvas), allow_raw_pointers())
		.function("onClearWorkCanvas", select_overload<void(int)>(&ReRendererReactor::onClearWorkCanvas), allow_raw_pointers())
		.function("onBeginRendererViewport", select_overload<void(int)>(&ReRendererReactor::onBeginRendererViewport), allow_raw_pointers())
		.function("onEndRendererViewport", select_overload<void(int)>(&ReRendererReactor::onEndRendererViewport), allow_raw_pointers())
		.function("onRendererViewport", select_overload<void(int)>(&ReRendererReactor::onRendererViewport), allow_raw_pointers());
}



#endif
