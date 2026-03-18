#include "DbGripData.h"



DbGripData::DbGripData() {
	this->m_index = 0;
	this->m_gripMode = DbGripMode::kGripModeDrag;
	this->m_rotationAxis = GeVector3d::kZAxis;
}
DbGripData::~DbGripData() {

}

const DbObjectId DbGripData::objectId()const {
	return this->m_id;
}
void DbGripData::setObjectId(const DbObjectId& _id) {
	this->m_id = _id;
}
unsigned int DbGripData::index() const {
	return this->m_index;
}
void DbGripData::setIndex(const unsigned int i) {
	this->m_index = i;
}
const GePoint3d& DbGripData::gripPoint() const {
	return this->m_gripPt;
}
void DbGripData::setGripPoint(const GePoint3d& pt) {
	this->m_gripPt = pt;
}
const DbGripData::DbGripMode& DbGripData::gripMode() const {
	return this->m_gripMode;
}
void DbGripData::setGripMode(const DbGripData::DbGripMode& mode) {
	this->m_gripMode = mode;
}
const GeScale3d DbGripData::scale()const {
	return this->m_scale;
}
void DbGripData::setScale(const GeScale3d& _scale) {
	this->m_scale = _scale;
}
const GeVector3d DbGripData::direction()const {
	return this->m_direction;
}
void DbGripData::setDirection(const GeVector3d& _direction) {
	this->m_direction = _direction;
}
const double DbGripData::angle()const {
	return this->m_rotationAxis.length();
}
const GeVector3d DbGripData::rotationAxis() const {
	return this->m_rotationAxis;
}
void DbGripData::setRotation(const double angle, const GeVector3d& _axis) {
	this->m_rotationAxis = _axis.normal() * angle;
}

