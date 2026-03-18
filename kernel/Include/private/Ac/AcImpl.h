#ifndef ACIMPL_H
#define ACIMPL_H


#include "AcString.h"
#include "AcJsonObject.h"


class AcJsonObjectImpl {
public:
	AcJsonObjectImpl() {
		this->type = AcJsonObject::Type::Object;
		this->integer = 0;
		this->real = 0.0;
	}
	~AcJsonObjectImpl() {

	}

	AcString key;
	AcJsonObject::Type type;
	AcString string;
#if EMSDK
	long integer;
#else
	long long integer;
#endif
	double real;
};



#endif





