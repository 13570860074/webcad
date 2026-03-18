#ifndef DBBLOCKREFERENCE_H
#define DBBLOCKREFERENCE_H


#include "DbEntity.h"
#include "GeMatrix3d.h"


class DbBlockReference :public DbEntity{
public:
	DbBlockReference();
    DbBlockReference(const GePoint3d& position, DbObjectId  blockTableRec);
	virtual ~DbBlockReference();
    DB_ENTITY_DECLARE_MEMBERS(DbBlockReference);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

public:
    DbObjectId      blockTableRecord() const;
    virtual Acad::ErrorStatus setBlockTableRecord(DbObjectId);

    virtual GePoint3d       position() const;
    virtual Acad::ErrorStatus setPosition(const GePoint3d&);

    GeScale3d       scaleFactors() const;
    virtual Acad::ErrorStatus setScaleFactors(const GeScale3d& scale);

    double            rotation() const;
    virtual Acad::ErrorStatus setRotation(double newVal);

    GeVector3d      normal() const;
    virtual Acad::ErrorStatus setNormal(const GeVector3d& newVal);

    Adesk::Boolean    isPlanar() const override { return Adesk::kTrue; }
    Acad::ErrorStatus getPlane(GePlane&, Db::Planarity&) const override;

    GeMatrix3d      blockTransform() const;
    virtual Acad::ErrorStatus setBlockTransform(const GeMatrix3d&);

};



#endif