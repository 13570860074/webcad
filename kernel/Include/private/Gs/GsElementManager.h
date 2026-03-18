#ifndef GSELEMENTMANAGER_H
#define GSELEMENTMANAGER_H


#include "RxManager.h"
#include "GsElement.h"

class GsCursor;
class GsCoordSystem;
class ApDocument;
class EdMouseEvent;
class EdTouchEvent;
class GsElementManager : public RxObject, public RxManager {
public:
    GsElementManager();
    virtual ~GsElementManager();
    RX_DECLARE_MEMBERS(GsElementManager);
    RX_MANAGER_DECLARE_MEMBERS(GsElementManager);

private:

    static void onBeginAnimationFrame(GsElementManager* pointer);
    static void onDocumentCreated(GsElementManager* pointer, ApDocument* pDocCreating);
    static void onSysVarChanged(GsElementManager* pointer, const ACHAR* varName, bool success);
    static void onBeginDrag(GsElementManager* pointer);
    static void onEndDrag(GsElementManager* pointer);
    static void onMousedown(GsElementManager* pointer, const EdMouseEvent& mouseEvent);
    static void onTouchstart(GsElementManager* pointer, const EdTouchEvent& touchEvent);

    /** 初始化 */
    void onInit();

    /** 添加元素 */
    void addElement(GsElement* ele);

public:

    /** 获得当前光标对象 */
    GsCursor* cursor() const;

    /** 获得当前坐标系对象 */
    GsCoordSystem* coordSystem() const;
};




#endif