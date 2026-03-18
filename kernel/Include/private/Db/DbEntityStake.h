#ifndef DBENTITYSTATE_H
#define DBENTITYSTATE_H

#include "AcArray.h"
#include "GeMatrix3d.h"

class DbEntity;
class DbEntityStake {
public:
	DbEntityStake();
	virtual ~DbEntityStake();

private:
	DbEntity* m_entity;
	DbEntityStake *m_previous;
	GeMatrix3d* m_matrix;
	AcArray<DbEntityStake*> m_stake;

public:

	static void free(DbEntityStake* stake);

	AcArray<DbEntityStake*>* stakes();

	DbEntity* entity()const;
	void setEntity(DbEntity *ent);

	DbEntityStake *previous() const;
	void setPrevious(DbEntityStake *p);

	GeMatrix3d matrix()const;
	void setMatrix(const GeMatrix3d& mat);

	DbEntityStake* root();
	DbEntity *rootEntity();
	GeMatrix3d rootMatrix();
};


#endif
