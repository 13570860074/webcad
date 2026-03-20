#ifndef GESEGMENTCHAIN2D_H
#define GESEGMENTCHAIN2D_H

#include "GePolyline2d.h"

class GeSegmentChain2d : public GePolyline2d
{
public:
    GeSegmentChain2d();
    GeSegmentChain2d(const GeSegmentChain2d& source);
    GeSegmentChain2d(const GePoint2dArray& fitpoints);
    GeSegmentChain2d(const GeKnotVector& knots, const GePoint2dArray& points);
    GeSegmentChain2d(const GeCurve2d& crv, double approxEps);

    GEENTITY2D_METHODS(GeSegmentChain2d);

    GeSegmentChain2d& operator = (const GeSegmentChain2d& pline);

    GeSegmentChain2d& setClosed(bool flag);

    GeDoubleArray& bulges();
    const GeDoubleArray& getBulges() const;

    GePoint2dArray& vertices();
    const GePoint2dArray& getVertices() const;

    bool hasBulges() const;

    void appendSamplePointsOptimal(int numSampleForEachArc, GePoint2dArray& pointArray) const;

private:
    void syncPolyline();

private:
    GePoint2dArray m_vertices;
    GeDoubleArray m_bulges;
    bool m_closed;
};

#endif