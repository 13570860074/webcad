#ifndef GIENTITYGEOMETRYSTAKE_H
#define GIENTITYGEOMETRYSTAKE_H

#include "AcArray.h"
#include "DbEntity.h"
#include "GeMatrix3d.h"
#include "RxManager.h"


class GiEntityGeometryStake {
public:
    GiEntityGeometryStake();
    virtual ~GiEntityGeometryStake();

private:
    DbEntity *m_entity;
    DbEntity *m_rootEntity;
    GeMatrix3d m_matrix;

public:
	/* 设置DbEntity */
	void setEntity(DbEntity* entity);

	/* 获得DbEntity */
	DbEntity* entity() const;

	/* 设置Stake */
	void setRootEntity(DbEntity* entity);

	/* 获得Stake */
	DbEntity* rootEntity() const;

    /* 获得matrix */
    GeMatrix3d matrix()const;

    /* 设置matrix */
    void setMatrix(const GeMatrix3d& mat);
};




#endif