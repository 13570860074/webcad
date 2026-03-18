#include "kernel.h"
#include "RxManagerDictionary.h"
#include "RxWorkerThreadManager.h"
#include "RxSysRegistryManager.h"
#include "RxSystemVariableManager.h"
#include "RxEnvironmentVariableManager.h"
#include "RxConfigurationVariableManager.h"
#include "RxAsyncManager.h"
#include "GiObjectManager.h"
#include "GiMaterialManager.h"
#include "GiEntityManager.h"
#include "GiWorldDrawManager.h"
#include "GiGeometryExtentManager.h"
#include "ReRendererManager.h"
#include "ApWorkDocManager.h"
#include "EvEventManager.h"
#include "DbObjectManager.h"
#include "DbLayoutManager.h"
#include "DbHostApplicationServices.h"
#include "GsElementManager.h"
#include "EdUserInputManager.h"
#include "EdControllerManager.h"
#include "EdSelectionSetManager.h"
#include "ApplicationManager.h"

class KernelImpl
{
public:
	KernelImpl()
	{
		this->m_pEventManager = new EvEventManager();
		this->m_pWorkerThreadManager = new RxWorkerThreadManager();
		this->m_pAsyncManager = new RxAsyncManager();
		this->m_pSysRegistry = new RxSysRegistryManager();
		this->m_pUserInputManager = new EdUserInputManager();
		this->m_pControllerManager = new EdControllerManager();

		this->m_pGiObjectManager = new GiObjectManager();
		this->m_pDbObjectManager = new DbObjectManager();
		this->m_pLayoutManager = new DbLayoutManager();
		this->m_pHostApplicationServices = new DbHostApplicationServices();
		this->m_pRendererManager = new ReRendererManager();

		this->m_workDocManager = new ApWorkDocManager();
		this->m_pWorldDrawManager = new GiWorldDrawManager();

		this->m_pSelectionSetManager = new EdSelectionSetManager();

		this->m_pGeometryExtentManager = new GiGeometryExtentManager();

		this->m_pElementManager = new GsElementManager();

		this->m_pMaterialManager = new GiMaterialManager();
		this->m_pEntityManager = new GiEntityManager();

		this->m_pApplicationManager = new ApplicationManager();

		this->m_pSystemVariableManager = new RxSystemVariableManager();
		this->m_pEnvironmentVariableManager = new RxEnvironmentVariableManager();
		this->m_pConfigurationVariableManager = new RxConfigurationVariableManager();
	}
	virtual ~KernelImpl()
	{

	}

	EvEventManager* m_pEventManager;
	RxWorkerThreadManager* m_pWorkerThreadManager;
	RxDictionary* m_pSysRegistry;
	RxAsyncManager* m_pAsyncManager;
	EdSelectionSetManager* m_pSelectionSetManager;
	GiObjectManager* m_pGiObjectManager;
	GiMaterialManager* m_pMaterialManager;
	GiEntityManager* m_pEntityManager;
	GiGeometryExtentManager* m_pGeometryExtentManager;
	DbObjectManager* m_pDbObjectManager;
	DbLayoutManager* m_pLayoutManager;
	DbHostApplicationServices* m_pHostApplicationServices;
	EdUserInputManager* m_pUserInputManager;
	EdControllerManager *m_pControllerManager;
	ReRendererManager* m_pRendererManager;
	ApDocManager* m_workDocManager;
	GsElementManager *m_pElementManager;
	GiWorldDrawManager* m_pWorldDrawManager;
	ApplicationManager* m_pApplicationManager;

	RxSystemVariableManager* m_pSystemVariableManager;
	RxEnvironmentVariableManager* m_pEnvironmentVariableManager;
	RxConfigurationVariableManager* m_pConfigurationVariableManager;
};

#define IMPL_KERNEL ((KernelImpl *)this->m_pImpl)

RxManagerDictionary* pManagerDictionary = new RxManagerDictionary();
Kernel* pKernel = new Kernel();

Kernel* kernel()
{
	return pKernel;
}
RxManagerDictionary* acrxManagerDictionary()
{
	return pManagerDictionary;
}

Kernel::Kernel()
{
	this->m_pImpl = new KernelImpl();
}
Kernel::~Kernel()
{
	KernelImpl* pImpl = (KernelImpl*)this->m_pImpl;
	delete pImpl;
}
EvEventManager* Kernel::acevEventManager() {
	return IMPL_KERNEL->m_pEventManager;
}
RxWorkerThreadManager* Kernel::acrxWorkerThreadManager() {
	return IMPL_KERNEL->m_pWorkerThreadManager;
}
RxAsyncManager *Kernel::acrxAsyncManager() {
	return IMPL_KERNEL->m_pAsyncManager;
}
RxDictionary* Kernel::acrxSysRegistry() {
	return IMPL_KERNEL->m_pSysRegistry;
}
RxSystemVariableManager* Kernel::acrxSystemVariableManager()
{
	return IMPL_KERNEL->m_pSystemVariableManager;
}
RxEnvironmentVariableManager* Kernel::acrxEnvironmentVariableManager() {
	return IMPL_KERNEL->m_pEnvironmentVariableManager;
}
RxConfigurationVariableManager* Kernel::acrxConfigurationVariableManager()
{
	return IMPL_KERNEL->m_pConfigurationVariableManager;
}
GiObjectManager* Kernel::acgiObjectManager() {
	return IMPL_KERNEL->m_pGiObjectManager;
}
GiMaterialManager* Kernel::acgiMaterialManager() {
	return IMPL_KERNEL->m_pMaterialManager;
}
GiEntityManager* Kernel::acgiEntityManager() {
	return IMPL_KERNEL->m_pEntityManager;
}
GiGeometryExtentManager* Kernel::acgiGeometryExtentManager() {
	return IMPL_KERNEL->m_pGeometryExtentManager;
}
EdUserInputManager* Kernel::acedUserInputManager() {
	return IMPL_KERNEL->m_pUserInputManager;
}
EdControllerManager* Kernel::acedControllerManager(){
	return IMPL_KERNEL->m_pControllerManager;
}
DbObjectManager* Kernel::acdbObjectManager() {
	return IMPL_KERNEL->m_pDbObjectManager;
}
GiWorldDrawManager* Kernel::acgiWorldDrawManager() {
	return IMPL_KERNEL->m_pWorldDrawManager;
}
DbLayoutManager *Kernel::acdbLayoutManager() {
	return IMPL_KERNEL->m_pLayoutManager;
}
DbHostApplicationServices* Kernel::acdbHostApplicationServices() {
	return IMPL_KERNEL->m_pHostApplicationServices;
}
ReRendererManager* Kernel::acreRendererManager() {
	return IMPL_KERNEL->m_pRendererManager;
}
GsElementManager* Kernel::acgsElementManager() {
	return IMPL_KERNEL->m_pElementManager;
}
EdSelectionSetManager* Kernel::acedSelectionSetManager() {
	return IMPL_KERNEL->m_pSelectionSetManager;
}
ApDocManager* Kernel::acapDocManager() {
	return IMPL_KERNEL->m_workDocManager;
}
ApplicationManager* Kernel::applicationManager() {
	return IMPL_KERNEL->m_pApplicationManager;
}

ReRendererManager* acreRendererManager() {
		return pKernel->acreRendererManager();
}
GiMaterialManager* acgiMaterialManager() {
	return pKernel->acgiMaterialManager();
}
GiEntityManager* acgiEntityManager() {
	return pKernel->acgiEntityManager();
}
EdUserInputManager* acedUserInputManager() {
	return pKernel->acedUserInputManager();
}