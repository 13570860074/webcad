#ifndef KERNEL_H
#define KERNEL_H

class RxDictionary;
class RxManagerDictionary;
class RxWorkerThreadManager;
class RxAsyncManager;
class EvEventManager;
class GiObjectManager;
class GiMaterialManager;
class GiEntityManager;
class GiWorldDrawManager;
class GiGeometryExtentManager;
class DbObjectManager;
class DbLayoutManager;
class DbHostApplicationServices;
class EdUserInputManager;
class EdControllerManager;
class ReRendererManager;
class ApDocManager;
class GsElementManager;
class ApplicationManager;
class RxSystemVariableManager;
class RxEnvironmentVariableManager;
class RxConfigurationVariableManager;
class EdSelectionSetManager;
class Kernel {
public:
	Kernel();
	~Kernel();

public:

	/** 事件管理器 */
	EvEventManager *acevEventManager();

	/** 线程管理器 */
	RxWorkerThreadManager* acrxWorkerThreadManager();

	/** 异步管理器 */
	RxAsyncManager *acrxAsyncManager();

	/** 系统变量管理器 */
	RxSystemVariableManager* acrxSystemVariableManager();

	/** 环境变量管理器 */
	RxEnvironmentVariableManager* acrxEnvironmentVariableManager();

	/** 配置变量管理器 */
	RxConfigurationVariableManager* acrxConfigurationVariableManager();

	/** 系统词典管理器 */
	RxDictionary* acrxSysRegistry();

	/** Gi对象管理器 */
	GiObjectManager* acgiObjectManager();

	/** 材质管理器 */
	GiMaterialManager *acgiMaterialManager();

	/** 实体管理器 */
	GiEntityManager* acgiEntityManager();

	/** 范围管理器 */
	GiGeometryExtentManager* acgiGeometryExtentManager();

	/** 用户输入管理器 */
	EdUserInputManager* acedUserInputManager();

	/** 控制器管理器 */
	EdControllerManager* acedControllerManager();

	/** Db对象管理器 */
	DbObjectManager* acdbObjectManager();

	/** 绘图管理器 */
	GiWorldDrawManager* acgiWorldDrawManager();

	/** 布局管理器 */
	DbLayoutManager *acdbLayoutManager();

	/** Db应用服务 */
	DbHostApplicationServices* acdbHostApplicationServices();

	/** 渲染管理器 */
	ReRendererManager* acreRendererManager();

	/** 图元对象管理器 */
	GsElementManager* acgsElementManager();

	/** 选择管理器 */
	EdSelectionSetManager* acedSelectionSetManager();

	/** 文档管理器 */
	ApDocManager* acapDocManager();

	/** app管理器 */
	ApplicationManager* applicationManager();

private:
	void* m_pImpl;
};



extern Kernel* kernel();
extern RxManagerDictionary* acrxManagerDictionary();


extern ReRendererManager* acreRendererManager();
extern GiMaterialManager* acgiMaterialManager();
extern GiEntityManager* acgiEntityManager();
extern EdUserInputManager* acedUserInputManager();

#endif




