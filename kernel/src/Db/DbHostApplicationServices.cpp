#include "DbHostApplicationServices.h"
#include "DbLayoutManager.h"
#include "DbDatabase.h"
#include "ApWorkDocManager.h"
#include "PrDXFParser.h"
#include "kernel.h"
#include "DbImpl.h"


DbHostApplicationServices* acdbHostApplicationServices() {
	return ::kernel()->acdbHostApplicationServices();
}



DbHostApplicationServices::DbHostApplicationServices() {
	this->m_pImpl = new DbHostApplicationServicesImpl();
}
DbHostApplicationServices::~DbHostApplicationServices() {

}

#if EMSDK
void DbHostApplicationServices::emsdk_load_dxf_data(const std::string& data) {
	PrDXFParser* pParser = new PrDXFParser();
	DbDatabase* database = pParser->loadBuffer(data);
	delete pParser;

	ApWorkDocManager::cast(acDocManager)->appDatabaseNewDocument(database, "drawing.fwg");
}
#endif

DbDatabase* DbHostApplicationServices::workingDatabase() const {
	return DB_IMP_HOSTAPPLICATIONSERVICES(this->m_pImpl)->workingDatabase;
}
void DbHostApplicationServices::setWorkingDatabase(DbDatabase* pDatabase) {
	DB_IMP_HOSTAPPLICATIONSERVICES(this->m_pImpl)->workingDatabase = pDatabase;
}


DbLayoutManager* DbHostApplicationServices::layoutManager() const {


	return ::kernel()->acdbLayoutManager();
}