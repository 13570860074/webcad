#ifndef GEENTITY3D_H
#define GEENTITY3D_H 

#include "EmObject.h"
#include "GePoint3d.h"


class GeMatrix3d;
class GeLine3d;
class GeImpEntity3d;
class GePlane;
class GeVector3d;
class GeEntity3d INHERIT_EM_OBJECT
{
public:
    virtual ~GeEntity3d();

    // Run time type information.
    //
    virtual bool isKindOf(Ge::EntityId entType) const;
    virtual Ge::EntityId type() const;

    // Make a copy of the entity.
    //
    virtual GeEntity3d* copy() const;
    virtual GeEntity3d& operator = (const GeEntity3d& entity);

    // Equivalence
    //
    virtual bool operator == (const GeEntity3d& entity) const;
    virtual bool operator != (const GeEntity3d& entity) const;
    virtual bool isEqualTo(const GeEntity3d& entity) const;
    virtual bool isEqualTo(const GeEntity3d& entity, const GeTol& tol) const;


    // Matrix multiplication
    //
    virtual GeEntity3d& transformBy(const GeMatrix3d& xfm);
    virtual GeEntity3d& translateBy(const GeVector3d& translateVec);
    virtual GeEntity3d& rotateBy(double angle, const GeVector3d& vec);
    virtual GeEntity3d& rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint);
    virtual GeEntity3d& mirror(const GePlane& plane);
    virtual GeEntity3d& scaleBy(double scaleFactor);
    virtual GeEntity3d& scaleBy(double scaleFactor, const GePoint3d& wrtPoint);
    // Point containment
    //
    virtual bool isOn(const GePoint3d& pnt) const;
    virtual bool isOn(const GePoint3d& pnt, const GeTol& tol) const;


em_protected:
    GeEntity3d();
    GeEntity3d(const GeEntity3d& entity);


em_protected:
    GeEntity3d(GeImpEntity3d*);
    GeImpEntity3d* m_pImpl;
};







#endif 

