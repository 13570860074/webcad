#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include "AcArray.h"
#include "RxManager.h"

class Application;
class ApplicationManager : public RxObject, public RxManager
{
public:
    ApplicationManager();
    virtual ~ApplicationManager();
    RX_DECLARE_MEMBERS(ApplicationManager);
    RX_MANAGER_DECLARE_MEMBERS(ApplicationManager);

public:
    // 注册应用
    static void registerApplication(std::function<Application *()> _fun, const AcString &_className = "");

    // 获得对象
    static Application *application(const AcString &_className);


public:
    // 初始化
    void onInit();
};

#endif