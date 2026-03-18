#include "GiEntityGeometryStake.h"
#include "GeMatrix3d.h"
#include "DbEntity.h"
#include "GiImpl.h"

GiEntityGeometryStake::GiEntityGeometryStake(){
    this->m_entity = NULL;
    this->m_rootEntity = NULL;
}
GiEntityGeometryStake::~GiEntityGeometryStake(){

}

void GiEntityGeometryStake::setEntity(DbEntity* entity){
    this->m_entity = entity;
}
DbEntity* GiEntityGeometryStake::entity() const{
    return this->m_entity;
}
void GiEntityGeometryStake::setRootEntity(DbEntity* entity){
    this->m_rootEntity = entity;
}
DbEntity* GiEntityGeometryStake::rootEntity() const{
    return this->m_rootEntity;
}
GeMatrix3d GiEntityGeometryStake::matrix()const{
    return this->m_matrix;
}
void GiEntityGeometryStake::setMatrix(const GeMatrix3d& mat){
    this->m_matrix = mat;
}