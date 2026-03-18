#ifndef DBXRECORD_H
#define DBXRECORD_H

#include "Db.h"
#include "DbObject.h"

class DbDwgFiler;
class DbDxfFiler;
class DbXrecord final : public DbObject
{
public:
    DB_DECLARE_MEMBERS(DbXrecord);
    DbXrecord();
    virtual ~DbXrecord();


    // auxDb parameter only useded when working with
    // non-Database-resident instances.

    Acad::ErrorStatus rbChain(resbuf** ppRb, DbDatabase* auxDb = NULL) const;
    Acad::ErrorStatus setFromRbChain(const resbuf& pRb,
        DbDatabase* auxDb = NULL);
    Acad::ErrorStatus appendRbChain(const resbuf& pRb);
    bool              isXlateReferences() const;
    void              setXlateReferences(bool translate);

    // Overridden methods from DbObject
    //
    Db::DuplicateRecordCloning mergeStyle() const override;
    virtual void  setMergeStyle(Db::DuplicateRecordCloning style);
};


#endif
