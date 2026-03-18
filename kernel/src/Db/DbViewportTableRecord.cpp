#include "DbViewportTableRecord.h"
#include "ReRendererManager.h"
#include "DbDwgFiler.h"
#include "kernel.h"
#include "DbImpl.h"

DbViewportTableRecord::DbViewportTableRecord()
{
	this->m_pImpl = new DbViewportTableRecordImpl();
}
DbViewportTableRecord::~DbViewportTableRecord()
{
}

void DbViewportTableRecord::close()
{
	DbAbstractViewTableRecord::close();
}

Int16 DbViewportTableRecord::number() const
{
	return 0;
}
GePoint2d DbViewportTableRecord::lowerLeftCorner() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->lowerLeftCorner;
}
void DbViewportTableRecord::setLowerLeftCorner(const GePoint2d &pt)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->lowerLeftCorner = pt;
}
GePoint2d DbViewportTableRecord::upperRightCorner() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->upperRightCorner;
}
void DbViewportTableRecord::setUpperRightCorner(const GePoint2d &pt)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->upperRightCorner = pt;
}
bool DbViewportTableRecord::ucsFollowMode() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->ucsFollowMode;
}
void DbViewportTableRecord::setUcsFollowMode(bool enabled)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->ucsFollowMode = enabled;
}
UInt16 DbViewportTableRecord::circleSides() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->circleSides;
}
void DbViewportTableRecord::setCircleSides(UInt16 circleSides)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->circleSides = circleSides;
}
bool DbViewportTableRecord::fastZoomsEnabled() const
{
	return true;
}
void DbViewportTableRecord::setFastZoomsEnabled(bool enabled)
{
}
bool DbViewportTableRecord::iconEnabled() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->iconEnabled;
}
void DbViewportTableRecord::setIconEnabled(bool enabled)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->iconEnabled = enabled;
}
bool DbViewportTableRecord::iconAtOrigin() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->iconEnabled;
}
void DbViewportTableRecord::setIconAtOrigin(bool atOrigin)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->iconEnabled = atOrigin;
}
bool DbViewportTableRecord::gridEnabled() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->gridEnabled;
}
void DbViewportTableRecord::setGridEnabled(bool enabled)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->gridEnabled = enabled;
}
GePoint2d DbViewportTableRecord::gridIncrements() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->gridIncrements;
}
void DbViewportTableRecord::setGridIncrements(const GePoint2d &base)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->gridIncrements = base;
}
bool DbViewportTableRecord::snapEnabled() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapEnabled;
}
void DbViewportTableRecord::setSnapEnabled(bool enabled)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapEnabled = enabled;
}
bool DbViewportTableRecord::isometricSnapEnabled() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->isometricSnapEnabled;
}
void DbViewportTableRecord::setIsometricSnapEnabled(bool enabled)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->isometricSnapEnabled = enabled;
}
Int16 DbViewportTableRecord::snapPair() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapPair;
}
void DbViewportTableRecord::setSnapPair(Int16 pairType)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapPair = pairType;
}
double DbViewportTableRecord::snapAngle() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapAngle;
}
void DbViewportTableRecord::setSnapAngle(double angle)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapAngle = angle;
}
GePoint2d DbViewportTableRecord::snapBase() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapBase;
}
void DbViewportTableRecord::setSnapBase(const GePoint2d &base)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapBase = base;
}

GePoint2d DbViewportTableRecord::snapIncrements() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapIncrements;
}
void DbViewportTableRecord::setSnapIncrements(const GePoint2d &base)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->snapIncrements = base;
}
bool DbViewportTableRecord::isUcsSavedWithViewport() const
{
	return DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->isUcsSavedWithViewport;
}
void DbViewportTableRecord::setUcsPerViewport(bool ucsvp)
{
	DB_IMP_VIEWPORTTABLERECORD(this->m_pImpl)->isUcsSavedWithViewport = ucsvp;
}

Acad::ErrorStatus DbViewportTableRecord::dwgInFields(DbDwgFiler *pFiler)
{

	Acad::ErrorStatus es = DbAbstractViewTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	GePoint2d lowerLeftCorner;
	pFiler->readPoint2d(&lowerLeftCorner);
	this->setLowerLeftCorner(lowerLeftCorner);

	GePoint2d upperRightCorner;
	pFiler->readPoint2d(&upperRightCorner);
	this->setUpperRightCorner(upperRightCorner);

	bool ucsFollowMode = false;
	pFiler->readBoolean(&ucsFollowMode);
	this->setUcsFollowMode(ucsFollowMode);

	UInt16 circleSides;
	pFiler->readUInt16(&circleSides);
	this->setCircleSides(circleSides);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewportTableRecord::dwgOutFields(DbDwgFiler *pFiler) const
{

	Acad::ErrorStatus es = DbAbstractViewTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	pFiler->writePoint2d(this->lowerLeftCorner());
	pFiler->writePoint2d(this->upperRightCorner());
	pFiler->writeBoolean(this->ucsFollowMode());
	pFiler->writeUInt16(this->circleSides());

	return Acad::ErrorStatus::eOk;
}
