#ifndef GEPNT2D_H
#define GEPNT2D_H

#include <math.h>
#include "export.h"
#include "EmObject.h"
#include "GeVector2d.h"



class GeMatrix2d;
class GeVector2d;
class GeLinearEnt2d;
class GeLine2d;
class GeScale2d;


class GePoint2d INHERIT_EM_OBJECT
{
public:
    GePoint2d(): x(0.0), y(0.0)
    {
    }
    GePoint2d(const GePoint2d& pnt) : x(pnt.x), y(pnt.y) {

    }
    GePoint2d(double xx,double yy): x(xx), y(yy)
    {
    }

    GE_STATIC_EXPORT static const GePoint2d kOrigin; // Origin (0,0).

    friend GePoint2d operator *(const GeMatrix2d& matrix,const GePoint2d& point);

    friend GePoint2d operator *(double scale,const GePoint2d& point)
    {
        return GePoint2d(point.x * scale, point.y * scale);
    }

    GePoint2d& setToProduct(const GeMatrix2d& matrix,const GePoint2d& point);

    GePoint2d& transformBy(const GeMatrix2d& xfm);

    GePoint2d& translation(const GeVector2d& vec)
    {
        this->x += vec.x;
        this->y += vec.y;
        return *this;
    }

    GePoint2d& rotateBy(double angle);
    GePoint2d& rotateBy(double angle,const GePoint2d& basePoint);

    GePoint2d& mirror(const GeLine2d& line);

    GePoint2d& scaleBy(const double scaleFactor);
    GePoint2d& scaleBy(double scaleFactor, const GePoint2d& basePoint);
    GePoint2d& scaleBy(const GeScale2d& scaleFactor);
    GePoint2d& scaleBy(const GeScale2d& scaleFactor, const GePoint2d& basePoint);


    GePoint2d operator *(double scale) const{
        return GePoint2d(x * scale, y * scale);
    }

    GePoint2d& operator *=(double scale){
        x *= scale;
        y *= scale;
        return *this;
    }

    GePoint2d operator /(double scale) const{
        return GePoint2d(x / scale, y / scale);
    }

    GePoint2d& operator /=(double scale){
        x /= scale;
        y /= scale;
        return *this;
    }

    GePoint2d operator +(const GeVector2d& vect) const{
        return GePoint2d(x + vect.x, y + vect.y);
    }

    GePoint2d& operator +=(const GeVector2d& vect){
        x += vect.x;
        y += vect.y;
        return *this;
    }

    GePoint2d operator -(const GeVector2d& vect) const
    {
        return GePoint2d(x - vect.x, y - vect.y);
    }

    GePoint2d& operator -=(const GeVector2d& vect){
        x -= vect.x;
        y -= vect.y;
        return *this;
    }

    GePoint2d& setToSum(const GePoint2d& point,const GeVector2d& vect){
        x = point.x + vect.x;
        y = point.y + vect.y;
        return *this;
    }

    GeVector2d operator -(const GePoint2d& point) const
    {
        return GeVector2d(x - point.x, y - point.y);
    }

    GeVector2d operator +(const GePoint2d& point) const
    {
        return GeVector2d(x + point.x, y + point.y);
    }

    const GeVector2d& asVector() const
    {
        return *reinterpret_cast<const GeVector2d*>(this);
    }

    double distanceTo(const GePoint2d& point) const
    {
        return sqrt((x - point.x) * (x - point.x) + (y - point.y) * (y - point.y));
    }

    bool operator ==(const GePoint2d& point) const
    {
        return isEqualTo(point);
    }
    bool operator !=(const GePoint2d& point) const
    {
        return !isEqualTo(point);
    }

    bool isEqualTo(const GePoint2d& point) const;
    bool isEqualTo(const GePoint2d& point, const GeTol& tol) const;

    double& operator [](unsigned int i)
    {
        return *(&x + i);
    }
    double operator [](unsigned int i) const
    {
        return *(&x + i);
    }

    GePoint2d& set(double xx,double yy)
    {
        x = xx;
        y = yy;
        return *this;
    }

    double x; // X-coordinate
    double y; // Y-coordinate
};








#endif
