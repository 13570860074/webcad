#ifndef GIVIEWPORTDRAW_H
#define GIVIEWPORTDRAW_H

#include "GiCommonDraw.h"

class GiViewportGeometry;
class GiViewportDraw : public GiCommonDraw
{
public:
	GiViewportDraw();
	virtual ~GiViewportDraw();

public:
	virtual GiViewportGeometry& geometry() const = 0;

	// 获得视口序列号
	virtual Adesk::UInt32 sequenceNumber() const = 0;

	// 判断ID是否有效
	virtual bool isValidId(const Adesk::UInt32 viewportId) const = 0;
};



#endif
