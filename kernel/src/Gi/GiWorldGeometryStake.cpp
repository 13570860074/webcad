#include "GiWorldGeometryStake.h"
#include "GiMaterial.h"
#include "GiEntityGeometry.h"


GiWorldGeometryStake::GiWorldGeometryStake() {
	this->m_entity = NULL;
	this->m_geometry = NULL;
	this->m_material = NULL;
}
GiWorldGeometryStake::~GiWorldGeometryStake() {

}
GiMaterial* GiWorldGeometryStake::material()const {
	return this->m_material;
}
void GiWorldGeometryStake::setMaterial(GiMaterial* material) {
	this->m_material = material;
}

DbEntity* GiWorldGeometryStake::entity()const {
	return this->m_entity;
}
void GiWorldGeometryStake::setEntity(DbEntity* ent) {
	this->m_entity = ent;
}

GiEntityGeometry* GiWorldGeometryStake::geometry()const {
	return this->m_geometry;
}
void GiWorldGeometryStake::setGeometry(GiEntityGeometry* g) {
	this->m_geometry = g;
}