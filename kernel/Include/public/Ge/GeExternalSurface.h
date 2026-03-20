#ifndef GEEXTERNALSURFACE_H
#define GEEXTERNALSURFACE_H

#include "GeEntity3dMethods.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GePlane;
class GeSphere;

class GeExternalSurface : public GeSurface
{
public:
    GeExternalSurface();
    GeExternalSurface(void* pSurfaceDef, Ge::ExternalEntityKind surfaceKind, bool makeCopy = true);
    GeExternalSurface(const GeExternalSurface& source);
    ~GeExternalSurface();

    GEENTITY3D_METHODS(GeExternalSurface);

    void getExternalSurface(void*& pSurfaceDef) const;
    Ge::ExternalEntityKind externalSurfaceKind() const;

    bool isPlane() const;
    bool isSphere() const;
    bool isCylinder() const;
    bool isCone() const;
    bool isEllipCylinder() const;
    bool isEllipCone() const;
    bool isTorus() const;
    bool isNurbSurface() const;

    bool isDefined() const;
    bool isNativeSurface(GeSurface*& nativeSurface) const;

    GeExternalSurface& operator = (const GeExternalSurface& extSurf);
    GeExternalSurface& set(void* pSurfaceDef, Ge::ExternalEntityKind surfaceKind, bool makeCopy = true);

    bool isOwnerOfSurface() const;
    GeExternalSurface& setToOwnSurface();

private:
    void clearOwnedSurface();

private:
    void* m_pSurfaceDef;
    GeSurface* m_pSurface;
    bool m_isOwner;
    Ge::ExternalEntityKind m_surfaceKind;
};

#endif