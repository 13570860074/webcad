#ifndef GILINEDASHEDMATERIAL_H
#define GILINEDASHEDMATERIAL_H


#include "Gi.h"
#include "GiObject.h"
#include "GiMaterial.h"

class GiLineDashedMaterial : public GiMaterial {
public:
    GiLineDashedMaterial();
    virtual ~GiLineDashedMaterial();
    RX_DECLARE_MEMBERS(GiLineDashedMaterial);

public:
    virtual Gi::MaterialType type() const;

    double dashSize() const;
    void setDashSize(const double val);

    double gapSize() const;
    void setGapSize(const double val);

    double lineWidth() const;
    void setLineWidth(const double _lineWidth);
};



#endif