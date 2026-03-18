#ifndef EDITORREACTORMANAGER_H
#define EDITORREACTORMANAGER_H


#include "aced.h"
#include "AcArray.h"

class RxEventReactor;
class EditorReactorManager :public Editor {
public:
	EditorReactorManager();
	virtual ~EditorReactorManager();
	RX_DECLARE_MEMBERS(EditorReactorManager);

public:
	void addReactor(RxEventReactor* newObj);
	void removeReactor(RxEventReactor* delObj);
};


#endif