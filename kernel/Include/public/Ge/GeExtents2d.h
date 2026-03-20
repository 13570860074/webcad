#ifndef GEEXTENTS2D_H
#define GEEXTENTS2D_H

#include "GePoint2d.h"
#include "GePoint2dArray.h"
#include "GeVector2d.h"
#include "GeMatrix2d.h"

class GeExtents2d
{
public:
    enum IntersectionStatus
    {
        kIntersectUnknown,
        kIntersectNot,
        kIntersectOpIn,
        kIntersectOpOut,
        kIntersectOk
    };

    GeExtents2d();
    GeExtents2d(const GePoint2d& min, const GePoint2d& max);

    GE_STATIC_EXPORT static const GeExtents2d kInvalid;

    const GePoint2d& minPoint() const;
    const GePoint2d& maxPoint() const;
    GeVector2d diagonal() const;

    void set(const GePoint2d& min, const GePoint2d& max);
    void comparingSet(const GePoint2d& pt1, const GePoint2d& pt2);

    GeExtents2d& addPoint(const GePoint2d& point);
    GeExtents2d& addPoints(const GePoint2dArray& points);
    GeExtents2d& addExt(const GeExtents2d& extents);

    bool isValidExtents() const;
    void expandBy(const GeVector2d& vect);
    void transformBy(const GeMatrix2d& xfm);
    void translate(const GeVector2d& shift);

    bool contains(const GePoint2d& point, const GeTol& tol = GeContext::gTol) const;
    bool contains(const GeExtents2d& extents, const GeTol& tol = GeContext::gTol) const;
    bool isDisjoint(const GeExtents2d& extents, const GeTol& tol = GeContext::gTol) const;

    IntersectionStatus intersectWith(const GeExtents2d& extents, GeExtents2d* pResult = NULL) const;
    GePoint2d center() const;

    bool operator == (const GeExtents2d& other) const;
    bool operator != (const GeExtents2d& other) const;

private:
    GePoint2d m_min;
    GePoint2d m_max;
};

#endif