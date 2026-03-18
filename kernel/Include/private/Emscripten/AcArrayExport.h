#ifndef ACARRAYEXPORT_H
#define ACARRAYEXPORT_H



#if EMSDK


#define ACARRAY_PTR_EMSDK_EXPORT_METHODS(CLASSNAME,BASECLASS)\
    .function("at",select_overload<BASECLASS&(unsigned int)>(&CLASSNAME::at), allow_raw_pointers())\
    .function("length", &CLASSNAME::length)\
    .function("remove", &CLASSNAME::remove, allow_raw_pointers())\
    .function("append", &CLASSNAME::append, allow_raw_pointers())\
    .function("removeAt", &CLASSNAME::removeAt)\
    .function("removeAll", &CLASSNAME::removeAll)\


#define ACARRAY_EMSDK_EXPORT_METHODS(CLASSNAME,BASECLASS)\
    .function("at",select_overload<const BASECLASS(unsigned int)const>(&CLASSNAME::at))\
    .function("at",select_overload<BASECLASS&(unsigned int)>(&CLASSNAME::at))\
    .function("length", &CLASSNAME::length)\
    .function("remove", &CLASSNAME::remove)\
    .function("append", &CLASSNAME::append)\
    .function("removeAt", &CLASSNAME::removeAt)\
    .function("removeAll", &CLASSNAME::removeAll)\



#endif



#endif