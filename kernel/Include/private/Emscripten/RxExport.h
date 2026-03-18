#ifndef RXEXPORT_H
#define RXEXPORT_H



#if EMSDK



#define RX_EMSDK_EXPORT_METHODS(CLASSNAME)\
	.class_function("cast", select_overload<CLASSNAME* (const RxObject*)>(&CLASSNAME::cast), allow_raw_pointers())\
	.class_function("cast", select_overload<CLASSNAME* (const unsigned int)>(&CLASSNAME::cast), allow_raw_pointers())\
    .function("isA", &CLASSNAME::isA, allow_raw_pointers())\
    .class_function("desc", &CLASSNAME::desc, allow_raw_pointers())\

#define EMSDK_CLASS_CONVERT_METHODS(CLASSNAME)\
	CLASSNAME emsdk_class_convert_##CLASSNAME(int inPtr) {\
		CLASSNAME* p = (CLASSNAME*)inPtr;\
		return *p;\
	}\

#define EMSDK_PTR_CLASS_CONVERT_METHODS(CLASSNAME)\
	CLASSNAME* emsdk_ptr_class_convert_##CLASSNAME(int inPtr) {\
		CLASSNAME* p = (CLASSNAME*)inPtr;\
		return p;\
	}\

#define EMSDK_CLASS_CONVERT_EXPORT(CLASSNAME)\
	function(STRING_MACRO(emsdk_class_convert_##CLASSNAME), &emsdk_class_convert_##CLASSNAME);

#define EMSDK_PTR_CLASS_CONVERT_EXPORT(CLASSNAME)\
	function(STRING_MACRO(emsdk_ptr_class_convert_##CLASSNAME), &emsdk_ptr_class_convert_##CLASSNAME, allow_raw_pointers());

#endif


#endif