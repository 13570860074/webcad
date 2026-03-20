#ifndef GEEXTENTS3D_H
#define GEEXTENTS3D_H

#include "GePoint3d.h"
#include "GePoint3dArray.h"
#include "GeVector3d.h"
#include "GeMatrix3d.h"

class GeExtents3d
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

    GeExtents3d();
    GeExtents3d(const GePoint3d& min, const GePoint3d& max);

    GE_STATIC_EXPORT static const GeExtents3d kInvalid;

    const GePoint3d& minPoint() const;
    const GePoint3d& maxPoint() const;
    const GePoint3d& operator[] (int index) const;
    GePoint3d& operator[] (int index);

    void set(const GePoint3d& min, const GePoint3d& max);
    void comparingSet(const GePoint3d& pt1, const GePoint3d& pt2);

    GeExtents3d& addPoint(const GePoint3d& point);
    GeExtents3d& addPoints(const GePoint3dArray& points);
    GeExtents3d& addExt(const GeExtents3d& extents);

    bool isValidExtents() const;
    void expandBy(const GeVector3d& vect);
    void transformBy(const GeMatrix3d& xfm);

    bool contains(const GePoint3d& point, const GeTol& tol = GeContext::gTol) const;
    bool contains(const GeExtents3d& extents, const GeTol& tol = GeContext::gTol) const;
    bool isDisjoint(const GeExtents3d& extents, const GeTol& tol = GeContext::gTol) const;
    bool isDisjointEuclidean(const GeExtents3d& extents, const GeTol& tol = GeContext::gTol) const;

    double distanceTo(const GePoint3d& point) const;
    double distanceTo(const GeExtents3d& extents) const;

    IntersectionStatus intersectWith(const GeExtents3d& extents, GeExtents3d* pResult = NULL) const;

    GePoint3d center() const;
    GeVector3d diagonal() const;

    bool operator == (const GeExtents3d& other) const;
    bool operator != (const GeExtents3d& other) const;

private:
    GePoint3d m_min;
    GePoint3d m_max;
};

#endif