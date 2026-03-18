#include "EditorReactorManager.h"
#include "EditorReactor.h"
#include "EdImpl.h"


EditorReactorManager::EditorReactorManager() {
	this->m_pImpl = new EditorReactorManagerImpl();
}
EditorReactorManager::~EditorReactorManager() {
	
}


void EditorReactorManager::addReactor(RxEventReactor* newObj) {
	ED_IMP_EDITTORREACTORMANAGER(this->m_pImpl)->reactors.append(newObj);
}
void EditorReactorManager::removeReactor(RxEventReactor* delObj) {
	for (int i = 0; i < ED_IMP_EDITTORREACTORMANAGER(this->m_pImpl)->reactors.length(); i++) {
		if (ED_IMP_EDITTORREACTORMANAGER(this->m_pImpl)->reactors.at(i) == delObj) {
			realloc_arry_at(ED_IMP_EDITTORREACTORMANAGER(this->m_pImpl)->reactors, i);
			break;
		}
	}
}
