#ifndef DBHANDLETABLERECORD_H
#define DBHANDLETABLERECORD_H

#include "GePoint3d.h"
#include "RxObject.h"
#include "AcArray.h"
#include "GiEntityGeometry.h"


class DbHandleTableRecord {
public:
	DbHandleTableRecord();
	virtual ~DbHandleTableRecord();

	unsigned int objectId;
	unsigned int handle;
};


#endif
