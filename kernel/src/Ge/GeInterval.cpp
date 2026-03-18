#include "GeInterval.h"
#include <cstdlib>
#include <vector>

GeInterval::GeInterval() {
    this->m_Tol = 1.e-6;
    this->m_UpperParam = 0.0;
    this->m_LowerParam = 0.0;
    this->m_bBoundedAbove = false;
    this->m_bBoundedBelow = false;
    
}
GeInterval::GeInterval(const GeInterval& src) {
    this->m_Tol = src.m_Tol;
    this->m_UpperParam = src.m_UpperParam;
    this->m_LowerParam = src.m_LowerParam;
    this->m_bBoundedAbove = src.m_bBoundedAbove;
    this->m_bBoundedBelow = src.m_bBoundedBelow;
}
GeInterval::GeInterval(double tol)
{
    this->m_Tol = tol;
    this->m_UpperParam = 0.0;
    this->m_LowerParam = 0.0;
    this->m_bBoundedAbove = false;
    this->m_bBoundedBelow = false;
}
GeInterval::GeInterval(double lower, double upper)
{
    this->m_Tol = 1.e-12;
    this->m_UpperParam = upper;
    this->m_LowerParam = lower;
    this->m_bBoundedAbove = true;
    this->m_bBoundedBelow = true;
}
GeInterval::GeInterval(double lower, double upper, double tol)
{
    this->m_Tol = tol;
    this->m_UpperParam = upper;
    this->m_LowerParam = lower;
    this->m_bBoundedAbove = true;
    this->m_bBoundedBelow = true;
}
GeInterval::GeInterval(bool boundedBelow, double bound, double tol)
{
    this->m_Tol = tol;
    this->m_LowerParam = bound;
    this->m_bBoundedBelow = boundedBelow;
    this->m_UpperParam = 0.0;
    this->m_bBoundedAbove = false;
}
GeInterval::~GeInterval()
{

}

double GeInterval::lowerBound() const
{
    return this->m_LowerParam;
}

double GeInterval::upperBound() const
{
    return this->m_UpperParam;
}


double GeInterval::element() const
{
    if (this->isBoundedAbove() == true) {
        return this->upperBound();
    }
    else if (this->isBoundedAbove() == false && this->isBoundedBelow() == true) {
        return this->lowerBound();
    }
    return 0.0;
}

void GeInterval::getBounds(double& lower, double& upper) const
{
    lower = this->m_LowerParam;
    upper = this->m_UpperParam;
}
double GeInterval::length() const {
    return this->m_UpperParam - this->m_LowerParam;
}
double GeInterval::tolerance() const
{
    return this->m_Tol;
}

GeInterval& GeInterval::set(double lower, double upper)
{
    this->m_LowerParam = lower;
    this->m_UpperParam = upper;
    this->m_bBoundedBelow = true;
    this->m_bBoundedAbove = true;
    return *this;
}
GeInterval& GeInterval::set(bool boundedBelow, double bound)
{
    this->m_bBoundedBelow = boundedBelow;
    this->m_LowerParam = bound;
    this->m_bBoundedAbove = false;
    this->m_UpperParam = 0.0;
    return *this;
}
GeInterval& GeInterval::set()
{
    this->m_Tol = 1.e-7;
    this->m_UpperParam = 0.0;
    this->m_LowerParam = 0.0;
    this->m_bBoundedAbove = false;
    this->m_bBoundedBelow = false;
    return *this;
}
GeInterval& GeInterval::setUpper(double upper)
{
    this->m_UpperParam = upper;
    return *this;
}
GeInterval& GeInterval::setLower(double lower)
{
    this->m_LowerParam = lower;
    return *this;
}
GeInterval& GeInterval::setTolerance(double tol)
{
    this->m_Tol = tol;
    return *this;
}

