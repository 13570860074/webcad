#ifndef DBPOLYLINE_H
#define DBPOLYLINE_H


#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"

class GeLineSeg2d;
class GeLineSeg3d;
class GeCircArc2d;
class GeCircArc3d;
class DbPolyline : public DbCurve
{
public:
    DbPolyline();
    virtual ~DbPolyline();
    DB_ENTITY_DECLARE_MEMBERS(DbPolyline);
    DB_CURVE_DECLARE_OVERRIDE_METHOD;
    DBCURVE_METHODS;

    //**********************************************************************
    // DbPolyline methods.
    //**********************************************************************

    Acad::ErrorStatus getPointAt(unsigned int, GePoint3d& pt) const;

    enum SegType {
        kLine,
        kArc,
        kCoincident,
        kPoint,
        kEmpty
    };

    SegType           segType(unsigned int index) const;

    Acad::ErrorStatus getLineSegAt(unsigned int index, GeLineSeg2d& ln) const;
    
    Acad::ErrorStatus getLineSegAt(unsigned int index, GeLineSeg3d& ln) const;
    Acad::ErrorStatus getArcSegAt(unsigned int index, GeCircArc2d& arc) const;
    
    Acad::ErrorStatus getArcSegAt(unsigned int index, GeCircArc3d& arc) const;

    //**********************************************************************
    // DbPolyline methods.
    //**********************************************************************

    virtual void      setClosed(Adesk::Boolean);
    virtual void      setElevation(double);
    virtual Acad::ErrorStatus setThickness(double);
    virtual Acad::ErrorStatus setConstantWidth(double);
    virtual Acad::ErrorStatus setNormal(const GeVector3d&);

    Adesk::Boolean    isOnlyLines()                        const;
    double            elevation()                        const;
    double            thickness()                        const;
    Acad::ErrorStatus getConstantWidth(double&)                 const;
    GeVector3d      normal()                        const;

    virtual Acad::ErrorStatus addVertexAt(unsigned int index,
        const GePoint2d& pt,
        double bulge = 0.,
        double startWidth = -1.,
        double endWidth = -1.,
        Adesk::Int32 vertexIdentifier = 0);

    virtual Acad::ErrorStatus removeVertexAt(unsigned int index);

    
    unsigned int      numVerts()                        const;

    Acad::ErrorStatus getPointAt(unsigned int index, GePoint2d& pt)         const;
    Acad::ErrorStatus getBulgeAt(unsigned int index, double& bulge)      const;
    Acad::ErrorStatus getVertexIdentifierAt(unsigned int index, Adesk::Int32& vertexIdentifier) const;
    Acad::ErrorStatus getWidthsAt(unsigned int index, double& startWidth, double& endWidth)   const;

    virtual Acad::ErrorStatus setPointAt(unsigned int index, const GePoint2d& pt);
    virtual Acad::ErrorStatus setBulgeAt(unsigned int index, double       bulge);
    virtual Acad::ErrorStatus setVertexIdentifierAt(unsigned int index, Adesk::Int32 suggestedValue);
    virtual Acad::ErrorStatus setWidthsAt(unsigned int index, double       startWidth, double       endWidth);



    //**********************************************************************
    // DbCurve methods.
    //**********************************************************************

    //DBCURVE_METHODS

    //*************************************************************************
    // Overridden methods from AcDEntity
    //*************************************************************************

    void getEcs(GeMatrix3d& retVal) const override;
};



#endif
