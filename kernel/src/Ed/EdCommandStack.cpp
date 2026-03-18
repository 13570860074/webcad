#include "EdCommandStack.h"
#include "EdCommandIterator.h"
#include "EdImpl.h"




EdCommandStack::~EdCommandStack() {

}

void EdCommandStack::addReactor(EdCommandStackReactor* pReactor) {
	RX_IMP_COMMANDSTACK(this->m_pImpl)->reactors.append(pReactor);
}
void EdCommandStack::removeReactor(EdCommandStackReactor* pReactor) {
	for (int i = RX_IMP_COMMANDSTACK(this->m_pImpl)->reactors.length() - 1; i >= 0; i--) {
		if (RX_IMP_COMMANDSTACK(this->m_pImpl)->reactors[i] == pReactor) {
			realloc_arry_at(RX_IMP_COMMANDSTACK(this->m_pImpl)->reactors, i);
			break;
		}
	}
}