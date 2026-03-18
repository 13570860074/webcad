#ifndef RXSYSTEMVARIABLE_H
#define RXSYSTEMVARIABLE_H

#include "AcString.h"
#include "adsdef.h"
#include "acadstrc.h"

class RxSystemVariable
{

public:
    RxSystemVariable();
    virtual ~RxSystemVariable();

private:
    AcString m_name;
    resbuf* m_variable;
    void *m_onGet;
    void *m_onSet;

public:

    const AcString& name();
    void setName(const AcString& _name);

    Acad::ErrorStatus variable(resbuf*& val);
    Acad::ErrorStatus setVariable(const resbuf* _val);

    // 设置获取回调函数
    void on_get(void(*fun)(resbuf*&));

    // 设置设置回调函数
    void on_set(Acad::ErrorStatus (*fun)(const resbuf*));
};

#endif