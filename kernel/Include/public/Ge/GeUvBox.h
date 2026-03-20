#ifndef GEUVBOX_H
#define GEUVBOX_H

#include "GeInterval.h"
#include "GePoint2d.h"

class GeExtents2d;

struct GeUvBox
{
    GeInterval intervals[2];

    GeUvBox();
    GeUvBox(const GeInterval& intervalU, const GeInterval& intervalV);

    const GeInterval& u() const;
    const GeInterval& v() const;
    GeInterval& u();
    GeInterval& v();

    const GeInterval& operator[] (int idx) const;
    GeInterval& operator[] (int idx);

    GeUvBox& set();
    GeUvBox& set(const GeInterval& intervalU, const GeInterval& intervalV);
    GeUvBox& set(const GePoint2d& lowerBound, const GePoint2d& upperBound);

    bool contains(double uparam, double vparam) const;
    bool contains(const GePoint2d& uvpoint) const;

    bool isBounded() const;
    bool finiteIntersectWith(const GeUvBox& range, GeUvBox& result) const;

    GePoint2d lowerBound() const;
    GePoint2d upperBound() const;

    bool isEqualTo(const GeUvBox& uvbox) const;
    operator GeExtents2d() const;

    GePoint2d eval(double ratioU, double ratioV) const;
    GePoint2d clamp(const GePoint2d& point) const;

    bool operator == (const GeUvBox& otherUvBox) const;
};

#endif