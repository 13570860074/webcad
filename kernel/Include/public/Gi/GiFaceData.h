#ifndef GIFACEDATA_H
#define GIFACEDATA_H

#include "GiParameter.h"
#include "adesk.h"
#include "GeVector3d.h"

class GiMapper;
class DbObjectId;
class CmEntityColor;
class CmTransparency;
class GiFaceData :public GiParameter
{
public:
	GiFaceData();
    virtual ~GiFaceData();

    virtual void            setColors(const short* colors);
    virtual void            setTrueColors(const CmEntityColor* colors);
    virtual void            setLayers(const DbObjectId* layers);
    virtual void            setSelectionMarkers(const Adesk::LongPtr* pSelectionMarkers);
    virtual void            setNormals(const GeVector3d* pNormals);
    virtual void            setVisibility(const Adesk::UInt8* vis);
    virtual void            setMaterials(const DbObjectId* materials);
    virtual void            setMappers(const GiMapper* mappers);
    virtual void            setTransparency(const CmTransparency* transparency);

    virtual short* colors() const;
    virtual CmEntityColor* trueColors() const;
    virtual DbObjectId* layerIds() const;
    virtual Adesk::LongPtr* selectionMarkers() const;
    virtual GeVector3d* normals() const;
    virtual Adesk::UInt8* visibility() const;
    virtual DbObjectId* materials() const;
    virtual GiMapper* mappers() const;
    virtual CmTransparency* transparency() const;
};



#endif