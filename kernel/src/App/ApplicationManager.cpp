#include "ApplicationManager.h"
#include "RxSystemVariableManager.h"
#include "RxWorkerThreadManager.h"
#include "RxAsyncManager.h"
#include "Application.h"
#include "GiEntityManager.h"
#include "GiLineSegmentGeometry.h"
#include "GiTextStyle.h"
#include "DbObjectManager.h"
#include "arxHeaders.h"
#include "acedTrans.h"
#include "kernel.h"
#include "RxImpl.h"
#include "DbImpl.h"
#include <iostream>
#include <fstream>


#include "GiComputeDisplay.h"

class ApplicationFun
{
public:
	ApplicationFun()
	{
		this->pointer = NULL;
	}

	AcString className;
	Application* pointer;
	std::function<Application* ()> fun;
};
AcArray<ApplicationFun*> functionApplications;

ApplicationManager::ApplicationManager()
{
}
ApplicationManager::~ApplicationManager()
{
}
void ApplicationManager::registerApplication(std::function<Application* ()> _fun, const AcString& _className)
{
	ApplicationFun* applicationFun = new ApplicationFun();
	applicationFun->className = _className;
	applicationFun->fun = _fun;
	applicationFun->pointer = NULL;
	functionApplications.append(applicationFun);
}
Application* ApplicationManager::application(const AcString& _className)
{
	for (int i = 0; i < functionApplications.length(); i++)
	{
		if (functionApplications[i]->className == _className)
		{
			return functionApplications[i]->pointer;
		}
	}
	return NULL;
}



static void test()
{
	std::cout << "测试" << std::endl;


	{

		AcDebug::startTag();

		DbDatabase* workingDatabase = ::acdbHostApplicationServices()->workingDatabase();

		const AcArray<DbObject*>* objects = ::kernel()->acdbObjectManager()->queryObjects();

		int nEntity = 0;
		AcArray<GiEntityGeometry*> geometrys;
		for (int i = 0; i < objects->length(); i++) {
			if (objects->at(i) == NULL) {
				continue;
			}
			if (DB_IMP_OBJECT(objects->at(i)->m_pImpl)->isEntity() == false) {
				continue;
			}
			nEntity++;
			//if (objects->at(i)->database() == workingDatabase) {
				AcArray<GiEntityGeometry*> entitys = DB_IMP_ENTITY(objects->at(i)->m_pImpl)->entitys;
				for (int u = 0; u < entitys.length(); u++) {
					geometrys.append(entitys[u]);
				}
			//}
		}
		std::cout << "objects:" << objects->length() << std::endl;
		std::cout << "nEntity:" << nEntity << std::endl;
		std::cout << "geometrys:"<< geometrys.length() << std::endl;
		std::cout << "测试2" << std::endl;

		GeMatrix3d displayMatrix;
		displayMatrix.setToRotation(1.2, GeVector3d(0.1, 0.24, 0.36));


		/** 获得所有工作线程 */
		unsigned int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
		unsigned int countGeometryThread = int(geometrys.length() / ((numWorkerThread + 1) * 1.0));

		/** 创建线程并分配任务 */
		AcArray<GiComputeDisplay*> computeDisplays;
		if (geometrys.length() > 10000)
		{
			for (int i = 0; i < numWorkerThread; i++)
			{
				int start = i * countGeometryThread;
				int end = (i + 1) * countGeometryThread;

				GiComputeDisplay* computeDisplay = new GiComputeDisplay();
				computeDisplay->matrix = displayMatrix;
				computeDisplay->defaultMatrix = false;
				computeDisplay->isMainThread = false;
				for (int u = start; u < end; u++)
				{
					computeDisplay->allGeometrys.append(geometrys[u]);
				}
				computeDisplays.append(computeDisplay);
			}
		}
		GiComputeDisplay* computeDisplay = new GiComputeDisplay();
		computeDisplay->matrix = displayMatrix;
		computeDisplay->defaultMatrix = false;
		computeDisplay->isMainThread = true;
		if (geometrys.length() <= 10000)
		{
			for (int u = 0; u < geometrys.length(); u++)
			{
				computeDisplay->allGeometrys.append(geometrys[u]);
			}
		}
		else
		{
			int start = numWorkerThread * countGeometryThread;
			int end = geometrys.length();
			for (int u = start; u < end; u++)
			{
				computeDisplay->allGeometrys.append(geometrys[u]);
			}
		}
		computeDisplays.append(computeDisplay);

		/** 计算 */
		for (int i = 0; i < computeDisplays.length(); i++)
		{
			if (computeDisplays.at(i)->isMainThread == false) {
				::kernel()->acrxWorkerThreadManager()->addTask((void (*)(void*))GiComputeDisplay::compute, computeDisplays.at(i), false);
			}
		}
		for (int i = 0; i < computeDisplays.length(); i++)
		{
			if (computeDisplays.at(i)->isMainThread == true)
			{
				GiComputeDisplay::compute(computeDisplay);
			}
		}

		/** 等待计算完成 */
		while (true)
		{
			bool isDone = true;
			for (int i = 0; i < computeDisplays.length(); i++)
			{
				if (computeDisplays.at(i)->isDone.load() == false)
				{
					isDone = false;
					break;
				}
			}
			if (isDone == true)
			{
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		/** 删除对象 */
		realloc_arry_all(computeDisplays);

		AcDebug::endTag();
	}


	return;

	DbBlockTable* pBlockTable = NULL;
	DbObjectId currentSpaceId = acdbHostApplicationServices()->workingDatabase()->currentSpaceId();

	DbBlockTableRecord* pBlockTableRecord = NULL;
	if (::acdbOpenObject(pBlockTableRecord, currentSpaceId, Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk)
	{
		DbLine* line = new DbLine();
		line->setStartPoint(GePoint3d(0, 0, 0));
		line->setEndPoint(GePoint3d(100, 100, 0));

		DbObjectId id;
		pBlockTableRecord->appendDbEntity(id, line);
		pBlockTableRecord->close();
		line->close();
	}
}


void ApplicationManager::onInit()
{

	for (int i = 0; i < functionApplications.length(); i++)
	{
		ApplicationFun* applicationFun = functionApplications.at(i);
		Application* app = applicationFun->fun();
		applicationFun->pointer = app;
		app->onInit();
	}

	// ACRX_CMD_MODAL(0)  在使用其他命令时不能调用该命令
	// ACRX_CMD_TRANSPARENT (1)  在使用其他命令时可以调用该命令
	// ACRX_CMD_NO_UNDO_MARKER 命令使用时没有撤销机制
	// ACRX_CMD_NOHISTORY 命令不会被重复
	// ACRX_CMD_USEPICKSET 拾取命令(增加该参数后使用命令时不会取消当前选择集,但会删除夹点)
	// ACRX_CMD_REDRAW 在使用命令时不会取消选择集且保留夹点
	// ACRX_CMD_NOPAPERSPACE 图纸空间不允许
	// ACRX_CMD_NOBEDIT 禁止在块编辑器中使用
	// ACRX_CMD_NOTILEMODE 禁止在模型空间中使用
	// ACRX_CMD_UNDEFINED 让命令变成未定义命令

	/** 测试 */
	acedRegCmds->addCommand("HelloArx", "TEST", "TEST", ACRX_CMD_MODAL, test);
}
