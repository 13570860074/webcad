#ifndef GILINEPIXELDASHEDMATERIAL_H
#define GILINEPIXELDASHEDMATERIAL_H


#include "Gi.h"
#include "GiObject.h"
#include "GiMaterial.h"

class GiLinePixelDashedMaterial : public GiMaterial {
public:
    GiLinePixelDashedMaterial();
    virtual ~GiLinePixelDashedMaterial();
    RX_DECLARE_MEMBERS(GiLinePixelDashedMaterial);

public:
    virtual Gi::MaterialType type() const;

    double pixelRatio() const;
    void setPixelRatio(const double val);

    double dashSize() const;
    void setDashSize(const double val);

    double gapSize() const;
    void setGapSize(const double val);

    double lineWidth() const;
    void setLineWidth(const double _lineWidth);
};



#endif