void GeInterval::getMerge(const GeInterval& otherInterval, GeInterval& result) const
{
    result.m_LowerParam = this->m_LowerParam;
    if (this->m_UpperParam < result.m_LowerParam) {
        result.m_LowerParam = this->m_UpperParam;
    }
    if (otherInterval.m_LowerParam < result.m_LowerParam) {
        result.m_LowerParam = otherInterval.m_LowerParam;
    }
    if (otherInterval.m_UpperParam < result.m_LowerParam) {
        result.m_LowerParam = otherInterval.m_UpperParam;
    }

    result.m_UpperParam = this->m_UpperParam;
    if (this->m_LowerParam > result.m_UpperParam) {
        result.m_UpperParam = this->m_LowerParam;
    }
    if (otherInterval.m_LowerParam > result.m_UpperParam) {
        result.m_UpperParam = otherInterval.m_LowerParam;
    }
    if (otherInterval.m_UpperParam > result.m_UpperParam) {
        result.m_UpperParam = otherInterval.m_UpperParam;
    }

    result.m_bBoundedBelow = false;
    if (this->m_bBoundedBelow == true || otherInterval.m_bBoundedBelow == true) {
        result.m_bBoundedBelow = true;
    }

    result.m_bBoundedAbove = false;
    if (this->m_bBoundedAbove == true || otherInterval.m_bBoundedAbove == true) {
        result.m_bBoundedAbove = true;
    }
}
   
int GeInterval::subtract(const GeInterval& otherInterval, GeInterval& lInterval, GeInterval& rInterval) const
{
    double otherLowerBound = otherInterval.lowerBound();
    if (otherInterval.upperBound() < otherLowerBound) {
        otherLowerBound = otherInterval.upperBound();
    }
    double otherUpperBound = otherInterval.upperBound();
    if (otherInterval.lowerBound() > otherUpperBound) {
        otherUpperBound = otherInterval.lowerBound();
    }

    double thisLowerBound = this->lowerBound();
    if (this->upperBound() < thisLowerBound) {
        thisLowerBound = this->upperBound();
    }
    double thisUpperBound = this->upperBound();
    if (this->lowerBound() > thisUpperBound) {
        thisUpperBound = this->lowerBound();
    }

    if (otherUpperBound < thisLowerBound) {
        return 0;
    }
    if (otherLowerBound < thisLowerBound && otherUpperBound > thisLowerBound && otherUpperBound < thisUpperBound) {
        lInterval.set(otherUpperBound, thisUpperBound);
        return 1;
    }
    if (otherLowerBound > thisLowerBound && otherUpperBound < thisUpperBound)
    {
        lInterval.set(thisLowerBound, otherLowerBound);
        rInterval.set(otherUpperBound, thisUpperBound);
        return 2;
    }
    if (otherLowerBound > thisLowerBound && otherLowerBound < thisUpperBound && otherUpperBound > thisUpperBound)
    {
        lInterval.set(thisLowerBound, otherLowerBound);
        return 1;
    }
    return 0;
}

bool GeInterval::intersectWith(const GeInterval& otherInterval, GeInterval& result) const
{
    double otherLowerBound = otherInterval.lowerBound();
    if (otherInterval.upperBound() < otherLowerBound) {
        otherLowerBound = otherInterval.upperBound();
    }
    double otherUpperBound = otherInterval.upperBound();
    if (otherInterval.lowerBound() > otherUpperBound) {
        otherUpperBound = otherInterval.lowerBound();
    }

    double thisLowerBound = this->lowerBound();
    if (this->upperBound() < thisLowerBound) {
        thisLowerBound = this->upperBound();
    }
    double thisUpperBound = this->upperBound();
    if (this->lowerBound() > thisUpperBound) {
        thisUpperBound = this->lowerBound();
    }

    if (otherUpperBound < thisLowerBound) {
        return false;
    }
    if (otherLowerBound < thisLowerBound && otherUpperBound > thisLowerBound && otherUpperBound < thisUpperBound) {
        result.set(thisLowerBound, otherUpperBound);
        return true;
    }
    if (otherLowerBound > thisLowerBound && otherUpperBound < thisUpperBound)
    {
        result.set(otherLowerBound, otherUpperBound);
        return true;
    }
    if (otherLowerBound > thisLowerBound && otherLowerBound < thisUpperBound && otherUpperBound > thisUpperBound)
    {
        result.set(otherLowerBound, thisUpperBound);
        return true;
    }

    return false;
}


bool GeInterval::isBounded() const
{
    if (this->isBoundedAbove() == true && this->isBoundedBelow() == true) {
        return true;
    }
    return false;
}
    
bool GeInterval::isBoundedAbove() const
{
    return this->m_bBoundedAbove;
}
    
bool GeInterval::isBoundedBelow() const
{
    return this->m_bBoundedBelow;
}

bool GeInterval::isUnBounded() const
{
    if (this->isBoundedAbove() == false || this->isBoundedBelow() == false) {
        return true;
    }
    return false;
}
  
