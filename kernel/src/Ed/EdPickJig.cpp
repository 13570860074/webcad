#include "arxHeaders.h"
#include "EdPickJig.h"
#include "EdUserInputManager.h"
#include "EdSelectionSetManager.h"
#include "GiEntityGeometry.h"
#include "acedTrans.h"
#include "EdImpl.h"
#include "kernel.h"

EdPickJig::EdPickJig()
{
	this->m_bPick = false;
	this->m_curPoint = GePoint3d::kOrigin;
}
EdPickJig::~EdPickJig()
{
}
DbObjectIdArray EdPickJig::ids() const
{
	return this->m_ids;
}
EdJig::DragStatus EdPickJig::pick(const AcString& prompt)
{
	GePoint2d deviceLocation = ::kernel()->acedUserInputManager()->deviceLocation();
	return this->pick(deviceLocation, prompt);
}
EdJig::DragStatus EdPickJig::pick(const GePoint2d& dcsPoint, const AcString& prompt)
{

	this->setSpecialCursorType(CursorType::kSelectionSet);

	resbuf res;
	res.restype = Db::DwgDataType::kDwg3Real;
	GePoint3d ucsPoint = ::trans_dcs_to_ucs(dcsPoint);
	res.resval.rpoint[0] = ucsPoint.x;
	res.resval.rpoint[1] = ucsPoint.y;
	res.resval.rpoint[2] = ucsPoint.z;
	acedSetVar("LASTPOINT", &res);

	this->displayStartPoint = dcsPoint;

	EdJig::DragStatus status = this->drag();
	if (status == EdJig::DragStatus::kNormal)
	{
		if (::kernel()->acedUserInputManager()->shift() == false)
		{
			this->m_ids = ::kernel()->acedSelectionSetManager()->selectExtents(this->displayStartPoint, this->displayEndPoint, 0, -1);
		}
		else
		{
			this->m_ids = ::kernel()->acedSelectionSetManager()->selectExtents(this->displayStartPoint, this->displayEndPoint, 1, -1);
		}
	}

	return status;
}
EdJig::DragStatus EdPickJig::entSel(const AcString& prompt)
{
	EdJig::DragStatus status = this->drag();
	return status;
}
GePoint3d EdPickJig::curPoint() const
{
	return this->m_curPoint;
}

DbEntity* EdPickJig::entity() const
{
	return NULL;
}
EdJig::DragStatus EdPickJig::sampler()
{
	if (this->m_bPick == true)
	{
		setUserInputControls((UserInputControls)(EdJig::kNoNegativeResponseAccepted |
			EdJig::kNullResponseAccepted |
			EdJig::kGovernedByOrthoMode |
			EdJig::kAcceptOtherInputString |
			EdJig::kAcceptMouseUpAsPoint));
	}
	else
	{
		setUserInputControls((UserInputControls)(EdJig::kNoNegativeResponseAccepted |
			EdJig::kNullResponseAccepted |
			EdJig::kGovernedByOrthoMode |
			EdJig::kAcceptOtherInputString));
	}

	DragStatus stat = acquirePoint(this->m_curPoint);
	this->displayEndPoint = ::trans_wcs_to_dcs(this->m_curPoint);

	return stat;
}
Adesk::Boolean EdPickJig::update()
{
	return true;
}
