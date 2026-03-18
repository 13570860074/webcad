#ifndef GIMESHMATERIAL_H
#define GIMESHMATERIAL_H


#include "Gi.h"
#include "GiObject.h"
#include "GiMaterial.h"

class GiMeshMaterial : public GiMaterial {
public:
    GiMeshMaterial();
    virtual ~GiMeshMaterial();
    RX_DECLARE_MEMBERS(GiMeshMaterial);

public:
    virtual Gi::MaterialType type() const;
};



#endif