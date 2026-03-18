#ifndef RERENDERERMANAGER_H
#define RERENDERERMANAGER_H

#include "AcArray.h"
#include "RxManager.h"


class DbObject;
class DbObjectId;
class ReUserInput;
class ReRendererReactor;
class ReRendererManager : public RxObject, public RxManager {
public:
    ReRendererManager();
    virtual ~ReRendererManager();
    RX_DECLARE_MEMBERS(ReRendererManager);
    RX_MANAGER_DECLARE_MEMBERS(ReRendererManager);

    enum ERendererMode {

        /** 所有视口 */
        kAllViewport,

        /** 工作视口 */
        kWorkViewport,
    };

public:

    static void onBeginDrag(ReRendererManager* pointer);
    static void onEndDrag(ReRendererManager* pointer);

    static void onViewChanged(ReRendererManager * pointer);

    static void onAsyncBegin(ReRendererManager* pointer);
    static void onAsyncEnd(ReRendererManager* pointer);
    static void onAsyncAwaitBegin(ReRendererManager* pointer);
    static void onAsyncAwaitEnd(ReRendererManager* pointer);

public:

    // 初始化
    void onInit();

    // 渲染视口
    void rendererViewport(DbObject* object, const DbObjectId& activeViewportId, const DbObjectId& blockTableRecordId);

    // 渲染工作视口
    void rendererWorkViewport();

    // 渲染所有视口
    void rendererViewports();

public:

    /** 计算渲染 */
    void renderer();

    /** 一帧 */
    void animationFrame();

    /** 添加反应器 */
    void addReactor(ReRendererReactor* reactor);

    /** 删除反应器 */
    void removeReactor(ReRendererReactor* reactor);

    /** 渲染模式 */
    ERendererMode rendererMode();

    /** 设置渲染模式 */
    void setRendererMode(ERendererMode mode);
};




#endif