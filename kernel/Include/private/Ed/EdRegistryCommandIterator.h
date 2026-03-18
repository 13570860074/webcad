#ifndef EDREGISTRYCOMMANDITERATOR_H
#define EDREGISTRYCOMMANDITERATOR_H

#include "adesk.h"
#include "EdCommandIterator.h"


class EdRegistryCommandIterator : public EdCommandIterator
{
public:
    EdRegistryCommandIterator();
    virtual ~EdRegistryCommandIterator();
    RX_DECLARE_MEMBERS(EdRegistryCommandIterator);

    virtual bool               done() const;
    virtual bool               next();
    virtual const EdCommand* command() const;
    virtual const ACHAR* commandGroup() const;
};

#endif