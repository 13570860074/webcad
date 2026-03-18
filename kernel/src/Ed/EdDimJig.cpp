#include "EdDimJig.h"
#include "DbViewportTableRecord.h"
#include "DbViewport.h"
#include "DbLine.h"
#include "DbPolyline.h"
#include "DbAlignedDimension.h"
#include "Db2LineAngularDimension.h"
#include "GePlane.h"
#include "EvEventManager.h"
#include "acedTrans.h"
#include "acedads.h"
#include "aced.h"
#include "kernel.h"
#include "EdImpl.h"

EdDimJig::EdDimJig()
{
	this->m_bCorner = false;
	this->m_bAutoEnableDragline = true;
	this->m_bEnableDragline = false;
	this->entCorner = NULL;

	this->m_dimType = EdDimJig::DimType::kNone;

	this->tempLine = NULL;
	this->m_bDimReferPoint = false;
	this->m_pDimDatas = new DbDimDataPtrArray();

	this->m_ucsOrigin = ED_IMP_JIG(this->m_pImpl)->m_ucsOrigin;
	this->m_ucsXAxis = ED_IMP_JIG(this->m_pImpl)->m_ucsXAxis;
	this->m_ucsYAxis = ED_IMP_JIG(this->m_pImpl)->m_ucsYAxis;
	this->m_ucsZAxis = ED_IMP_JIG(this->m_pImpl)->m_ucsZAxis;
	this->m_referXAxis = ED_IMP_JIG(this->m_pImpl)->m_referXAxis;
}
EdDimJig::~EdDimJig()
{
	if (this->m_pDimDatas != NULL)
	{
		for (int i = this->m_pDimDatas->length() - 1; i >= 0; i--)
		{
			delete this->m_pDimDatas->at(i);
		}
		delete this->m_pDimDatas;
		this->m_pDimDatas = NULL;
	}
	if (this->tempLine != NULL)
	{
		delete this->tempLine;
	}
}
EdDimJig::DimType EdDimJig::dimType() const
{
	return this->m_dimType;
}
bool EdDimJig::isEnableDragline() const
{
	return this->m_bEnableDragline;
}
void EdDimJig::setEnableDragline(const bool v)
{
	this->m_bAutoEnableDragline = false;
	this->m_bEnableDragline = v;
}
void EdDimJig::setDimType(const EdDimJig::DimType dimType)
{
	this->m_dimType = dimType;
}
void EdDimJig::setDimReferPoint(const GePoint3d &ps)
{
	this->m_bDimReferPoint = true;
	this->m_dimReferPoint = ps;
}
EdJig::DragStatus EdDimJig::drag()
{
	if (this->dimType() == EdDimJig::kCustomString ||
		this->dimType() == EdDimJig::kCustomDist ||
		this->dimType() == EdDimJig::kCustomAngle)
	{
		bool isExistence = false;
		bool isCustomString = false;
		AcString customString;
		AcString dispPrompt = this->dispPrompt();
		for (int i = 0; i < dispPrompt.length(); i++)
		{
			if (isCustomString == false && dispPrompt[i] == '<')
			{
				isCustomString = true;
				continue;
			}
			if (isCustomString == true && dispPrompt[i] == '>')
			{
				isExistence = true;
				isCustomString = false;
				continue;
			}
			if (isCustomString == true)
			{
				customString += dispPrompt[i];
			}
		}
		if (customString.length() > 0 && isExistence == true)
		{
			this->setCustomValue(customString);
		}
	}
	if (this->m_bAutoEnableDragline == true)
	{
		if (this->m_bDimReferPoint == true)
		{
			this->m_bEnableDragline = true;
		}
	}
	EdJig::DragStatus sta = EdJig::drag();
	this->customString = "";
	this->m_bEnableDragline = false;
	this->m_bAutoEnableDragline = true;
	this->m_bCorner = false;
	return sta;
}
EdJig::DragStatus EdDimJig::dragAcquireString(AcString &str, const bool bSpace)
{
	ED_IMP_JIG(this->m_pImpl)->bConfirm = false;
	ED_IMP_JIG(this->m_pImpl)->bSpace = bSpace;
	this->setDimType(EdDimJig::kCustomString);
	EdJig::DragStatus sta = this->drag();
	this->acquireValue(str);
	return sta;
}
EdJig::DragStatus EdDimJig::dragAcquirePoint(GePoint3d &pt, const EdDimJig::DimType dimType)
{
	this->setDimType(dimType);
	this->setDimReferPoint(this->m_ucsOrigin);
	EdJig::DragStatus sta = this->drag();
	pt = this->m_curPoint;
	return sta;
}

