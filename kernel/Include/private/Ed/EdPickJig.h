#ifndef EDPICKJIG_H
#define EDPICKJIG_H


#include "EdDimJig.h"
#include "DbObjectId.h"


class EdPickJig : public EdDimJig {
public:
	EdPickJig();
	virtual ~EdPickJig();


private:
	bool m_bPick;
	GePoint3d m_curPoint;
	DbObjectIdArray m_ids;
	GePoint2d displayStartPoint;
	GePoint2d displayEndPoint;

public:

	/* 对象列表 */
	DbObjectIdArray ids() const;

	/* 拾取一次 */
	EdJig::DragStatus pick(const AcString& prompt);
	EdJig::DragStatus pick(const GePoint2d& dcsPoint, const AcString& prompt);

	/* 拾取一个对象 */
	EdJig::DragStatus entSel(const AcString& prompt);

	/** 当前坐标 */
	GePoint3d curPoint() const;

	virtual DbEntity* entity() const;
	DragStatus sampler();
	Adesk::Boolean update();
};



#endif