#include "GsElement.h"
#include "EvEventManager.h"
#include "GiMaterialManager.h"
#include "GiEntityManager.h"
#include "Kernel.h"
#include "GsImpl.h"


GsElement::GsElement() {

}
GsElement::~GsElement() {

}

void GsElement::onInit() {

}

ApDocument* GsElement::document() const {
	return GS_IMP_ELEMENT(this->m_pImpl)->document;
}

