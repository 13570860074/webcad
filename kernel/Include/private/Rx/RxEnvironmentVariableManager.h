#ifndef RXENVIRONMENTVARIABLEMANAGER_H
#define RXENVIRONMENTVARIABLEMANAGER_H

#include "RxManager.h"
#include "GePoint3d.h"
#include "adsdef.h"
#include "AcString.h"
#include "RxEnvironmentVariable.h"

class DbDatabase;
class RxEnvironmentVariableManager : public RxObject, public RxManager
{

public:
    RxEnvironmentVariableManager();
    virtual ~RxEnvironmentVariableManager();
    RX_DECLARE_MEMBERS(RxEnvironmentVariableManager);
    RX_MANAGER_DECLARE_MEMBERS(RxEnvironmentVariableManager);


public:
    virtual void onInit();

    /** 添加环境变量 */
    virtual bool append(const AcString &_name, RxEnvironmentVariable*& variable);

    /** 查询环境变量 */
    virtual RxEnvironmentVariable *get(const AcString &_name);

    /** 删除环境变量 */
    virtual void remove(const AcString &_name);
};

#endif