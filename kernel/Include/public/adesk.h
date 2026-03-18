#ifndef   _ADESK_H
#define   _ADESK_H

#if defined(__cplusplus)
#include <cstdint> 
#else
#include <stdint.h>
#endif
#include <stddef.h>  


#define Adesk_Boolean_is_bool 1
#define Adesk_Int32_is_int 1


struct Adesk
{

    typedef int8_t      Int8;
    typedef int16_t     Int16;

    typedef uint8_t     UInt8;
    typedef uint16_t    UInt16;

    typedef int32_t     Int32;
    typedef uint32_t    UInt32;

    typedef int64_t     Int64;
    typedef uint64_t    UInt64;

    typedef unsigned char  uchar;
    typedef unsigned short ushort;
    typedef unsigned int   uint;

    typedef int64_t             LongPtr;
    typedef uint64_t            ULongPtr;
    typedef int64_t             IntPtr;
    typedef uint64_t            UIntPtr;

    typedef LongPtr IntDbId;
    typedef IntPtr GsMarker;

    // Logical type (Note: never use int when Boolean is intended!)
    // Please transition from Boolean type to native bool..
    //
    typedef bool       Boolean;
    static const bool kFalse = false;
    static const bool kTrue = true;

    using Time64 = int64_t;
};


#endif //_ADESK_H
