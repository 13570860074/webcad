
#include "DbOsnapAnchorPoint.h"






DbOsnapAnchorPoint::DbOsnapAnchorPoint() {
	this->m_osnapPt = GePoint3d::kOrigin;
	this->m_dcsOosnapPt = GePoint2d::kOrigin;
	this->m_osnapMode = DbOsnapData::OsnapMode::kOsModeNone;
}
DbOsnapAnchorPoint::~DbOsnapAnchorPoint() {
	realloc_arry_all(this->m_stakes);
}

const DbOsnapData::OsnapMode& DbOsnapAnchorPoint::osnapMode() const {
	return this->m_osnapMode;
}
void DbOsnapAnchorPoint::setOsnapMode(const DbOsnapData::OsnapMode& mode) {
	this->m_osnapMode = mode;
}

const GePoint3d& DbOsnapAnchorPoint::osnapPoint() const {
	return this->m_osnapPt;
}
void DbOsnapAnchorPoint::setOsnapPoint(const GePoint3d& pt) {
	this->m_osnapPt = pt;
}
const GePoint2d& DbOsnapAnchorPoint::dcsOsnapPoint() const {
	return this->m_dcsOosnapPt;
}
void DbOsnapAnchorPoint::setDcsOsnapPoint(const GePoint2d& pt) {
	this->m_dcsOosnapPt = pt;
}

AcArray<DbOsnapAnchorPointStake*>& DbOsnapAnchorPoint::stakes() {
	return this->m_stakes;
}

