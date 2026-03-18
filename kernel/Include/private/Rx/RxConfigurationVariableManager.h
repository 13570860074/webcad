#ifndef RXCONFIGURATIONVARIABLEMANAGER_H
#define RXCONFIGURATIONVARIABLEMANAGER_H

#include "RxManager.h"
#include "GePoint3d.h"
#include "adsdef.h"
#include "AcString.h"
#include "RxConfigurationVariable.h"

class DbDatabase;
class RxConfigurationVariableManager : public RxObject, public RxManager
{

public:
    RxConfigurationVariableManager();
    virtual ~RxConfigurationVariableManager();
    RX_DECLARE_MEMBERS(RxConfigurationVariableManager);
    RX_MANAGER_DECLARE_MEMBERS(RxConfigurationVariableManager);


public:
    virtual void onInit();

    /** 添加变量 */
    virtual bool append(const AcString &_name, RxConfigurationVariable*& variable);

    /** 查询变量 */
    virtual RxConfigurationVariable *get(const AcString &_name);

    /** 删除变量 */
    virtual void remove(const AcString &_name);
};

#endif