EdJig::DragStatus EdDimJig::acquireCorner(GePoint3d &pt)
{
	this->m_bCorner = true;
	this->entCorner = new DbPolyline();
	this->entCorner->addVertexAt(0, GePoint2d::kOrigin);
	this->entCorner->addVertexAt(1, GePoint2d::kOrigin);
	this->entCorner->addVertexAt(2, GePoint2d::kOrigin);
	this->entCorner->addVertexAt(3, GePoint2d::kOrigin);
	this->entCorner->setClosed(true);
	this->setEnableDragline(false);
	EdJig::DragStatus sta = this->drag();
	this->m_bCorner = false;
	delete this->entCorner;
	this->entCorner = NULL;
	this->acquireValue(pt);
	return sta;
}
EdJig::DragStatus EdDimJig::acquireCorner(GePoint3d &pt, const GePoint3d &basePnt)
{
	this->m_bCorner = true;
	this->entCorner = new DbPolyline();
	this->entCorner->addVertexAt(0, GePoint2d::kOrigin);
	this->entCorner->addVertexAt(1, GePoint2d::kOrigin);
	this->entCorner->addVertexAt(2, GePoint2d::kOrigin);
	this->entCorner->addVertexAt(3, GePoint2d::kOrigin);
	this->entCorner->setClosed(true);
	this->setEnableDragline(false);
	this->setDimReferPoint(basePnt);
	EdJig::DragStatus sta = this->drag();
	this->m_bCorner = false;
	delete this->entCorner;
	this->entCorner = NULL;
	this->acquireValue(pt);
	return sta;
}

EdJig::DragStatus EdDimJig::dragAcquirePoint(GePoint3d &pt, const GePoint3d &basePnt, const EdDimJig::DimType dimType)
{
	this->setDimType(dimType);
	this->setDimReferPoint(basePnt);
	EdJig::DragStatus sta = this->drag();
	pt = this->m_curPoint;
	return sta;
}
EdJig::DragStatus EdDimJig::dragAcquireAngle(double &angle)
{
	this->setDimType(EdDimJig::DimType::kCustomString);
	EdJig::DragStatus sta = this->drag();
	double real;
	if (this->acquireValue(real) == true)
	{
		angle = real;
		angle = angle / 180.0 * PI;
	}
	if (sta == EdJig::DragStatus::kNormal)
	{
	}
	return sta;
}
EdJig::DragStatus EdDimJig::dragAcquireAngle(double &angle, const EdDimJig::DimType dimType)
{
	this->setDimType(dimType);
	EdJig::DragStatus sta = this->drag();
	double real;
	if (this->acquireValue(real) == true)
	{
		angle = real;
		angle = angle / 180.0 * PI;
	}
	if (sta == EdJig::DragStatus::kNormal)
	{
	}
	return sta;
}
EdJig::DragStatus EdDimJig::dragAcquireAngle(double &v, const GePoint3d &basePnt, const EdDimJig::DimType dimType)
{
	return this->dragAcquireAngle(v, basePnt, this->m_ucsXAxis, dimType);
}
EdJig::DragStatus EdDimJig::dragAcquireAngle(double &v, const GePoint3d &basePnt, const GeVector3d &referenceAxis, const EdDimJig::DimType dimType)
{
	return EdJig::DragStatus::kCancel;
}

bool EdDimJig::acquireValue(double &val)
{
	AcString inputStr;
	acquireString(inputStr);
	return AcString::toFloat64(inputStr, val);
}
bool EdDimJig::acquireValue(int &val)
{
	AcString inputStr;
	acquireString(inputStr);
	return AcString::toInt32(inputStr, val);
}
bool EdDimJig::acquireValue(AcString &val)
{
	AcString inputStr;
	acquireString(inputStr);
	if (inputStr.length() == 0)
	{
		return false;
	}
	val = inputStr;
	return true;
}
bool EdDimJig::acquireKword(AcString &kword)
{
	AcString inputStr;
	acquireString(inputStr);
	if (inputStr.length() == 0)
	{
		return false;
	}

	// 获得所有关键字
	AcString key;
	AcArray<AcString> keys;
	AcString keyInput = this->keywordList();
	for (int i = 0; i < keyInput.length(); i++)
	{
		if (keyInput[i] == ' ')
		{
			keys.append(key);
			key.clear();
			continue;
		}
		else if (i == keyInput.length() - 1)
		{
			key += keyInput[i];
			keys.append(key);
			key.clear();
			continue;
		}

		key += keyInput[i];
	}

	bool isKey = false;
	for (int i = 0; i < keys.length(); i++)
	{
		if (inputStr.isEqual(keys[i], true) == true)
		{
			isKey = true;
			kword = keys[i];
			break;
		}
	}
	return isKey;
}
bool EdDimJig::acquireValue(GePoint3d &val)
{
	val = this->m_curPoint;
	return true;
}

