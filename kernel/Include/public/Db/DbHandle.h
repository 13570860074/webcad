
#ifndef DBHANDLE_H
#define DBHANDLE_H

#include "export.h"
#include "AcArray.h"
#include "AcString.h"


class DbHandle
{
public:
    DbHandle() : m_val(0) {}
    DbHandle(const DbHandle& value) : m_val(value.m_val) {}
    DbHandle(const ACHAR*);
    DbHandle(unsigned int value) : m_val(value) {}
    DbHandle& operator=(unsigned int value) {
        m_val = value;
        return *this;
    }
    DbHandle& operator=(const DbHandle& value) {
        m_val = value.m_val;
        return *this;
    }
    DbHandle& operator=(int value) {
        m_val = value;
        return *this;
    }

    operator unsigned int() const { return m_val; }
  
    /** \details
        Returns true and only if this Handle object is null.
    */
    bool isNull() const { return m_val == 0; }
  
    bool operator == (unsigned int value) const { return m_val == value; }
  
    bool operator != (unsigned int value) const { return m_val != value; }
  
    bool operator > (unsigned int value) const { return m_val > value; }
  
    bool operator < (unsigned int value) const { return m_val < value; }
  
    bool operator <= (unsigned int value) const { return m_val < value || m_val == value; }
  
    bool operator >= (unsigned int value) const { return m_val > value || m_val == value; }
  
    DbHandle& operator+=(const unsigned int& value){
        m_val += value;
        return *this;
    }
    DbHandle operator+(const unsigned int& value)
    {
        DbHandle res;
        res.m_val = m_val + value;
        return res;
    }

    bool getIntoAsciiBuffer(ACHAR* pBuf, size_t nBufLen) const;
#if EMSDK
    emstring emsdk_getIntoAsciiBuffer() const;
#endif
    // Helper template for fixed size arrays
    template<size_t nBufLen> inline bool getIntoAsciiBuffer(wchar_t(&buf)[nBufLen]) const
    {
        return this->getIntoAsciiBuffer(buf, nBufLen);
    }


private:
    unsigned int  m_val;
};

typedef AcArray<DbHandle> DbHandleArray;


#endif

