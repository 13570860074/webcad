#include "EdCommand.h"
#include "EdImpl.h"


void EdCommand::addReactor(EdCommandReactor* pReactor) {
	RX_IMP_COMMAND(this->m_pImpl)->reactor.append(pReactor);
}
void EdCommand::removeReactor(EdCommandReactor* pReactor) {
	for (int i = RX_IMP_COMMAND(this->m_pImpl)->reactor.length() - 1; i >= 0; i--) {
		if (RX_IMP_COMMAND(this->m_pImpl)->reactor[i] == pReactor) {
			realloc_arry_at(RX_IMP_COMMAND(this->m_pImpl)->reactor, i);
			break;
		}
	}
}
Adesk::UInt8 EdCommand::defaultCommandVersion() const {
	return 0;
}