void EdDimJig::setCustomValue(const AcString &str)
{
	this->customString = str;
}
void EdDimJig::setSpace(const bool v)
{
	ED_IMP_JIG(this->m_pImpl)->bSpace = v;
}
void EdDimJig::setConfirm(const bool v)
{
	ED_IMP_JIG(this->m_pImpl)->bConfirm = v;
}
void EdDimJig::setKeyword(const bool v) {
	ED_IMP_JIG(this->m_pImpl)->bKeyword = v;
}

DbEntity *EdDimJig::entity() const
{
	return this->entCorner;
}
EdJig::DragStatus EdDimJig::sampler()
{
	setUserInputControls((UserInputControls)(EdJig::kNullResponseAccepted |
											 EdJig::kNullResponseAccepted |
											 EdJig::kGovernedByOrthoMode |
											 EdJig::kAcceptOtherInputString));


	EdJig::DragStatus sta = DragStatus::kNormal;
	if (this->m_bEnableDragline == true && this->m_bDimReferPoint == true)
	{
		sta = this->acquirePoint(this->m_curPoint, this->m_dimReferPoint);
	}
	else if (this->m_bEnableDragline == true && this->m_bDimReferPoint == false)
	{
		sta = this->acquirePoint(this->m_curPoint, this->m_ucsOrigin);
	}
	else if (this->m_bEnableDragline == false && this->m_bDimReferPoint == true)
	{
		sta = this->acquirePoint(this->m_curPoint);
	}
	else if (this->m_bEnableDragline == false && this->m_bDimReferPoint == false)
	{
		sta = this->acquirePoint(this->m_curPoint);
	}

	AcString inputStr;
	acquireString(inputStr);
	if (inputStr.length() > 0)
	{
		AcString str;
		AcArray<AcString> keys;
		AcString keyword = AcString(this->keywordList());
		for (int i = 0; i < keyword.length(); i++)
		{
			if (keyword[i] == ' ' || i == keyword.length() - 1)
			{
				if (i == keyword.length() - 1)
				{
					str += keyword[i];
				}
				keys.append(str);
				str = "";
				continue;
			}

			str += keyword[i];
		}

		for (int i = 0; i < keys.length(); i++)
		{
			if (keys[i].makeUpper() != inputStr.makeUpper())
			{
				continue;
			}

			if (i == 0)
			{
				sta = EdJig::DragStatus::kKW1;
			}
			else if (i == 1)
			{
				sta = EdJig::DragStatus::kKW2;
			}
			else if (i == 2)
			{
				sta = EdJig::DragStatus::kKW3;
			}
			else if (i == 3)
			{
				sta = EdJig::DragStatus::kKW4;
			}
			else if (i == 4)
			{
				sta = EdJig::DragStatus::kKW5;
			}
			else if (i == 5)
			{
				sta = EdJig::DragStatus::kKW6;
			}
			else if (i == 6)
			{
				sta = EdJig::DragStatus::kKW7;
			}
			else if (i == 7)
			{
				sta = EdJig::DragStatus::kKW8;
			}
			else if (i == 8)
			{
				sta = EdJig::DragStatus::kKW9;
			}
			break;
		}
	}

	return sta;
}
Adesk::Boolean EdDimJig::update()
{
	return true;
}
DbDimDataPtrArray *EdDimJig::dimData(const double dimScale)
{
	realloc_all(this->m_pDimDatas);
	this->m_pDimDatas = new DbDimDataPtrArray();

	return this->m_pDimDatas;
}
Acad::ErrorStatus EdDimJig::setDimValue(const DbDimData *dimData, const double dimValue)
{

	return Acad::ErrorStatus::eOk;
}
