#ifndef GIDRAWABLE_H
#define GIDRAWABLE_H

#include "RxObject.h"
#include "DbObjectId.h"

class GiDrawableTraits;
class GiWorldDraw;
class GiViewportDraw;

class GiDrawable :public RxObject
{
public:

	// 属性标志位
	enum SetAttributesFlags
	{
		kDrawableNone						= 0,
		kDrawableIsAnEntity					= 1,
		kDrawableUsesNesting				= 2,
		kDrawableIsCompoundObject			= 4,
		kDrawableViewIndependentViewportDraw	= 8,
		kDrawableIsInvisible				= 16,
		kDrawableHasAttributes				= 32,
		kDrawableRegenTypeDependantGeometry	= 64,
		kDrawableIsDimension				= 128,
		kDrawableRegenDraw					= 256,
		kDrawableViewDependentViewportDraw	= 2048,
		kDrawableBlockDependentViewportDraw	= 4096,
		kDrawableIsExternalReference			= 8192,
		kDrawableNotPlottable				= 16384
	};

	// 可绘制对象类型
	enum DrawableType
	{
		kGeometry			= 0,
		kDistantLight		= 1,
		kPointLight			= 2,
		kSpotLight			= 3,
		kAmbientLight		= 4,
		kSolidBackground	= 5,
		kGradientBackground	= 6,
		kImageBackground	= 7
	};

	GiDrawable();
	virtual ~GiDrawable();
	RX_DECLARE_MEMBERS(GiDrawable);

	// NVI公共接口 (调用者入口)
	unsigned int setAttributes(GiDrawableTraits* pTraits)
	{
		return subSetAttributes(pTraits);
	}
	bool worldDraw(GiWorldDraw* pWd)
	{
		return subWorldDraw(pWd);
	}
	void viewportDraw(GiViewportDraw* pVd)
	{
		subViewportDraw(pVd);
	}

	// 类型查询
	virtual DrawableType drawableType() const { return kGeometry; }

	// 持久性
	virtual bool isPersistent() const { return false; }
	virtual DbObjectId id() const { return DbObjectId(); }

protected:
	// 子类实现点
	virtual unsigned int subSetAttributes(GiDrawableTraits* pTraits) const { return kDrawableNone; }
	virtual bool subWorldDraw(GiWorldDraw* pWd) const { return false; }
	virtual void subViewportDraw(GiViewportDraw* pVd) const {}
};



#endif