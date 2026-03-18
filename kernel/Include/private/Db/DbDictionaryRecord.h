#ifndef DBDICTIONARYRECORD_H
#define DBDICTIONARYRECORD_H

#include "RxObject.h"
#include "DbObjectId.h"


class DbDictionaryRecord :public RxObject{
public:
	DbDictionaryRecord() {}
	virtual ~DbDictionaryRecord() {}
	RX_DECLARE_MEMBERS(DbDictionaryRecord);

	DbObjectId objectId;
	AcString name;
};




#endif