#ifndef EDCOMMANDITERATOR_H
#define EDCOMMANDITERATOR_H

#include "adesk.h"
#include "EdCommand.h"


class EdCommandIterator : public RxObject
{
public:
    RX_DECLARE_MEMBERS_VIRTUAL(EdCommandIterator);

    virtual bool               done() const = 0;
    virtual bool               next() = 0;
    virtual const EdCommand* command() const = 0;
    virtual const ACHAR* commandGroup() const = 0;
};

#endif