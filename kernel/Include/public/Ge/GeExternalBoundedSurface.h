#ifndef GEEXTERNALBOUNDEDSURFACE_H
#define GEEXTERNALBOUNDEDSURFACE_H

#include "GeCurveBoundary.h"
#include "GeEntity3dMethods.h"
#include "GeExternalSurface.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GeExternalBoundedSurface : public GeSurface
{
public:
    GeExternalBoundedSurface();
    GeExternalBoundedSurface(void* pSurfaceDef, Ge::ExternalEntityKind surfaceKind, bool makeCopy = true);
    GeExternalBoundedSurface(const GeExternalBoundedSurface& source);
    ~GeExternalBoundedSurface();

    GEENTITY3D_METHODS(GeExternalBoundedSurface);

    Ge::ExternalEntityKind externalSurfaceKind() const;
    bool isDefined() const;
    void getExternalSurface(void*& pSurfaceDef) const;

    void getBaseSurface(GeSurface*& pSurfaceDef) const;
    void getBaseSurface(GeExternalSurface& unboundedSurfaceDef) const;

    bool isPlane() const;
    bool isSphere() const;
    bool isCylinder() const;
    bool isCone() const;
    bool isEllipCylinder() const;
    bool isEllipCone() const;
    bool isTorus() const;
    bool isNurbs() const;
    bool isExternalSurface() const;

    int numContours() const;
    void getContours(int& numContours, GeCurveBoundary*& contours) const;

    GeExternalBoundedSurface& set(void* pSurfaceDef, Ge::ExternalEntityKind surfaceKind, bool makeCopy = true);

    bool isOwnerOfSurface() const;
    GeExternalBoundedSurface& setToOwnSurface();

    GeExternalBoundedSurface& setContours(int numContours, const GeCurveBoundary* contours, bool makeCopy = true);

    GeExternalBoundedSurface& operator = (const GeExternalBoundedSurface& extBoundSurf);

private:
    void clearContours();

private:
    GeExternalSurface m_baseSurface;
    GeCurveBoundary* m_pContours;
    int m_numContours;
    bool m_contoursOwned;
};

#endif