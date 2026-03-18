#ifndef EDCONTROLLER_H
#define EDCONTROLLER_H


#include "RxObject.h"


class EdController : public RxObject {
public:
	RX_DECLARE_MEMBERS_VIRTUAL(EdController);

public:
	virtual void onInit();
};


#endif