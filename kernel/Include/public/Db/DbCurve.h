//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2019 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
//
// DESCRIPTION: API Abstract class for "Curve" entities.

#ifndef DBCURVE_H
#define DBCURVE_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbEntity.h"
#include "GeDoubleArray.h"
#include "GePoint3dArray.h"


class DbSpline;
class GeVector3d;
class GeCurve3d;



class DbCurve : public DbEntity
{
public:
    virtual ~DbCurve();
    DB_ENTITY_DECLARE_MEMBERS(DbCurve);
    

    // Curve property tests.
    //
    virtual bool    isClosed() const;
    virtual bool    isPeriodic() const;

    // Get planar and start/end geometric properties.
    //
    virtual Acad::ErrorStatus getStartParam(double&) const;
    virtual Acad::ErrorStatus getEndParam(double&) const;
    virtual Acad::ErrorStatus getStartPoint(GePoint3d&) const;
    virtual Acad::ErrorStatus getEndPoint(GePoint3d&) const;

    // Conversions to/from parametric/world/distance.
    //
    virtual Acad::ErrorStatus getPointAtParam(double, GePoint3d&) const;
    virtual Acad::ErrorStatus getParamAtPoint(const GePoint3d&, double&)const;
    virtual Acad::ErrorStatus getDistAtParam(double param, double& dist) const;
    virtual Acad::ErrorStatus getParamAtDist(double dist, double& param) const;
    virtual Acad::ErrorStatus getDistAtPoint(const GePoint3d&, double&)const;
    virtual Acad::ErrorStatus getPointAtDist(double, GePoint3d&) const;

    // Derivative information.
    //
    virtual Acad::ErrorStatus getFirstDeriv(double param, GeVector3d& firstDeriv) const;
    virtual Acad::ErrorStatus getFirstDeriv(const GePoint3d&, GeVector3d& firstDeriv) const;
    virtual Acad::ErrorStatus getSecondDeriv(double param, GeVector3d& secDeriv) const;
    virtual Acad::ErrorStatus getSecondDeriv(const GePoint3d&, GeVector3d& secDeriv) const;

    // Closest point on curve.
    //
    virtual Acad::ErrorStatus getClosestPointTo(const GePoint3d& givenPnt,GePoint3d& pointOnCurve,bool extend = false) const;
    virtual Acad::ErrorStatus getClosestPointTo(const GePoint3d& givenPnt, const GeVector3d& direction, GePoint3d& pointOnCurve, bool extend = false) const;

    // Get a projected copy of the curve.
    //
    virtual Acad::ErrorStatus getOrthoProjectedCurve(const GePlane&, DbCurve*& projCrv) const;
    virtual Acad::ErrorStatus getProjectedCurve(const GePlane&, const GeVector3d& projDir, DbCurve*& projCrv) const;

    // Get offset, spline and split copies of the curve.
    //
    virtual Acad::ErrorStatus getOffsetCurves(double offsetDist, DbVoidPtrArray& offsetCurves) const;
    virtual Acad::ErrorStatus getOffsetCurvesGivenPlaneNormal(
        const GeVector3d& normal, 
        double offsetDist,
        DbVoidPtrArray& offsetCurves) const;
    virtual Acad::ErrorStatus getSpline(DbSpline*& spline) const;
    virtual Acad::ErrorStatus getSplitCurves(const GeDoubleArray& params, DbVoidPtrArray& curveSegments) const;
    virtual Acad::ErrorStatus getSplitCurves(const GePoint3dArray& points, DbVoidPtrArray& curveSegments) const;

    // Extend the curve.
    //
    virtual Acad::ErrorStatus extend(double newParam);
    virtual Acad::ErrorStatus extend(bool extendStart, const GePoint3d& toPoint);

    // Area calculation.
    //
    virtual Acad::ErrorStatus getArea(double&) const;

    // Reverse the curve.
    //
    virtual Acad::ErrorStatus reverseCurve();

