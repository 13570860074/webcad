#include "GeQuaternion.h"
#include <cmath>

//=======================================================================
// function : IsEqual
// purpose  :
//=======================================================================

bool GeQuaternion::isEqual(const GeQuaternion &theOther) const
{
    if (this == &theOther)
    {
        return true;
    }
        return std::fabs(x - theOther.x) <= GeContext::gTol.equalPoint() &&
            std::fabs(y - theOther.y) <= GeContext::gTol.equalPoint() &&
            std::fabs(z - theOther.z) <= GeContext::gTol.equalPoint() &&
            std::fabs(w - theOther.w) <= GeContext::gTol.equalPoint();
}

//=======================================================================
// function : SetRotation
// purpose  :
//=======================================================================

void GeQuaternion::setRotation(const GeVector3d &theVecFrom, const GeVector3d &theVecTo)
{
    GeVector3d aVecCross(theVecFrom.crossProduct(theVecTo));
    set(aVecCross.x, aVecCross.y, aVecCross.z, theVecFrom.dotProduct(theVecTo));
    normalize();                           // if "from" or "to" not unit, normalize quat
    w += 1.0;                              // reducing angle to halfangle
    if (w <= GeContext::gTol.equalPoint()) // angle close to PI
    {
        if ((theVecFrom.z * theVecFrom.z) > (theVecFrom.x * theVecFrom.x))
        {
            set(0.0, theVecFrom.z, -theVecFrom.y, w); // theVecFrom * GeVector3d(1,0,0)
        }
        else
        {
            set(theVecFrom.y, -theVecFrom.x, 0.0, w); // theVecFrom * GeVector3d(0,0,1)
        }
    }
    normalize();
}

//=======================================================================
// function : SetRotation
// purpose  :
//=======================================================================

void GeQuaternion::setRotation(const GeVector3d &theVecFrom, const GeVector3d &theVecTo, const GeVector3d &theHelpCrossVec)
{
    GeVector3d aVecCross(theVecFrom.crossProduct(theVecTo));
    set(aVecCross.x, aVecCross.y, aVecCross.z, theVecFrom.dotProduct(theVecTo));
    normalize();                           // if "from" or "to" not unit, normalize quat
    w += 1.0;                              // reducing angle to halfangle
    if (w <= GeContext::gTol.equalPoint()) // angle close to PI
    {
        GeVector3d theAxis = theVecFrom.crossProduct(theHelpCrossVec);
        set(theAxis.x, theAxis.y, theAxis.z, w);
    }
    normalize();
}

//=======================================================================
// function : SetVectorAndAngle
// purpose  :
//=======================================================================

void GeQuaternion::setVectorAndAngle(const GeVector3d &theAxis, const double theAngle)
{
    GeVector3d anAxis = theAxis.normal();
    double anAngleHalf = 0.5 * theAngle;
    double sin_a = sin(anAngleHalf);
    set(anAxis.x * sin_a, anAxis.y * sin_a, anAxis.z * sin_a, cos(anAngleHalf));
}

//=======================================================================
// function : GetVectorAndAngle
// purpose  :
//=======================================================================
GeVector3d GeQuaternion::getVectorAndAngle() const
{
    GeVector3d theAxis;
    double theAngle;
    this->getVectorAndAngle(theAxis, theAngle);
    theAxis.normalize();
    theAxis *= theAngle;
    return theAxis;
}
void GeQuaternion::getVectorAndAngle(GeVector3d &theAxis, double &theAngle) const
{
    double vl = sqrt(x * x + y * y + z * z);
    if (vl > GeContext::gTol.equalPoint())
    {
        double ivl = 1.0 / vl;
        theAxis.set(x * ivl, y * ivl, z * ivl);
        if (w < 0.0)
        {
            theAngle = 2.0 * atan2(-vl, -w); // [-PI,  0]
        }
        else
        {
            theAngle = 2.0 * atan2(vl, w); // [  0, PI]
        }
    }
    else
    {
        theAxis.set(0.0, 0.0, 1.0);
        theAngle = 0.0;
    }
}

//=======================================================================
// function : SetMatrix
// purpose  :
//=======================================================================
// 矩阵转四元数
void GeQuaternion::setMatrix(const GeMatrix3d &theMat)
{
    double tr = theMat(0, 0) + theMat(1, 1) + theMat(2, 2); // trace of martix
    if (tr > 0.0)
    { // if trace positive than "w" is biggest component
        set(theMat(2, 1) - theMat(1, 2),
            theMat(0, 2) - theMat(2, 0),
            theMat(1, 0) - theMat(0, 1),
            tr + 1.0);
        scale(0.5 / sqrt(w)); // "w" contain the "norm * 4"
    }
    else if ((theMat(0, 0) > theMat(1, 1)) && (theMat(0, 0) > theMat(2, 2)))
    { // Some of vector components is bigger
        set(1.0 + theMat(0, 0) - theMat(1, 1) - theMat(2, 2),
            theMat(0, 1) + theMat(1, 0),
            theMat(0, 2) + theMat(2, 0),
            theMat(2, 1) - theMat(1, 2));
        scale(0.5 / sqrt(x));
    }
    else if (theMat(1, 1) > theMat(2, 2))
    {
        set(theMat(0, 1) + theMat(1, 0),
            1.0 + theMat(1, 1) - theMat(0, 0) - theMat(2, 2),
            theMat(1, 2) + theMat(2, 1),
            theMat(0, 2) - theMat(2, 0));
        scale(0.5 / sqrt(y));
    }
    else
    {
        set(theMat(0, 2) + theMat(2, 0),
            theMat(1, 2) + theMat(2, 1),
            1.0 + theMat(2, 2) - theMat(0, 0) - theMat(1, 1),
            theMat(1, 0) - theMat(0, 1));
        scale(0.5 / sqrt(z));
    }
}

