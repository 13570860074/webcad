#ifndef DBEXTENTS_H
#define DBEXTENTS_H

#include "Db.h"
#include "GePoint3d.h"
#include "GeVector3d.h"
#include "GeMatrix3d.h"

class DbExtents
{
public:
    DbExtents();
    DbExtents(const DbExtents& src);
    DbExtents(const GePoint3d& min, const GePoint3d& max);

    GePoint3d minPoint() const { return mMinPoint; }
    GePoint3d maxPoint() const { return mMaxPoint; }
    Acad::ErrorStatus set(const GePoint3d& min, const GePoint3d& max);

    void addPoint(const GePoint3d& pt);
    void addExt(const DbExtents& src);

    void expandBy(const GeVector3d& vec);
    void transformBy(const GeMatrix3d& mat);

    bool operator==(const DbExtents& other);

private:
    GePoint3d mMinPoint;
    GePoint3d mMaxPoint;
};


#endif