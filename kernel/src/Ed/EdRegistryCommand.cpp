#include "EdRegistryCommand.h"
#include "EdImpl.h"



EdRegistryCommand::EdRegistryCommand() {
	this->m_pImpl = new EdRegistryCommandImpl();
}
EdRegistryCommand::~EdRegistryCommand() {

}
void EdRegistryCommand::commandUndef(bool undefIt) {

}
void EdRegistryCommand::commandFlags(Adesk::Int32 flags) const {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->commandFlags = flags;
}

RxFunctionPtr EdRegistryCommand::functionAddr() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->function;
}
void EdRegistryCommand::functionAddr(RxFunctionPtr fhdl) {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->function = fhdl;
}
Adesk::Int32 EdRegistryCommand::commandFlags() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->commandFlags;
}
void EdRegistryCommand::commandApp(void* app)const {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->app = app;
}
void* EdRegistryCommand::commandApp() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->app;
}
int EdRegistryCommand::functionCode() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->functionCode;
}
void EdRegistryCommand::functionCode(int fcode) {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->functionCode = fcode;
}

void EdRegistryCommand::groupName(const ACHAR* name) const {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->groupName = name;
}
const ACHAR* EdRegistryCommand::groupName() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->groupName.constPtr();
}
void EdRegistryCommand::globalName(const ACHAR* name) {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->globalName = name;
}
const ACHAR* EdRegistryCommand::globalName() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->globalName.constPtr();
}
void EdRegistryCommand::localName(const ACHAR* name) {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->localName = name;
}
const ACHAR* EdRegistryCommand::localName() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->localName.constPtr();
}



#if EMSDK
void EdRegistryCommand::emsdk_groupName(const emstring& name) const {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->groupName = name;
}
const emstring EdRegistryCommand::emsdk_groupName() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->groupName.constPtr();
}
void EdRegistryCommand::emsdk_globalName(const emstring& name) const {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->globalName = name;
}
const emstring EdRegistryCommand::emsdk_globalName() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->globalName.constPtr();
}
void EdRegistryCommand::emsdk_localName(const emstring& name) const {
	RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->localName = name;
}
const emstring EdRegistryCommand::emsdk_localName() const {
	return RX_IMP_REGISTRYCOMMAND(this->m_pImpl)->localName.constPtr();
}
#endif

