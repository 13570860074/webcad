#include "ApDocumentIterator.h"
#include "ApImpl.h"


ApDocumentIterator::ApDocumentIterator() {
	this->m_pImpl = new ApDocumentIteratorImpl();
}
ApDocumentIterator::~ApDocumentIterator() {

}


void ApDocumentIterator::start() {
	AP_IMP_DOCUMENTITERATORIMPL(this->m_pImpl)->iterator.start();
}
bool ApDocumentIterator::done() {
	return AP_IMP_DOCUMENTITERATORIMPL(this->m_pImpl)->iterator.done();
}
void ApDocumentIterator::step() {
	AP_IMP_DOCUMENTITERATORIMPL(this->m_pImpl)->iterator.next();
}
ApDocument* ApDocumentIterator::document() {
	return AP_IMP_DOCUMENTITERATORIMPL(this->m_pImpl)->iterator.record();
}


