#ifndef APPLICATION_H
#define APPLICATION_H

#include <functional>
#include "ApplicationManager.h"

class Application
{
public:
    Application();
    virtual ~Application();

public:

    // 初始化
    virtual void onInit();
};

#define APP_REGISTER_MALLOC(CLASSNAME) \
    class CLASSNAME##_AppRegisterMalloc {\
    public:\
        CLASSNAME##_AppRegisterMalloc() {\
            std::function<Application* ()> _fun = [] { return new CLASSNAME(); };\
            ApplicationManager::registerApplication(_fun, #CLASSNAME);\
        }\
};\
CLASSNAME##_AppRegisterMalloc _registerMalloc##CLASSNAME = CLASSNAME##_AppRegisterMalloc();\

#define APPLICATION_OBJECT(CLASSNAME) \
    ((CLASSNAME*)ApplicationManager::application(#CLASSNAME))\

#endif