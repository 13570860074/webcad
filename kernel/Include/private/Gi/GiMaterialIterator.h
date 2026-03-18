#ifndef GIMATERIALITERATOR_H
#define GIMATERIALITERATOR_H


#include "GiMaterial.h"


class GiMaterialIterator :public RxObject
{
public:
    GiMaterialIterator();
    virtual ~GiMaterialIterator();
    RX_DECLARE_MEMBERS(GiMaterialIterator);

    void start();
    bool done();
    void step();
    GiMaterial *getMaterial();
};


#endif