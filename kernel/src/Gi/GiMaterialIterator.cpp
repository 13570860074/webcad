#include "GiMaterialIterator.h"
#include "RxObject.h"
#include "AcArray.h"
#include "GiImpl.h"


GiMaterialIterator::GiMaterialIterator()
{
	this->m_pImpl = new GiMaterialIteratorImpl();
}
GiMaterialIterator::~GiMaterialIterator() {

}


void GiMaterialIterator::start() {
	GI_IMP_MATERIALITERATOR(this->m_pImpl)->iterator.start();
}

bool GiMaterialIterator::done() {
	return GI_IMP_MATERIALITERATOR(this->m_pImpl)->iterator.done();
}
GiMaterial* GiMaterialIterator::getMaterial() {
	return GiMaterial::cast(GI_IMP_MATERIALITERATOR(this->m_pImpl)->iterator.record());
}
void GiMaterialIterator::step() {
	GI_IMP_MATERIALITERATOR(this->m_pImpl)->iterator.next();
}


