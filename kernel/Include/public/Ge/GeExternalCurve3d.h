#ifndef GEEXTERNALCURVE3D_H
#define GEEXTERNALCURVE3D_H

#include "GeCurve3d.h"
#include "GeEntity3dMethods.h"
#include "GeMatrix3d.h"

class GeLine3d;
class GeLineSeg3d;
class GeRay3d;
class GeCircArc3d;
class GeEllipArc3d;
class GeNurbCurve3d;

class GeExternalCurve3d : public GeCurve3d
{
public:
    GeExternalCurve3d();
    GeExternalCurve3d(const GeExternalCurve3d& source);
    GeExternalCurve3d(void* pCurveDef, Ge::ExternalEntityKind curveKind, bool makeCopy = true);
    ~GeExternalCurve3d();

    GEENTITY3D_METHODS(GeExternalCurve3d);

    bool isLine() const;
    bool isRay() const;
    bool isLineSeg() const;
    bool isCircArc() const;
    bool isEllipArc() const;
    bool isNurbCurve() const;

    bool isDefined() const;
    bool isNativeCurve(GeCurve3d*& nativeCurve) const;

    void getExternalCurve(void*& pCurveDef) const;
    Ge::ExternalEntityKind externalCurveKind() const;
    GeExternalCurve3d& set(void* pCurveDef, Ge::ExternalEntityKind curveKind, bool makeCopy = true);

    GeExternalCurve3d& operator = (const GeExternalCurve3d& extCurve);

    bool isOwnerOfCurve() const;
    GeExternalCurve3d& setToOwnCurve();

private:
    void clearOwnedCurve();

private:
    void* m_pCurveDef;
    GeCurve3d* m_pCurve;
    bool m_isOwner;
    Ge::ExternalEntityKind m_curveKind;
};

#endif