#include "AcEventBase.h"


void AcEventItemGroup::push_back(AcEventItem* item) {
	if (item->priority == 0) {
		std::vector<AcEventItem*>::push_back(item);
	}
	else {
		bool isInsert = false;
		std::vector<AcEventItem*> items;
		for (int i = 0; i < this->size(); i++) {
			if (isInsert == false && item->priority >= this->at(i)->priority) {
				items.push_back(item);
				isInsert = true;
			}
			items.push_back(this->at(i));
		}

		std::vector<AcEventItem*>::clear();
		for (int i = 0; i < items.size(); i++) {
			std::vector<AcEventItem*>::push_back(items.at(i));
		}
		if (isInsert == false) {
			std::vector<AcEventItem*>::push_back(item);
		}
	}
}

unsigned int AcEventBase::numEvents() {
	int num = 0;
	for (int i = 0; i < this->pItems->size(); i++) {
		for (int u = 0; u < this->pItems->at(i)->size(); u++) {
			num++;
		}
	}
	return num;
}
unsigned int AcEventBase::numEvents(int _msgId) {
	int num = 0;
	for (int i = 0; i < this->pItems->size(); i++) {
		if (this->pItems->at(i)->MsgId == _msgId) {
			for (int u = 0; u < this->pItems->at(i)->size(); u++) {
				num++;
			}
			break;
		}
	}
	return num;
}