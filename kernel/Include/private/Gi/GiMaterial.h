#ifndef GIMATERIAL_H
#define GIMATERIAL_H


#include "Gi.h"
#include "GiObject.h"

class GiMaterial : public GiObject {
public:
    virtual ~GiMaterial();

public:
    RX_DECLARE_MEMBERS(GiMaterial);

public:
    virtual Gi::MaterialType type() const;
    unsigned int color() const;
    unsigned char transparency() const;
    unsigned char colorIndex() const;

    void setColor(const unsigned int _color);
    void setTransparency(unsigned char _transparency);
    void setColorIndex(unsigned char _colorIndex);
};



#endif