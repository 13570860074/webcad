#ifndef RXENVIRONMENTVARIABLE_H
#define RXENVIRONMENTVARIABLE_H

#include "AcString.h"
#include "adsdef.h"
#include "acadstrc.h"

class RxEnvironmentVariable
{

public:
    RxEnvironmentVariable();
    virtual ~RxEnvironmentVariable();

private:
    AcString m_name;
    AcString m_variable;

public:
    const AcString &name() const;
    void setName(const AcString &_name);

    const AcString & variable() const;
    void setVariable(const AcString &_val);
};

#endif