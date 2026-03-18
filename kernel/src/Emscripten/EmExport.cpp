#include "EmObject.h"

#if EMSDK



EMSCRIPTEN_BINDINGS(Em) {

	emscripten::class_<EmObject>("EmObject")
		.constructor<>();
}



#endif