bool GeInterval::isSingleton() const
{
    if (this->isBoundedAbove() == true || this->isBoundedBelow() == true) {
        if (this->length() < this->tolerance()) {
            return true;
        }
    }
    return false;
}

bool GeInterval::isDisjoint(const GeInterval& otherInterval) const
{
    GeInterval result;
    return this->intersectWith(otherInterval, result);
}


bool GeInterval::contains(const GeInterval& otherInterval) const
{
    int num = 0;

    if ((otherInterval.lowerBound() < this->lowerBound() + this->tolerance() && otherInterval.lowerBound() > this->upperBound() - this->tolerance()) ||
        (otherInterval.lowerBound() > this->lowerBound() - this->tolerance() && otherInterval.lowerBound() < this->upperBound() + this->tolerance())) {
        num++;
    }
    if ((otherInterval.upperBound() < this->lowerBound() + this->tolerance() && otherInterval.upperBound() > this->upperBound() - this->tolerance()) ||
        (otherInterval.upperBound() > this->lowerBound() - this->tolerance() && otherInterval.upperBound() < this->upperBound() + this->tolerance())) {
        num++;
    }
    if (num == 2) {
        return true;
    }
    return false;
}
bool GeInterval::contains(double value) const
{
    if ((value < this->lowerBound() + this->tolerance() && value > this->upperBound() - this->tolerance()) ||
        (value > this->lowerBound() - this->tolerance() && value < this->upperBound() + this->tolerance())) {
        return true;
    }
    return false;
}

bool GeInterval::operator ==(const GeInterval& otherInterval) const
{
    if (this->m_bBoundedBelow != otherInterval.m_bBoundedBelow) {
        return false;
    }
    if (this->m_bBoundedAbove != otherInterval.m_bBoundedAbove) {
        return false;
    }
    if (abs(this->m_LowerParam - otherInterval.m_LowerParam) > this->tolerance()) {
        return false;
    }
    if (abs(this->m_UpperParam - otherInterval.m_UpperParam) > this->tolerance()) {
        return false;
    }
    return true;
}
bool GeInterval::operator !=(const GeInterval& otherInterval) const
{
    return !(* this == otherInterval);
}
bool GeInterval::isEqualAtUpper(const GeInterval& otherInterval) const
{
    if (this->m_bBoundedAbove != otherInterval.m_bBoundedAbove) {
        return false;
    }
    if (abs(this->m_UpperParam - otherInterval.m_UpperParam) > this->tolerance()) {
        return false;
    }
    return true;
}
bool GeInterval::isEqualAtUpper(double value) const
{
    if (abs(this->m_UpperParam - value) > this->tolerance()) {
        return false;
    }
    return true;
}
bool GeInterval::isEqualAtLower(const GeInterval& otherInterval) const
{
    if (this->m_bBoundedBelow != otherInterval.m_bBoundedBelow) {
        return false;
    }
    if ((this->m_LowerParam - otherInterval.m_LowerParam) > this->tolerance()) {
        return false;
    }
    return true;
}
bool GeInterval::isEqualAtLower(double value) const
{
    if ((this->m_LowerParam - value) > this->tolerance()) {
        return false;
    }
    return true;
}

bool GeInterval::operator > (double value) const
{
    if (this->length() > value) {
        return true;
    }
    return false;
}
bool GeInterval::operator > (const GeInterval& otherInterval) const
{
    if (this->length() > otherInterval.length()) {
        return true;
    }
    return false;
}
bool GeInterval::operator >= (double value) const
{
    if (this->length() > value - this->tolerance()) {
        return true;
    }
    return false;
}
bool GeInterval::operator >= (const GeInterval& otherInterval) const
{
    if (this->length() > otherInterval.length() - this->tolerance()) {
        return true;
    }
    return false;
}
bool GeInterval::operator < (double value) const
{
    if (this->length() < value) {
        return true;
    }
    return false;
}
bool GeInterval::operator < (const GeInterval& otherInterval) const
{
    if (this->length() < otherInterval.length()) {
        return true;
    }
    return false;
}
bool GeInterval::operator <= (double value) const
{
    if (this->length() < value + this->tolerance()) {
        return true;
    }
    return false;
}
bool GeInterval::operator <= (const GeInterval& otherInterval) const
{
    if (this->length() < otherInterval.length() + this->tolerance()) {
        return true;
    }
    return false;
}
