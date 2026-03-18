#ifndef GIENTITYHIGHLANDER_H
#define GIENTITYHIGHLANDER_H

#include "RxObject.h"

class DbEntity;
class GiEntityHighlander : public RxObject
{
public:
	GiEntityHighlander();
	virtual ~GiEntityHighlander();
	RX_DECLARE_MEMBERS(GiEntityHighlander);

	virtual void highlight(DbEntity *entity);
	virtual void unhighlight(DbEntity *entity);
};

#endif