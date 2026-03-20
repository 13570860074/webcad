#ifndef GECURVEBOUNDARY_H
#define GECURVEBOUNDARY_H

#include "GeExport.h"

class GeCurve2d;
class GeEntity3d;
class GePosition3d;

class GE_EXPORT GeCurveBoundary
{
public:
    GeCurveBoundary();
    GeCurveBoundary(
        int numCurves,
        const GeEntity3d* const* crv3d,
        const GeCurve2d* const* crv2d,
        bool* orientation3d,
        bool* orientation2d,
        bool makeCopy = true);
    GeCurveBoundary(const GeCurveBoundary& source);
    ~GeCurveBoundary();

    GeCurveBoundary& operator = (const GeCurveBoundary& crvBoundary);

    bool isDegenerate() const;
    bool isDegenerate(GePosition3d& degenPoint, GeCurve2d** paramCurve) const;

    int numElements() const;

    void getContour(
        int& numCurves,
        GeEntity3d*** crv3d,
        GeCurve2d*** crv2d,
        bool** orientation3d,
        bool** orientation2d) const;

    GeCurveBoundary& set(
        int numCurves,
        const GeEntity3d* const* crv3d,
        const GeCurve2d* const* crv2d,
        bool* orientation3d,
        bool* orientation2d,
        bool makeCopy = true);

    bool isOwnerOfCurves() const;
    GeCurveBoundary& setToOwnCurves();

protected:
    int m_iNumCurves;
    GeEntity3d** m_pArrCrv3d;
    GeCurve2d** m_pArrCrv2d;
    bool* m_bArrOrientation3d;
    bool* m_bArrOrientation2d;
    bool m_bMakeCopy;

private:
    void clearData();
    void allocateArrays(int numCurves, bool has3d, bool has2d);
};

#endif