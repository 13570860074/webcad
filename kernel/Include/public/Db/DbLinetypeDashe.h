#ifndef DBLINETYPEDASHE_H
#define DBLINETYPEDASHE_H


#include "RxObject.h"
#include "AcString.h"
#include "GeVector2d.h"


class DbLinetypeDashe :public RxObject{
public:
	DbLinetypeDashe();
	virtual ~DbLinetypeDashe();
	RX_DECLARE_MEMBERS(DbLinetypeDashe);

	double dashLength();
	AcString text();
	GeVector2d shapeOffset() const;
	double shapeScale();
	double shapeRotation();

	void setText(const AcString& _text);
	void setDashLength(double _v);
	void setShapeOffset(const GeVector2d& point);
	void setShapeScale(double _v);
	void setShapeRotation(double _v);
};



#endif
