#ifndef GIWORLDDRAW_H
#define GIWORLDDRAW_H


#include "GiCommonDraw.h"

class GiWorldGeometry;
class GiWorldDraw :public GiCommonDraw
{
public:
	GiWorldDraw();
	virtual ~GiWorldDraw();

public:
	virtual GiWorldGeometry& geometry() const = 0;
};



#endif