#include "RxSysRegistryDictionaryIterator.h"
#include "RxImpl.h"

RxSysRegistryDictionaryIterator::RxSysRegistryDictionaryIterator() {
	this->m_pImpl = new RxSysRegistryDictionaryIteratorImpl();
}
RxSysRegistryDictionaryIterator::~RxSysRegistryDictionaryIterator() {

}

bool RxSysRegistryDictionaryIterator::done() const {
	return RX_IMP_SYSREGISTRYDICTIONARYITERATOR(this->m_pImpl)->done();
}
bool RxSysRegistryDictionaryIterator::next() {
	return RX_IMP_SYSREGISTRYDICTIONARYITERATOR(this->m_pImpl)->next();
}
#if EMSDK
const std::string RxSysRegistryDictionaryIterator::emsdk_key() const {
	return RX_IMP_SYSREGISTRYDICTIONARYITERATOR(this->m_pImpl)->key();
}
#else
const ACHAR* RxSysRegistryDictionaryIterator::key() const {
	return RX_IMP_SYSREGISTRYDICTIONARYITERATOR(this->m_pImpl)->key();
}
#endif
RxObject* RxSysRegistryDictionaryIterator::object() const {
	return RX_IMP_SYSREGISTRYDICTIONARYITERATOR(this->m_pImpl)->object();
 }
Adesk::UInt32 RxSysRegistryDictionaryIterator::id() const {
	return RX_IMP_SYSREGISTRYDICTIONARYITERATOR(this->m_pImpl)->id();
}


