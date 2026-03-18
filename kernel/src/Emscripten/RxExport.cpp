#include "Gi.h"
#include "export.h"
#include "EmObject.h"
#include "RxExport.h"
#include "kernel.h"
#include "RxObject.h"
#include "GiObjectId.h"
#include "GiMaterial.h"
#include "RxManager.h"


#if EMSDK

EMSCRIPTEN_BINDINGS(Rx)
{
	// RxObject
	emscripten::class_<RxObject, base<EmObject>>("RxObject")
		.class_function("cast", &RxObject::cast, allow_raw_pointers());

	// RxManager
	emscripten::class_<RxManager>("RxManager");
}

#endif
