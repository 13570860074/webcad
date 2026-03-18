#include "DbEntityGroup.h"


DbEntityGroup::DbEntityGroup() {
	this->m_entitys = new AcArray<DbEntity*>();
}
DbEntityGroup::~DbEntityGroup() {
	if (this->m_entitys != NULL) {
		delete this->m_entitys;
		this->m_entitys = NULL;
	}
}


AcArray<DbEntity*>* DbEntityGroup::entitys() {
	return this->m_entitys;
}