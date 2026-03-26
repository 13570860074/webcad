#ifndef DBLEADER_H
#define DBLEADER_H


#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"
#include "GePoint3dArray.h"


class DbLeader : public DbCurve
{
public:
    DbLeader();
    virtual ~DbLeader();
    DB_ENTITY_DECLARE_MEMBERS(DbLeader);
    DB_CURVE_DECLARE_OVERRIDE_METHOD;
    DBCURVE_METHODS

    enum AnnoType
    {
        kMText    = 0,
        kFcf      = 1,
        kBlockRef = 2,
        kNoAnno   = 3
    };

    GeVector3d normal() const;

    int numVertices() const;
    bool appendVertex(const GePoint3d& vertex);
    void removeLastVertex();
    GePoint3d firstVertex() const;
    GePoint3d lastVertex() const;
    GePoint3d vertexAt(int index) const;
    bool setVertexAt(int index, const GePoint3d& vertex);

    bool hasArrowHead() const;
    void enableArrowHead();
    void disableArrowHead();

    bool isSplined() const;
    void setToSplineLeader();
    void setToStraightLeader();

    DbObjectId dimensionStyle() const;
    Acad::ErrorStatus setDimensionStyle(const DbObjectId& dimStyleId);

    DbObjectId annotationObjId() const;
    Acad::ErrorStatus attachAnnotation(const DbObjectId& annoId);
    Acad::ErrorStatus detachAnnotation();
    GeVector3d annotationOffset() const;
    Acad::ErrorStatus setAnnotationOffset(const GeVector3d& offset);

    AnnoType annoType() const;
};



#endif
