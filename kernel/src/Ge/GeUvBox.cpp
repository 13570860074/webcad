#include "GeUvBox.h"
#include "GeExtents2d.h"

namespace {
double uvbox_clampValue(double value, double lower, double upper)
{
    if (value < lower) {
        return lower;
    }
    if (value > upper) {
        return upper;
    }
    return value;
}

double uvbox_evalValue(const GeInterval& interval, double ratio)
{
    double lower = interval.lowerBound();
    double upper = interval.upperBound();
    return lower + (upper - lower) * ratio;
}

bool uvbox_intervalContains(const GeInterval& interval, double value)
{
    if (interval.isBoundedBelow() && value < interval.lowerBound()) {
        return false;
    }
    if (interval.isBoundedAbove() && value > interval.upperBound()) {
        return false;
    }
    return true;
}
}

GeUvBox::GeUvBox()
{
}

GeUvBox::GeUvBox(const GeInterval& intervalU, const GeInterval& intervalV)
{
    intervals[0] = intervalU;
    intervals[1] = intervalV;
}

const GeInterval& GeUvBox::u() const
{
    return intervals[0];
}

const GeInterval& GeUvBox::v() const
{
    return intervals[1];
}

GeInterval& GeUvBox::u()
{
    return intervals[0];
}

GeInterval& GeUvBox::v()
{
    return intervals[1];
}

const GeInterval& GeUvBox::operator[] (int idx) const
{
    return intervals[idx];
}

GeInterval& GeUvBox::operator[] (int idx)
{
    return intervals[idx];
}

GeUvBox& GeUvBox::set()
{
    intervals[0].set();
    intervals[1].set();
    return *this;
}

GeUvBox& GeUvBox::set(const GeInterval& intervalU, const GeInterval& intervalV)
{
    intervals[0] = intervalU;
    intervals[1] = intervalV;
    return *this;
}

GeUvBox& GeUvBox::set(const GePoint2d& lowerBound, const GePoint2d& upperBound)
{
    intervals[0].set(lowerBound.x, upperBound.x);
    intervals[1].set(lowerBound.y, upperBound.y);
    return *this;
}

bool GeUvBox::contains(double uparam, double vparam) const
{
    return uvbox_intervalContains(u(), uparam) && uvbox_intervalContains(v(), vparam);
}

bool GeUvBox::contains(const GePoint2d& uvpoint) const
{
    return this->contains(uvpoint.x, uvpoint.y);
}

bool GeUvBox::isBounded() const
{
    return u().isBounded() && v().isBounded();
}

bool GeUvBox::finiteIntersectWith(const GeUvBox& range, GeUvBox& result) const
{
    GeInterval iu;
    GeInterval iv;
    bool uOk = u().intersectWith(range.u(), iu);
    bool vOk = v().intersectWith(range.v(), iv);

    if (uOk == false || vOk == false) {
        return false;
    }

    result.set(iu, iv);
    return result.isBounded();
}

GePoint2d GeUvBox::lowerBound() const
{
    return GePoint2d(u().lowerBound(), v().lowerBound());
}

GePoint2d GeUvBox::upperBound() const
{
    return GePoint2d(u().upperBound(), v().upperBound());
}

bool GeUvBox::isEqualTo(const GeUvBox& uvbox) const
{
    return u() == uvbox.u() && v() == uvbox.v();
}

GeUvBox::operator GeExtents2d() const
{
    return GeExtents2d(this->lowerBound(), this->upperBound());
}

GePoint2d GeUvBox::eval(double ratioU, double ratioV) const
{
    return GePoint2d(uvbox_evalValue(u(), ratioU), uvbox_evalValue(v(), ratioV));
}

GePoint2d GeUvBox::clamp(const GePoint2d& point) const
{
    double clampedU = point.x;
    double clampedV = point.y;

    if (u().isBounded()) {
        clampedU = uvbox_clampValue(point.x, u().lowerBound(), u().upperBound());
    }
    if (v().isBounded()) {
        clampedV = uvbox_clampValue(point.y, v().lowerBound(), v().upperBound());
    }

    return GePoint2d(clampedU, clampedV);
}

bool GeUvBox::operator == (const GeUvBox& otherUvBox) const
{
    return this->isEqualTo(otherUvBox);
}