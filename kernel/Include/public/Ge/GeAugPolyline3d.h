#ifndef GEAUGPOLYLINE3D_H
#define GEAUGPOLYLINE3D_H

#include "GePolyline3d.h"
#include "GeVector3dArray.h"

class GeAugPolyline3d : public GePolyline3d
{
public:
    GeAugPolyline3d();
    GeAugPolyline3d(const GeAugPolyline3d& apline);
    GeAugPolyline3d(const GeKnotVector& knots, const GePoint3dArray& controlPoints, const GeVector3dArray& vecBundle);
    GeAugPolyline3d(const GePoint3dArray& controlPoints, const GeVector3dArray& vecBundle);
    GeAugPolyline3d(const GeCurve3d& curve, double fromParam, double toParam, double approxEps);

    GEENTITY3D_METHODS(GeAugPolyline3d);

    GeAugPolyline3d& operator = (const GeAugPolyline3d& apline);

    GePoint3d getPoint(int index) const;
    GeAugPolyline3d& setPoint(int controlpointIndex, const GePoint3d& point);
    void getPoints(GePoint3dArray& controlPoints) const;

    GeVector3d getVector(int vectorIndex) const;
    GeAugPolyline3d& setVector(int vectorIndex, const GeVector3d& vect);
    void getD1Vectors(GeVector3dArray& tangents) const;

    GeVector3d getD2Vector(int vectorIndex) const;
    GeAugPolyline3d& setD2Vector(int vectorIndex, const GeVector3d& vect);
    void getD2Vectors(GeVector3dArray& d2Vectors) const;

    double approxTol() const;
    GeAugPolyline3d& setApproxTol(double approxTol);

private:
    GeVector3dArray m_vecBundle;
    GeVector3dArray m_d2Vectors;
    double m_approxTol;
};

#endif