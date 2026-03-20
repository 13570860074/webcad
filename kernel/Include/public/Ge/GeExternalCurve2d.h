#ifndef GEEXTERNALCURVE2D_H
#define GEEXTERNALCURVE2D_H

#include "GeCurve2d.h"
#include "GeEntity2dMethods.h"
#include "GeMatrix2d.h"

class GeNurbCurve2d;

class GeExternalCurve2d : public GeCurve2d
{
public:
    GeExternalCurve2d();
    GeExternalCurve2d(const GeExternalCurve2d& source);
    GeExternalCurve2d(void* pCurveDef, Ge::ExternalEntityKind curveKind, bool makeCopy = true);
    ~GeExternalCurve2d();

    GEENTITY2D_METHODS(GeExternalCurve2d);

    bool isNurbCurve() const;
    bool isNurbCurve(GeNurbCurve2d& nurbCurve) const;

    bool isDefined() const;
    void getExternalCurve(void*& pCurveDef) const;
    Ge::ExternalEntityKind externalCurveKind() const;

    GeExternalCurve2d& set(void* pCurveDef, Ge::ExternalEntityKind curveKind, bool makeCopy = true);

    bool isOwnerOfCurve() const;
    GeExternalCurve2d& setToOwnCurve();

    GeExternalCurve2d& operator = (const GeExternalCurve2d& extCurve);

private:
    void clearOwnedCurve();

private:
    void* m_pCurveDef;
    GeCurve2d* m_pCurve;
    bool m_isOwner;
    Ge::ExternalEntityKind m_curveKind;
};

#endif