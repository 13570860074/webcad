#ifndef GEENTITY2D_H
#define GEENTITY2D_H 

class GeMatrix2d;
class GeLine2d;
class GeImpEntity2d;

#include "EmObject.h"
#include "GePoint2d.h"

class GeLine2d;
class GeEntity2d INHERIT_EM_OBJECT
{
public:
    virtual ~GeEntity2d();

    // Run time type information.
    //
    virtual bool isKindOf(Ge::EntityId entType) const;
    virtual Ge::EntityId type() const;

    // Make a copy of the entity.
    //
    virtual GeEntity2d* copy() const;
    virtual GeEntity2d& operator = (const GeEntity2d& entity);

    // Equivalence
    //
    virtual bool operator == (const GeEntity2d& entity) const;
    virtual bool operator != (const GeEntity2d& entity) const;
    virtual bool isEqualTo(const GeEntity2d& entity) const;
    virtual bool isEqualTo(const GeEntity2d& entity, const GeTol& tol) const;


    // Matrix multiplication
    //
    virtual GeEntity2d& transformBy(const GeMatrix2d& xfm);
    virtual GeEntity2d& translateBy(const GeVector2d& translateVec);
    virtual GeEntity2d& rotateBy(double angle);
    virtual GeEntity2d& rotateBy(double angle, const GePoint2d& wrtPoint);
    virtual GeEntity2d& mirror(const GeLine2d& line);
    virtual GeEntity2d& scaleBy(double scaleFactor);
    virtual GeEntity2d& scaleBy(double scaleFactor, const GePoint2d& wrtPoint);
    // Point containment
    //
    virtual bool isOn(const GePoint2d& pnt) const;
    virtual bool isOn(const GePoint2d& pnt, const GeTol& tol) const;


em_protected:
    GeEntity2d();
    GeEntity2d(const GeEntity2d& entity);


em_protected:
    GeEntity2d(GeImpEntity2d*);
    GeImpEntity2d* m_pImpl;
};




#endif 

