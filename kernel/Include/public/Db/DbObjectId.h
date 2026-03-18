
#ifndef DBOBJECTID_H
#define DBOBJECTID_H


#include "export.h"
#include "AcArray.h"
#include "PlatformSettings.h"
#include "DbHandle.h"



class DbDatabase;






/** \details
    This class implements memory-resident ObjectId objects for OdDbDatabase objects.  

    \remarks
    Database objects reference
    other database objects using ObjectId objects, and a database object pointer
    can always be obtained from a valid ObjectId objects. The effect of this mechanism is
    that database objects do not have to reside in memory unless they are explicitly
    being examined or modified by the user.  
    
    The user must explicitly open an object
    before reading or writing to it, and should release it when the operation
    is completed.  This functionality allows Teigha to support partial loading of 
    a database, where ObjectId objects exist for all objects in the database, but 
    the actual database objects need not be loaded until they are accessed.  
    It also allows database objects that are not in use to be swapped out of memory, 
    and loaded back in when they are accessed.  ObjectId objects are not written out to a 
    DWG/DXF file.  If a reference must be preserved to a database object that has been 
    serialized, the object's database handle (OdDbHandle) should be used.

    \sa
    * OdDbHandle
    * OdDbObject

    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class DbObjectId
{
public:

#if EMSDK
    static DbObjectId cast(unsigned int inPtr) {
        return (DbObjectId&)inPtr;
    }
#endif

    /** \remarks
        This function guarantees that isNull() will 
        return true if it is the first operation applied to this instance.
    */
    DbObjectId(unsigned int _id = 0) : m_Id(_id) {}

    /** \details
        The null ObjectId object.
    */
    static const DbObjectId kNull;

    bool isNull () const { return m_Id == 0; }
    void setNull () { m_Id = 0; }

    unsigned int asOldId() const { return this->m_Id; };
    DbObjectId& setFromOldId(unsigned int oldId) { 
        this->m_Id = oldId; 
        return *this; 
    };

    /** \details
        Returns true and only if this ObjectId object references a valid object.
    */
    bool isValid () const;

    DbObjectId& operator = (unsigned int _m_Id) { m_Id = _m_Id; return *this; }
    DbObjectId& operator = (const DbObjectId& objectId) { m_Id = objectId.m_Id; return *this; }
    bool operator < (const DbObjectId& objectId) const;
    bool operator > (const DbObjectId& objectId) const;
    bool operator >= (const DbObjectId& objectId) const;
    bool operator <= (const DbObjectId& objectId) const;
    bool operator == (const DbObjectId& objectId) const;
    bool operator != (const DbObjectId& objectId) const;
    bool operator ! () const { return isNull(); }



    bool isErased () const;

    DbHandle handle() const;


protected:
    unsigned int m_Id;
};





#endif


