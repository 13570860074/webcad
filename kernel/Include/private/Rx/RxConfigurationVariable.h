#ifndef RXCONFIGURATIONVARIABLE_H
#define RXCONFIGURATIONVARIABLE_H

#include "AcString.h"
#include "adsdef.h"
#include "acadstrc.h"

class RxConfigurationVariable
{

public:
    RxConfigurationVariable();
    virtual ~RxConfigurationVariable();

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