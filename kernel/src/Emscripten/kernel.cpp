#include "kernel.h"
#include "EmObject.h"
#include "DbHostApplicationServices.h"
#include "GiObject.h"
#include "GiObjectManager.h"
#include "GiMaterialManager.h"
#include "GiEntityManager.h"
#include "ReRendererManager.h"
#include "EdUserInputManager.h"
#include "RxClassManager.h"
#include "RxExport.h"


#if EMSDK

void emsdk_free(EmObject* p) {
	if (p != NULL) {
		delete p;
		p = NULL;
	}
}


EMSCRIPTEN_BINDINGS(Kerne) {

	function("emsdk_free", select_overload<void(EmObject*)>(&emsdk_free), allow_raw_pointers());

	function("acdbHostApplicationServices", &acdbHostApplicationServices, allow_raw_pointers());
	function("acreRendererManager", &acreRendererManager, allow_raw_pointers());
	function("acgiMaterialManager", &acgiMaterialManager, allow_raw_pointers());
	function("acgiEntityManager", &acgiEntityManager, allow_raw_pointers());
	function("acedUserInputManager", &acedUserInputManager, allow_raw_pointers());
}



#endif
