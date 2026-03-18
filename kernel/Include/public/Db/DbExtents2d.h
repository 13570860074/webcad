#ifndef DBEXTENTS2D_H
#define DBEXTENTS2D_H

#include "Db.h"
#include "GePoint2d.h"
#include "GeVector2d.h"
#include "GeMatrix2d.h"

class DbExtents2d
{
public:
    DbExtents2d();
    DbExtents2d(const DbExtents2d& src);
    DbExtents2d(const GePoint2d& min, const GePoint2d& max);

    GePoint2d minPoint() const { return mMinPoint; }
    GePoint2d maxPoint() const { return mMaxPoint; }
    Acad::ErrorStatus set(const GePoint2d& min, const GePoint2d& max);

    void addPoint(const GePoint2d& pt);
    void addExt(const DbExtents2d& src);

    void expandBy(const GeVector2d& vec);
    void transformBy(const GeMatrix2d& mat);

    bool operator==(const DbExtents2d& other);

private:
    GePoint2d mMinPoint;
    GePoint2d mMaxPoint;
};


#endif