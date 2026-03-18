#ifndef GEINTERVAL_H
#define GEINTERVAL_H 

#include "EmObject.h"
#include "GeExport.h"



class GeInterval INHERIT_EM_OBJECT
{
public:
    GeInterval();
    GeInterval(double tol);
    GeInterval(const GeInterval& src);
    GeInterval(double lower, double upper);
    GeInterval(double lower, double upper, double tol);
    GeInterval(bool boundedBelow, double bound, double tol = 1.e-12);
    ~GeInterval();

    // Assignment operator.
    //
    GeInterval& operator = (const GeInterval& otherInterval);

    // Get/set methods.
    //
    double lowerBound() const;
    double upperBound() const;
    double element() const;
    void getBounds(double& lower, double& upper) const;
    double length() const;
    double tolerance() const;

    GeInterval& set(double lower, double upper);
    GeInterval& set(bool boundedBelow, double bound);
    GeInterval& set();
    GeInterval& setUpper(double upper);
    GeInterval& setLower(double lower);
    GeInterval& setTolerance(double tol);

    // Interval editing.
    //
    void getMerge(const GeInterval& otherInterval, GeInterval& result) const;
    int subtract(const GeInterval& otherInterval, GeInterval& lInterval, GeInterval& rInterval) const;
    bool intersectWith(const GeInterval& otherInterval, GeInterval& result) const;

    // Interval characterization.
    //
    bool isBounded() const;
    bool isBoundedAbove() const;
    bool isBoundedBelow() const;
    bool isUnBounded() const;
    bool isSingleton() const;

    // Relation to other intervals.
    //
    bool isDisjoint(const GeInterval& otherInterval) const;
    bool contains(const GeInterval& otherInterval) const;
    bool contains(double val) const;


    // Equality
    //
    bool operator == (const GeInterval& otherInterval) const;
    bool operator != (const GeInterval& otherInterval) const;
    bool isEqualAtUpper(const GeInterval& otherInterval) const;
    bool isEqualAtUpper(double value) const;
    bool isEqualAtLower(const GeInterval& otherInterval) const;
    bool isEqualAtLower(double value) const;

    // Comparisons.
    //
    friend bool operator > (double val, const GeInterval& intrvl);
    bool operator > (double val) const;
    bool operator > (const GeInterval& otherInterval) const;
    friend bool operator >= (double val, const GeInterval& intrvl);
    bool operator >= (double val) const;
    bool operator >= (const GeInterval& otherInterval) const;
    friend bool operator < (double val, const GeInterval& intrvl);
    bool operator < (double val) const;
    bool operator < (const GeInterval& otherInterval) const;
    friend bool operator <= (double val, const GeInterval& intrvl);
    bool operator <= (double val) const;
    bool operator <= (const GeInterval& otherInterval) const;

private:
    double m_Tol;
    double m_UpperParam;
    double m_LowerParam;
    bool m_bBoundedAbove;
    bool m_bBoundedBelow;
};







#endif