//=======================================================================
// function : GetMatrix
// purpose  :
//=======================================================================

GeMatrix3d GeQuaternion::getMatrix() const
{
    double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    double s = 2.0 / squareNorm();
    x2 = x * s;
    y2 = y * s;
    z2 = z * s;
    xx = x * x2;
    xy = x * y2;
    xz = x * z2;
    yy = y * y2;
    yz = y * z2;
    zz = z * z2;
    wx = w * x2;
    wy = w * y2;
    wz = w * z2;

    GeMatrix3d aMat;

    aMat(1, 1) = 1.0 - (yy + zz);
    aMat(1, 2) = xy - wz;
    aMat(1, 3) = xz + wy;

    aMat(2, 1) = xy + wz;
    aMat(2, 2) = 1.0 - (xx + zz);
    aMat(2, 3) = yz - wx;

    aMat(3, 1) = xz - wy;
    aMat(3, 2) = yz + wx;
    aMat(3, 3) = 1.0 - (xx + yy);
    // 1 division    16 multiplications    15 addidtions    12 variables

    return aMat;
}

//=======================================================================
// function : StabilizeLength
// purpose  :
//=======================================================================

void GeQuaternion::stabilizeLength()
{
    double cs = std::fabs(x) + std::fabs(y) + std::fabs(z) + std::fabs(w);
    if (cs > 0.0)
    {
        x /= cs;
        y /= cs;
        z /= cs;
        w /= cs;
    }
    else
    {
        setIdent();
    }
}

//=======================================================================
// function : normalize
// purpose  :
//=======================================================================

void GeQuaternion::normalize()
{
    double aMagn = norm();
    if (aMagn < GeContext::gTol.equalPoint())
    {
        stabilizeLength();
        aMagn = norm();
    }
    scale(1.0 / aMagn);
}

//=======================================================================
// function : Set
// purpose  :
//=======================================================================
void GeQuaternion::set(double theX, double theY, double theZ, double theW)
{
    this->x = theX;
    this->y = theY;
    this->z = theZ;
    this->w = theW;
}

//=======================================================================
// function : Set
// purpose  :
//=======================================================================
void GeQuaternion::set(const GeQuaternion &theQuaternion)
{
    x = theQuaternion.x;
    y = theQuaternion.y;
    z = theQuaternion.z;
    w = theQuaternion.w;
}

//=======================================================================
// function : scale
// purpose  :
//=======================================================================
void GeQuaternion::scale(const double thescale)
{
    x *= thescale;
    y *= thescale;
    z *= thescale;
    w *= thescale;
}

//=======================================================================
// function : Multiplied
// purpose  :
//=======================================================================
GeQuaternion GeQuaternion::multiplied(const GeQuaternion &theQ) const
{
    return GeQuaternion(
        w * theQ.x + x * theQ.w + y * theQ.z - z * theQ.y,
        w * theQ.y + y * theQ.w + z * theQ.x - x * theQ.z,
        w * theQ.z + z * theQ.w + x * theQ.y - y * theQ.x,
        w * theQ.w - x * theQ.x - y * theQ.y - z * theQ.z);
    // 16 multiplications    12 addidtions    0 variables
}

//=======================================================================
// function : Add
// purpose  :
//=======================================================================
void GeQuaternion::add(const GeQuaternion &theQ)
{
    x += theQ.x;
    y += theQ.y;
    z += theQ.z;
    w += theQ.w;
}

//=======================================================================
// function : Subtract
// purpose  :
//=======================================================================
void GeQuaternion::subtract(const GeQuaternion &theQ)
{
    x -= theQ.x;
    y -= theQ.y;
    z -= theQ.z;
    w -= theQ.w;
}

//=======================================================================
// function : normalize
// purpose  :
//=======================================================================

double GeQuaternion::getRotationAngle() const
{
    if (w < 0.0)
    {
        return 2.0 * atan2(-sqrt(x * x + y * y + z * z), -w);
    }
    else
    {
        return 2.0 * atan2(sqrt(x * x + y * y + z * z), w);
    }
}

//=======================================================================
// function : multiply
// purpose  :
//=======================================================================

GeVector3d GeQuaternion::multiply(const GeVector3d &theVec) const
{
    GeQuaternion theQ(theVec.x * w + theVec.z * y - theVec.y * z,
                      theVec.y * w + theVec.x * z - theVec.z * x,
                      theVec.z * w + theVec.y * x - theVec.x * y,
                      theVec.x * x + theVec.y * y + theVec.z * z);
    return GeVector3d(w * theQ.x + x * theQ.w + y * theQ.z - z * theQ.y,
                      w * theQ.y + y * theQ.w + z * theQ.x - x * theQ.z,
                      w * theQ.z + z * theQ.w + x * theQ.y - y * theQ.x) *
           (1.0 / squareNorm());
}
