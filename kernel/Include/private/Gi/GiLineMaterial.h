#ifndef GILINEMATERIAL_H
#define GILINEMATERIAL_H


#include "Gi.h"
#include "GiObject.h"
#include "GiMaterial.h"

class GiLineMaterial : public GiMaterial {
public:
    GiLineMaterial();
    virtual ~GiLineMaterial();
    RX_DECLARE_MEMBERS(GiLineMaterial);


public:
    Gi::MaterialType type() const;

    double lineWidth() const;
    void setLineWidth(const double _lineWidth);
};



#endif