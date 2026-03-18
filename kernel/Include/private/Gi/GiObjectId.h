#ifndef GIOBJECTID_H
#define GIOBJECTID_H


#include "AcArray.h"
#include "RxObject.h"




class GiObjectId
{
public:
    GiObjectId(unsigned int _id = 0) : m_Id(_id) { }

    static GiObjectId kNull;

    bool isNull() const { return m_Id == 0; }
    void setNull() { m_Id = 0; }

    unsigned int asOldId() const { return this->m_Id; };
    GiObjectId& setFromOldId(unsigned int oldId) { this->m_Id = oldId; return *this; };

    bool isValid() const;

    GiObjectId& operator = (unsigned int _m_Id) { m_Id = _m_Id; return *this; }
    bool operator < (const GiObjectId& objectId) const;
    bool operator > (const GiObjectId& objectId) const;
    bool operator >= (const GiObjectId& objectId) const;
    bool operator <= (const GiObjectId& objectId) const;
    bool operator == (const GiObjectId& objectId) const;
    bool operator != (const GiObjectId& objectId) const;
    bool operator ! () const { return isNull(); }


protected:
    unsigned int m_Id;
};



typedef AcArray<GiObjectId> GiObjectIdArray;






#endif



