#include "RxDeclareMembers.h"
#include "RxClassManager.h"

RxClass* acrxFindAcRxClass(const char* name) {
	return RxClassManager::acrxFindAcRxClass(name);
}