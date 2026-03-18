#include "AcEmsdkEvent.h"


int AcEmsdkEvent::on(int _MsgId, long long _function, unsigned int priority)
{
	AcEventItemGroup* pItemGroup = NULL;
	for (int i = 0; i < this->pItems->size(); i++) {
		if (this->pItems->at(i)->MsgId == _MsgId)
		{
			pItemGroup = this->pItems->at(i);
			break;
		}
	}
	if (pItemGroup == NULL) {
		pItemGroup = new AcEventItemGroup();
		pItemGroup->MsgId = _MsgId;
		this->pItems->push_back(pItemGroup);
	}

	AcEventItem* pEventItem = new AcEventItem();
	pEventItem->Id = this->Index++;
	pEventItem->function = (void*)_function;
	pEventItem->priority = priority;
	if (priority == 0) {
		pItemGroup->push_back(pEventItem);
	}
	else {
		bool isPush = false;
		std::vector<AcEventItem*> list;
		for (int i = 0; i < pItemGroup->size(); i++) {
			if (priority > pItemGroup->at(i)->priority && isPush == false) {
				list.push_back(pEventItem);
				isPush = true;
			}
			list.push_back(pItemGroup->at(i));
		}
		if (isPush == false) {
			list.push_back(pEventItem);
		}

		pItemGroup->clear();
		for (int i = 0; i < list.size(); i++) {
			pItemGroup->push_back(list[i]);
		}
	}
	return pEventItem->Id;
}
int AcEmsdkEvent::addEventListener(int _MsgId, long long _function, unsigned int priority)
{
	return this->on(_MsgId, _function, priority);
}

void AcEmsdkEvent::removeListener(int _MsgId, long long _function) {
	for (int i = 0; i < this->pItems->size(); i++) {
		if (this->pItems->at(i)->MsgId != _MsgId) {
			continue;
		}
		for (int u = 0; u < this->pItems->at(i)->size(); u++)
		{
			if (this->pItems->at(i)->at(u)->function == (void*)_function) {
				AcEventItem* pEventItem = this->pItems->at(i)->at(u);
				this->pItems->at(i)->erase(this->pItems->at(i)->begin() + u);
				delete pEventItem;
				break;
			}
		}
		break;
	}
}
void AcEmsdkEvent::removeEventListener(int _MsgId, long long _function) {
	this->removeListener(_MsgId, _function);
}

void AcEmsdkEvent::removeListenerId(int _Id) {
	for (int i = 0; i < this->pItems->size(); i++) {
		bool isFind = false;
		for (int u = 0; u < this->pItems->at(i)->size(); u++)
		{
			if (this->pItems->at(i)->at(u)->Id == _Id) {
				isFind = true;
				AcEventItem* pEventItem = this->pItems->at(i)->at(u);
				this->pItems->at(i)->erase(this->pItems->at(i)->begin() + u);
				delete pEventItem;
				break;
			}
		}
		if (isFind == true) {
			break;
		}
	}
}
void AcEmsdkEvent::removeEventListenerId(int _Id) {
	this->removeListenerId(_Id);
}

void AcEmsdkEvent::removeAllListeners() {
	for (int i = this->pItems->size() - 1; i >= 0; i--) {
		AcEventItemGroup* pEventItemGroup = this->pItems->at(i);
		delete pEventItemGroup;
	}
	this->pItems->clear();
}
void AcEmsdkEvent::removeAllListeners(int _MsgId) {
	for (int i = this->pItems->size() - 1; i >= 0; i--) {
		if (this->pItems->at(i)->MsgId != _MsgId)
		{
			continue;
		}
		AcEventItemGroup* pEventItemGroup = this->pItems->at(i);
		this->pItems->erase(this->pItems->begin() + i);
		delete pEventItemGroup;
		break;
	}
}