#ifndef EDDIMJIG_H
#define EDDIMJIG_H

#include "EdJig.h"


class DbLine;
class DbPolyline;
class EdDimJig : public EdJig
{
public:
	EdDimJig();
	virtual ~EdDimJig();

	enum DimType
	{
		kNone,
		kDist,
		kAngle,
		kDistAddAngle,
		kPoint2d,
		kPoint3d,
		kCustomString,
		kCustomDist,
		kCustomAngle,
	};

private:
	bool m_bCorner;								// 是否是拐角输入

	bool m_bAutoEnableDragline;					// 是否自动启用拖拽线
	bool m_bEnableDragline;						// 是否启用拖拽线

protected:
	EdDimJig::DimType m_dimType;				// 类型用于决定使用那种方式
	GePoint3d m_curPoint;						// 当前点
	GePoint3d m_lastPoint;						// 上一个点
	AcString customString;						// 自定义字符串(在使用kCustomString类型时生效)

	bool m_bDimReferPoint;
	GePoint3d m_dimReferPoint;

	/** 坐标系信息 */
	GePoint3d m_ucsOrigin;
	GeVector3d m_ucsXAxis;
	GeVector3d m_ucsYAxis;
	GeVector3d m_ucsZAxis;
	GeVector3d m_referXAxis;


public:

	/** 尺寸类型 */
	EdDimJig::DimType dimType() const;

	/** 是否启用拖拽线 */
	bool isEnableDragline() const;

	/** 设置是否启用拖拽线 */
	void setEnableDragline(const bool v);

	/** 设置尺寸类型 */
	void setDimType(const EdDimJig::DimType dimType);

	/** 设置参照点 */
	void setDimReferPoint(const GePoint3d &ps);

public:
	/** 获得拐角输入 */
	EdJig::DragStatus acquireCorner(GePoint3d&);
	EdJig::DragStatus acquireCorner(GePoint3d&, const GePoint3d& basePoint);

public:

	/** 拖拽 */
	EdJig::DragStatus drag();

	/** 获得输入 */
	EdJig::DragStatus dragAcquireString(AcString&, const bool bSpace = false);

	/** 拾取坐标 */
	EdJig::DragStatus dragAcquirePoint(GePoint3d&, const EdDimJig::DimType dimType);
	EdJig::DragStatus dragAcquirePoint(GePoint3d&, const GePoint3d& basePnt, const EdDimJig::DimType dimType);

	/** 拾取角度 */
	EdJig::DragStatus dragAcquireAngle(double&);
	EdJig::DragStatus dragAcquireAngle(double&, const EdDimJig::DimType dimType);
	EdJig::DragStatus dragAcquireAngle(double&, const GePoint3d& basePnt, const EdDimJig::DimType dimType);
	EdJig::DragStatus dragAcquireAngle(double&, const GePoint3d& basePnt, const GeVector3d& referenceAxis, const EdDimJig::DimType dimType);

private:
	DbLine *tempLine;
	DbPolyline* entCorner;
	DbDimDataPtrArray *m_pDimDatas;

public:

	/** 设置是否允许输入空格 */
	void setSpace(const bool v);

	/** 设置是否允许输入确认 */
	void setConfirm(const bool v);

	/** 设置是否是快捷键 */
	void setKeyword(const bool v);

	/** 判断输出的是否是整数 */
	bool acquireValue(int& val);

	/** 判断输出的是否是实数 */
	bool acquireValue(double& val);

	/** 判断输出的是否是字符串 */
	bool acquireValue(AcString& val);

	/** 判断输出的是否是快捷键 */
	bool acquireKword(AcString& key);

	/** 判断输出的是否是坐标 */
	bool acquireValue(GePoint3d& val);

	/** 设置当前自定义字符串的值 */
	void setCustomValue(const AcString& str);

public:
	virtual DbEntity *entity() const;
	virtual DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual DbDimDataPtrArray *dimData(const double dimScale);
	virtual Acad::ErrorStatus setDimValue(const DbDimData *dimData, const double dimValue);
};

#endif