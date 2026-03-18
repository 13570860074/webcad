#include "DbOsnapData.h"
#include "GeLine3d.h"
#include "GeLineSeg3d.h"
#include "GeLine2d.h"
#include "GeLineSeg2d.h"
#include "acedTrans.h"

DbOsnapData::DbOsnapData()
{
	this->m_entity = NULL;
	this->m_osnapDir = GeVector3d::kXAxis;
	this->m_osnapPt = GePoint3d::kOrigin;
	this->m_dcsOsnapPt = GePoint2d::kOrigin;
	this->m_osnapMode = DbOsnapData::OsnapMode::kOsModeNone;
	this->m_thisGsMarker = NULL;
	this->m_otherGsMarker = NULL;
}
DbOsnapData::~DbOsnapData()
{
}
DbOsnapData* DbOsnapData::clone()
{
	DbOsnapData* data = new DbOsnapData();
	data->m_entity = this->m_entity;
	data->m_dcsOsnapPt = this->m_dcsOsnapPt;
	data->m_osnapPt = this->m_osnapPt;
	data->m_osnapMode = this->m_osnapMode;
	data->m_osnapDir = this->m_osnapDir;
	data->m_thisGsMarker = this->m_thisGsMarker;
	data->m_otherGsMarker = this->m_otherGsMarker;
	return data;
}
double DbOsnapData::distanceTo(const GePoint2d& _pt) const
{
	double dist = 0.0;
	if (this->osnapMode() == DbOsnapData::OsnapMode::kOsModeExtendLine)
	{
		GePoint2d projection1 = ::trans_wcs_to_dcs(this->osnapPoint());
		GePoint2d projection2 = ::trans_wcs_to_dcs(this->osnapPoint() + this->osnapDir());
		GeLine2d line(projection1, projection2);
		dist = line.distanceTo(_pt);
	}
	else if (this->osnapMode() == DbOsnapData::OsnapMode::kOsModeExtendLineSeg)
	{
		GePoint2d projection1 = ::trans_wcs_to_dcs(this->osnapPoint());
		GePoint2d projection2 = ::trans_wcs_to_dcs(this->osnapPoint() + this->osnapDir());
		GeLineSeg2d line(projection1, projection2);
		dist = line.distanceTo(_pt);
	}
	else
	{
		dist = this->dcsOsnapPoint().distanceTo(this->dcsOsnapPoint());
	}
	return dist;
}
double DbOsnapData::distanceTo(const GePoint3d& _pt) const
{
	double dist = 0.0;
	if (this->osnapMode() == DbOsnapData::OsnapMode::kOsModeExtendLine)
	{
		GeLine3d line(this->osnapPoint(), this->osnapDir());
		dist = line.distanceTo(_pt);
	}
	else if (this->osnapMode() == DbOsnapData::OsnapMode::kOsModeExtendLineSeg)
	{
		GeLineSeg3d line(this->osnapPoint(), this->osnapDir());
		dist = line.distanceTo(_pt);
	}
	else
	{
		dist = this->osnapPoint().distanceTo(_pt);
	}
	return dist;
}
bool DbOsnapData::isOn(const GePoint2d& _pt) const
{
	if (this->distanceTo(_pt) < GeContext::gTol.equalPoint())
	{
		return true;
	}
	return false;
}
DbEntity* DbOsnapData::entity()const {
	return this->m_entity;
}
void DbOsnapData::setEntity(DbEntity* _entity) {
	this->m_entity = _entity;
}
const DbOsnapData::OsnapMode& DbOsnapData::osnapMode() const
{
	return this->m_osnapMode;
}
Adesk::GsMarker DbOsnapData::thisGsMarker()const {
	return this->m_thisGsMarker;
}
void DbOsnapData::setThisGsMarker(Adesk::GsMarker marker) {
	this->m_thisGsMarker = marker;
}

Adesk::GsMarker DbOsnapData::otherGsMarker()const {
	return this->m_otherGsMarker;
}
void DbOsnapData::setOtherGsMarker(Adesk::GsMarker marker) {
	this->m_otherGsMarker = marker;
}
void DbOsnapData::setOsnapMode(const DbOsnapData::OsnapMode& mode)
{
	this->m_osnapMode = mode;
}
const GePoint2d& DbOsnapData::dcsOsnapPoint() const
{
	return this->m_dcsOsnapPt;
}
void DbOsnapData::setDcsOsnapPoint(const GePoint2d& pt)
{
	this->m_dcsOsnapPt = pt;
}
const GePoint3d& DbOsnapData::osnapPoint() const
{
	return this->m_osnapPt;
}
void DbOsnapData::setOsnapPoint(const GePoint3d& pt)
{
	this->m_osnapPt = pt;
}
const GeVector3d& DbOsnapData::osnapDir() const
{
	return this->m_osnapDir;
}
void DbOsnapData::setOsnapDir(const GeVector3d& dir)
{
	this->m_osnapDir = dir;
}
