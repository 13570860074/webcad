#include "GiEntityIterator.h"
#include "RxObject.h"
#include "AcArray.h"
#include "GiImpl.h"


GiEntityIterator::GiEntityIterator()
{
	this->m_pImpl = new GiEntityIteratorImpl();
}
GiEntityIterator::~GiEntityIterator() {

}


void GiEntityIterator::start() {
	GI_IMP_ENTITYITERATOR(GI_IMP_ENTITYITERATOR(this->m_pImpl))->iterator.start();
}

bool GiEntityIterator::done() {
	return GI_IMP_ENTITYITERATOR(this->m_pImpl)->iterator.done();
}
GiEntity* GiEntityIterator::getEntity() {
	return GiEntity::cast(GI_IMP_ENTITYITERATOR(this->m_pImpl)->iterator.record());
}
void GiEntityIterator::step() {
	GI_IMP_ENTITYITERATOR(this->m_pImpl)->iterator.next();
}


