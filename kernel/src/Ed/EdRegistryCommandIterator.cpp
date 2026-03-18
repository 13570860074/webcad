#include "EdRegistryCommandIterator.h"
#include "EdImpl.h"

EdRegistryCommandIterator::EdRegistryCommandIterator() {
	this->m_pImpl = new EdRegistryCommandIteratorImpl();
}
EdRegistryCommandIterator::~EdRegistryCommandIterator() {

}
bool EdRegistryCommandIterator::done() const {
	return RX_IMP_REGISTRYCOMMANDITERATOR(this->m_pImpl)->done();
}
bool EdRegistryCommandIterator::next() {
	return RX_IMP_REGISTRYCOMMANDITERATOR(this->m_pImpl)->next();
}
const EdCommand* EdRegistryCommandIterator::command() const {
	return (EdCommand*)RX_IMP_REGISTRYCOMMANDITERATOR(this->m_pImpl)->command();
}
const ACHAR* EdRegistryCommandIterator::commandGroup() const {
	return RX_IMP_REGISTRYCOMMANDITERATOR(this->m_pImpl)->key();
}
