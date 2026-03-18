#ifndef DBHOSTAPPLICATIONSERVICES_H
#define DBHOSTAPPLICATIONSERVICES_H

#include "RxObject.h"

class DbDatabase;
class DbLayoutManager;
class DbHostApplicationServices : public RxObject {
public:
	DbHostApplicationServices();
	virtual ~DbHostApplicationServices();
	RX_DECLARE_MEMBERS(DbHostApplicationServices);

#if EMSDK
	void emsdk_load_dxf_data(const std::string& data);
#endif

	DbDatabase* workingDatabase() const;
	void setWorkingDatabase(DbDatabase* pDatabase);

	virtual DbLayoutManager* layoutManager() const;
};

extern DbHostApplicationServices* acdbHostApplicationServices();


#endif