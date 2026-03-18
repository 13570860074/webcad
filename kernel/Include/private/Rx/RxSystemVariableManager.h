#ifndef RXSYSTEMVARIABLEMANAGER_H
#define RXSYSTEMVARIABLEMANAGER_H

#include "RxManager.h"
#include "GePoint3d.h"
#include "adsdef.h"
#include "AcString.h"
#include "RxSystemVariable.h"

class DbDatabase;
class RxSystemVariableManager : public RxObject, public RxManager
{

public:
    RxSystemVariableManager();
    virtual ~RxSystemVariableManager();
    RX_DECLARE_MEMBERS(RxSystemVariableManager);
    RX_MANAGER_DECLARE_MEMBERS(RxSystemVariableManager);


private:

    static void get_viewHeight(resbuf*& val);
    static Acad::ErrorStatus set_viewHeight(const resbuf* val);

    static void get_ctab(resbuf*& val);
    static Acad::ErrorStatus set_ctab(const resbuf* val);

    static void get_tilemode(resbuf*& val);
    static Acad::ErrorStatus set_tilemode(const resbuf* val);

    static void get_viewctr(resbuf*& val);
    static Acad::ErrorStatus set_viewctr(const resbuf* val);

    static void get_plineWid(resbuf*& val);
    static Acad::ErrorStatus set_plineWid(const resbuf* val);

    static void get_lastPoint(resbuf*& val);
    static Acad::ErrorStatus set_lastPoint(const resbuf* val);
    

public:
    virtual void onInit();

    // 查询环境变量
    virtual RxSystemVariable *get(const AcString &_name);
};

#endif