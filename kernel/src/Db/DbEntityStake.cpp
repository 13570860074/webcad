#include "DbEntityStake.h"


DbEntityStake::DbEntityStake() {
	this->m_entity = NULL;
	this->m_previous = NULL;
	this->m_matrix = NULL;
}
DbEntityStake::~DbEntityStake() {
	if (this->m_matrix != NULL) {
		delete this->m_matrix;
		this->m_matrix = NULL;
	}
}
void DbEntityStake::free(DbEntityStake* stake) {
	if (stake == NULL) {
		return;
	}
	for (int i = stake->stakes()->length() - 1; i >= 0; i--) {
		DbEntityStake::free(stake->stakes()->at(i));
	}
	delete stake;
}
AcArray<DbEntityStake*>* DbEntityStake::stakes() {
	return &this->m_stake;
}
DbEntity* DbEntityStake::entity()const {
	return this->m_entity;
}
void DbEntityStake::setEntity(DbEntity* ent) {
	this->m_entity = ent;
}

DbEntityStake* DbEntityStake::previous() const {
	return this->m_previous;
}
void DbEntityStake::setPrevious(DbEntityStake* p) {
	this->m_previous = p;
}

GeMatrix3d DbEntityStake::matrix()const {
	if (this->m_matrix == NULL) {
		return GeMatrix3d::kIdentity;
	}
	return *this->m_matrix;
}
void DbEntityStake::setMatrix(const GeMatrix3d& mat) {
	if (this->m_matrix == NULL) {
		this->m_matrix = new GeMatrix3d();
	}
	this->m_matrix->set(mat);
}

DbEntityStake* DbEntityStake::root() {
	DbEntityStake* stake = this->previous();
	while (true) {
		if (stake == NULL) {
			break;
		}
		stake = stake->previous();
	}
	if (stake == NULL) {
		return this;
	}
	return stake;
}
DbEntity* DbEntityStake::rootEntity() {
	DbEntityStake* p = this->root();
	return p->entity();
}
GeMatrix3d DbEntityStake::rootMatrix() {
	GeMatrix3d mat;
	while (true) {
		if (this->previous() == NULL) {
			break;
		}
		if (this->previous()->m_matrix != NULL) {
			mat *= this->previous()->matrix();
		}
	}
	return mat;
}
