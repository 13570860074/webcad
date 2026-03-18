#ifndef GIPOINTMATERIAL_H
#define GIPOINTMATERIAL_H


#include "Gi.h"
#include "GiObject.h"
#include "GiMaterial.h"

class GiPointMaterial : public GiMaterial {
public:
    GiPointMaterial();
    virtual ~GiPointMaterial();
    RX_DECLARE_MEMBERS(GiPointMaterial);

public:
    virtual Gi::MaterialType type() const;

    unsigned char size()const;
    void setSize(unsigned char _size);
};



#endif