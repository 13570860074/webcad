#ifndef GIENTITYITERATOR_H
#define GIENTITYITERATOR_H


#include "GiEntity.h"


class GiEntityIterator :public RxObject
{
public:
    GiEntityIterator();
    virtual ~GiEntityIterator();
    RX_DECLARE_MEMBERS(GiEntityIterator);

    void start();
    bool done();
    void step();
    GiEntity *getEntity();
};


#endif