    /// <summary>
    /// This function returns an GeCurve3d that is geometrically identical to
    /// this DbCurve.
    /// </summary>
    /// <param name="pGeCurve">Output pointer to an GeCurve3d object that is
    /// geometrically identical to this DbCurve.  The caller of this function
    /// is responsible for deleting this GeCurve3d object.</param>
    /// <param name="tol">Optional input tolerance.</param>
    /// <returns>Returns eOk if successful.</returns>
    virtual Acad::ErrorStatus getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol = GeContext::gTol) const;

    /// <summary>
    /// This function takes an GeCurve3d and returns an DbCurve that is
    /// geometrically identical to the GeCurve3d.
    /// </summary>
    /// <param name="geCurve">Input reference to an GeCurve3d.</param>
    /// <param name="pDbCurve">Output pointer to an DbCurve object that is
    /// geometrically identical to geCurve.  The caller of this function is
    /// responsible for deleting this DbCurve object or adding it to the
    /// database.</param>
    /// <param name="normal">Optional normal vector.  If this parameter is 
    /// supplied then it must be a valid vector that is perpendicular to the
    /// input geCurve, and this vector will become the normal vector of
    /// the output pDbCurve.  If this parameter is not supplied, then this
    /// function will compute the normal vector itself.</param>
    /// <param name="tol">Optional input tolerance.</param>
    /// <returns>Returns eOk if successful.</returns>
    static Acad::ErrorStatus createFromGeCurve(const GeCurve3d& geCurve, DbCurve*& pDbCurve,
        GeVector3d* normal = NULL,
        const GeTol& tol = GeContext::gTol);

    /// <summary>
    /// This function takes an GeCurve3d and sets this DbCurve to be
    /// geometrically identical to the GeCurve3d.
    /// </summary>
    /// <param name="geCurve">Input reference to an GeCurve3d.</param>
    /// <param name="normal">Optional normal vector.  If this parameter is 
    /// supplied then it must be a valid vector that is perpendicular to the
    /// input geCurve, and this vector will become the normal vector of
    /// the output pDbCurve.  If this parameter is not supplied, then this
    /// function will compute the normal vector itself.</param>
    /// <param name="tol">Optional input tolerance.</param>
    /// <returns>Returns eOk if successful.</returns>
    virtual Acad::ErrorStatus setFromGeCurve(
        const GeCurve3d& geCurve,
        GeVector3d* normal = NULL,
        const GeTol& tol = GeContext::gTol);

protected:
    DbCurve();
};



// Protocols of DbCurve available in macro from.

#define DBCURVE_METHODS \
bool isClosed() const override; \
bool isPeriodic() const override; \
bool isPlanar() const override; \
Acad::ErrorStatus getPlane(GePlane&, Db::Planarity&) const override; \
Acad::ErrorStatus getStartParam(double&) const override; \
Acad::ErrorStatus getEndParam(double&) const override; \
Acad::ErrorStatus getStartPoint(GePoint3d&) const override; \
Acad::ErrorStatus getEndPoint(GePoint3d&) const override; \
Acad::ErrorStatus getPointAtParam(double, GePoint3d&) const override; \
Acad::ErrorStatus getParamAtPoint(const GePoint3d&, double&) const override; \
Acad::ErrorStatus getDistAtParam(double, double&) const override; \
Acad::ErrorStatus getParamAtDist(double, double&) const override; \
Acad::ErrorStatus getDistAtPoint(const GePoint3d&, double&) const override; \
Acad::ErrorStatus getPointAtDist(double, GePoint3d&) const override; \
Acad::ErrorStatus getFirstDeriv(double, GeVector3d&) const override; \
Acad::ErrorStatus getFirstDeriv(const GePoint3d&,GeVector3d&)const override; \
Acad::ErrorStatus getSecondDeriv(double, GeVector3d&) const override; \
Acad::ErrorStatus getSecondDeriv(const GePoint3d&, GeVector3d&) const override; \
Acad::ErrorStatus getClosestPointTo(const GePoint3d&, GePoint3d&, bool = false) const override; \
Acad::ErrorStatus getClosestPointTo(const GePoint3d&, const GeVector3d&, GePoint3d&,bool = false) const override; \
Acad::ErrorStatus getOrthoProjectedCurve(const GePlane&, DbCurve*&) const override; \
Acad::ErrorStatus getProjectedCurve(const GePlane&, const GeVector3d&, DbCurve*&) const override; \
Acad::ErrorStatus getOffsetCurves(double, DbVoidPtrArray&) const override; \
Acad::ErrorStatus getSpline(DbSpline*&) const override; \
Acad::ErrorStatus getSplitCurves(const GeDoubleArray&, DbVoidPtrArray& curveSegments) const override; \
Acad::ErrorStatus getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const override; \
Acad::ErrorStatus extend(double) override; \
Acad::ErrorStatus extend(bool, const GePoint3d&) override; \
Acad::ErrorStatus getArea(double&) const override; \
Acad::ErrorStatus reverseCurve() override; \
Acad::ErrorStatus getGeCurve (GeCurve3d*& pGeCurve,const GeTol& tol = GeContext::gTol) const override; \
Acad::ErrorStatus setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal = NULL, const GeTol& tol = GeContext::gTol) override;\



#endif
