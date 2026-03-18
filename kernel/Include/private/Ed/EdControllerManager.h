#ifndef EDCONTROLLERMANAGER_H
#define EDCONTROLLERMANAGER_H


#include "RxManager.h"
#include "GePoint2d.h"
#include "DbObjectIdArray.h"


class EdController;
class EdControllerManager : public RxObject, public RxManager {
public:
	EdControllerManager();
	virtual ~EdControllerManager();
	RX_DECLARE_MEMBERS(EdControllerManager);
	RX_MANAGER_DECLARE_MEMBERS(EdControllerManager);

private:

	// 初始化
	void onInit();

public:

	/** 设置视口缩放 */
	static void viewportZoom(const GePoint2d& ncsPoint, double _v);

	/** 设置视口平移 */
	static void viewportMove(const GePoint2d& dcs1, const GePoint2d& dcs2);
	
	/** 添加控制器 */
	void appendController(EdController* controller);

	/** 控制器数量 */
	int numController() const;

};


#endif