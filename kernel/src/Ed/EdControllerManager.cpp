#include "EdControllerManager.h"
#include "EdController.h"
#include "EdAcadController.h"
#include "EdTouchController.h"
#include "EdUserInputManager.h"
#include "DbObjectManager.h"
#include "arxHeaders.h"
#include "acedTrans.h"
#include "kernel.h"
#include "EdImpl.h"
#include "DbImpl.h"

EdControllerManager::EdControllerManager()
{
	this->m_pImpl = new EdControllerManagerImpl();
}
EdControllerManager::~EdControllerManager()
{
}

void EdControllerManager::onInit()
{

	// 创建Acad控制器
	EdAcadController *pController = new EdAcadController();
	this->appendController(pController);

	// 创建触屏控制器
	EdTouchController *pTouchController = new EdTouchController();
	this->appendController(pTouchController);
}
void EdControllerManager::appendController(EdController *controller)
{

	ED_IMP_CONTROLLERMANAGER(this->m_pImpl)->controllers->append(controller);

	// 触发初始化
	controller->onInit();
}
int EdControllerManager::numController() const
{
	return ED_IMP_CONTROLLERMANAGER(this->m_pImpl)->controllers->length();
}
void EdControllerManager::viewportZoom(const GePoint2d& ncsPoint, double _v)
{
	do
	{
		// 获得需要缩放的视口
		DbObject* viewport = NULL;
		DbObjectId viewportId = acedActiveViewportId();
		if (acdbOpenObject(viewport, viewportId) != Acad::ErrorStatus::eOk)
		{
			break;
		}

		// 如果是视口且锁定则取第一个视口为基准视口
		if (viewport->isA() == DbViewport::desc())
		{
			DbViewport* pViewport = (DbViewport*)viewport;

			// 判断是否被锁定,如果被锁定则取布局中第一个视口为基准视口
			if (pViewport->isLocked() == true)
			{
				// 获得当前布局id
				DbObjectId layoutId = acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId();

				// 打开布局
				DbLayout* pLayout = NULL;
				if (acdbOpenObject(pLayout, layoutId) == Acad::ErrorStatus::eOk)
				{
					// 获得所有视口
					DbObjectIdArray viewportIds = pLayout->getViewportArray();
					pLayout->close();

					// 获得第一个视口
					for (int i = 0; i < viewportIds.length(); i++)
					{
						if (viewportIds[i] == viewportId)
						{
							continue;
						}
						DbViewport* tempViewport = NULL;
						if (acdbOpenObject(tempViewport, viewportIds[i]) == Acad::ErrorStatus::eOk)
						{
							int number = tempViewport->number();
							tempViewport->close();
							if (number == 1)
							{
								viewportId = viewportIds[i];
								break;
							}
						}
					}
				}
			}
		}
		viewport->close();

		// 设置视口缩放
		if (acdbOpenObject(viewport, viewportId, Db::OpenMode::kForWrite) != Acad::ErrorStatus::eOk)
		{
			break;
		}
		while (viewport->isA() == DbViewportTableRecord::desc())
		{
			DbViewportTableRecord* pViewportTableRecord = (DbViewportTableRecord*)viewport;

			double viewHeight = pViewportTableRecord->height() * _v;
			if (viewHeight < ACDB_VIEWPORT_EXTMINSIZE)
			{
				break;
			}
			else if (viewHeight > ACDB_VIEWPORT_EXTMAXSIZE)
			{
				break;
			}

			// 获得vcs1
			GePoint2d vcs1 = ::trans_ncs_to_vcs(::kernel()->acedUserInputManager()->deviceLocation());

			// 获得vcs2
			GePoint2d vcs2 = vcs1;
			if (_v < 0)
			{
				vcs2.scaleBy(_v, GePoint2d(0.5, 0.5));
			}
			else if (_v > 0)
			{
				vcs2.scaleBy(_v, GePoint2d(0.5, 0.5));
			}

			// 设视口高度
			pViewportTableRecord->setHeight(viewHeight);
			pViewportTableRecord->setWidth(pViewportTableRecord->height() * ::kernel()->acedUserInputManager()->screenAspect());
			pViewportTableRecord->setWidth(pViewportTableRecord->width() * (pViewportTableRecord->upperRightCorner().x - pViewportTableRecord->lowerLeftCorner().x));

			GePoint2d dcs1 = ::trans_vcs_to_dcs(vcs1);
			GePoint2d dcs2 = ::trans_vcs_to_dcs(vcs2);

			// 重新计算视图中心
			GePoint2d viewCenter = pViewportTableRecord->centerPoint();
			viewCenter += (dcs1 - dcs2);
			pViewportTableRecord->setCenterPoint(viewCenter);
			break;
		}
		while (viewport->isA() == DbViewport::desc())
		{
			DbViewport* pViewport = (DbViewport*)viewport;

			double viewHeight = pViewport->viewHeight() * _v;
			if (viewHeight < ACDB_VIEWPORT_EXTMINSIZE)
			{
				break;
			}
			else if (viewHeight > ACDB_VIEWPORT_EXTMAXSIZE)
			{
				break;
			}

			// 获得视口位置
			DbExtents2d extents = viewport_on_space_extents(pViewport->objectId());

			// 获得vcs1
			GePoint2d vcs1 = ::trans_ncs_to_vcs(::kernel()->acedUserInputManager()->deviceLocation(), viewportId);

			// 获得vcs2
			GePoint2d vcs2 = vcs1;
			if (_v < 0.0)
			{
				vcs2.scaleBy(1.1, GePoint2d(0.5, 0.5));
			}
			else if (_v > 0.0)
			{
				vcs2.scaleBy(0.9, GePoint2d(0.5, 0.5));
			}

			// 设置视口高度
			DB_IMP_VIEWPORT(pViewport->m_pImpl)->viewHeight = viewHeight;

			GePoint2d dcs1 = ::trans_vcs_to_dcs(vcs1, viewportId);
			GePoint2d dcs2 = ::trans_vcs_to_dcs(vcs2, viewportId);

			// 重新计算视图中心
			GePoint2d viewCenter = pViewport->viewCenter();
			viewCenter += (dcs1 - dcs2);
			pViewport->setViewCenter(viewCenter);
			break;
		}
		viewport->close();

	} while (false);
}
void EdControllerManager::viewportMove(const GePoint2d& _dcs1, const GePoint2d& _dcs2)
{

	do
	{

		GePoint2d dcs1 = _dcs1;
		GePoint2d dcs2 = _dcs2;

		// 获得当前被激活视口
		DbObjectId activeViewportId = acedActiveViewportId();
		DbObject* object = ::kernel()->acdbObjectManager()->openDbObject(activeViewportId);
		if (object == NULL)
		{
			break;
		}

		if (DB_IMP_OBJECT(object->m_pImpl)->isViewportTableRecord() == true)
		{

			// 屏幕点转视口坐标
			DbViewportTableRecord* pViewportTableRecord = (DbViewportTableRecord*)object;
			if (::acdbOpenObject(pViewportTableRecord, activeViewportId, Db::OpenMode::kForWrite) == 0)
			{
				// 获得位移
				GeVector2d displacement;
				displacement.x = dcs2.x - dcs1.x;
				displacement.y = dcs2.y - dcs1.y;

				// 设置视口中点
				GePoint2d centerPoint = pViewportTableRecord->centerPoint();
				centerPoint.x -= displacement.x;
				centerPoint.y -= displacement.y;
				pViewportTableRecord->setCenterPoint(centerPoint);

				// 关闭视口对象
				pViewportTableRecord->close();
			}
		}
		else if (DB_IMP_OBJECT(object->m_pImpl)->isViewport() == true)
		{
			// 判断当前视口是否被锁定
			bool isLocked = false;
			DbViewport* pViewport = NULL;
			if (::acdbOpenObject(pViewport, activeViewportId, Db::OpenMode::kForWrite) == 0)
			{
				isLocked = pViewport->isLocked();
				pViewport->close();
			}

			// 如果当前视口被锁定则将第一个视口作为基准视口
			if (isLocked == true)
			{
				// 重新计算获得ncs
				GePoint2d ncs1 = ::trans_dcs_to_ncs(dcs1, activeViewportId);
				GePoint2d ncs2 = ::trans_dcs_to_ncs(dcs2, activeViewportId);

				// 获得当前布局id
				DbObjectId layoutId = acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId();

				// 获得布局
				DbLayout* pLayout = NULL;
				if (::acdbOpenObject(pLayout, layoutId) == Acad::ErrorStatus::eOk)
				{
					// 获得视口
					DbObjectIdArray viewportIds = pLayout->getViewportArray();

					// 获得图纸空间视口
					for (int i = 0; i < viewportIds.length(); i++)
					{
						DbViewport* viewport = NULL;
						if (::acdbOpenObject(viewport, viewportIds[i]) == Acad::ErrorStatus::eOk)
						{
							if (viewport->number() == 1)
							{
								activeViewportId = viewportIds[i];
							}
							viewport->close();
						}
					}

					pLayout->close();
				}

				// 转换为dcs
				dcs1 = ::trans_ncs_to_dcs(ncs1, activeViewportId);
				dcs2 = ::trans_ncs_to_dcs(ncs2, activeViewportId);
			}

			// 设置视口移动
			if (::acdbOpenObject(pViewport, activeViewportId, Db::OpenMode::kForWrite) == 0)
			{
				//  获得位移
				GeVector2d displacement;
				displacement.x = dcs2.x - dcs1.x;
				displacement.y = dcs2.y - dcs1.y;

				// 设置视口中点
				GePoint2d centerPoint = pViewport->viewCenter();
				centerPoint.x -= displacement.x;
				centerPoint.y -= displacement.y;
				pViewport->setViewCenter(centerPoint);

				// 关闭视口
				pViewport->close();
			}
		}

	} while (false);
}
