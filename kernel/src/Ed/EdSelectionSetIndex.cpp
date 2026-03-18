#include "EdSelectionSetIndex.h"


EdSelectionSetIndex::EdSelectionSetIndex() {
	this->index = 0;
	this->m_isEffective = false;
	this->m_size = 0;
	this->length = 0;
	this->selectionSet = NULL;
}
EdSelectionSetIndex::~EdSelectionSetIndex() {
}
void EdSelectionSetIndex::init(const unsigned int len) {
	this->m_isEffective = true;
	if (len == 0) {
		this->m_size = 512;
		this->length = 0;
		this->selectionSet = new ads_name[this->m_size];
	}
	else {
		this->m_size = len;
		this->length = 0;
		this->selectionSet = new ads_name[this->m_size];
	}
}
bool EdSelectionSetIndex::isEffective() const {
	return this->m_isEffective;
}
void EdSelectionSetIndex::expansion() {
	ads_name* temps = new ads_name[this->m_size];
	for (int i = 0; i < this->m_size; i++) {
		temps[i][0] = this->selectionSet[i][0];
		temps[i][1] = this->selectionSet[i][1];
	}
	delete[] this->selectionSet;
	this->selectionSet = new ads_name[this->m_size * 2];
	for (int i = 0; i < this->m_size; i++) {
		this->selectionSet[i][0] = temps[i][0];
		this->selectionSet[i][1] = temps[i][1];
	}
	this->m_size *= 2;
}
unsigned int EdSelectionSetIndex::capacity() const {
	return this->m_size;
}
void EdSelectionSetIndex::add(const ads_name name) {
	if (this->length >= this->m_size) {
		this->expansion();
	}
	this->selectionSet[this->length][0] = name[0];
	this->selectionSet[this->length][1] = name[1];
	this->length++;
}
void EdSelectionSetIndex::release() {
	this->m_isEffective = false;
	this->m_size = 0;
	this->length = 0;
	delete[] this->selectionSet;
}

