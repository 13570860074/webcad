#ifndef DLL_H
#define DLL_H

#include <stdio.h>
#include "PlatformSettings.h"

#define IMPLEMENT_ARX_ENTRYPOINT(classname){classname *p = new classname();p->test();}



#if(__EMSCRIPTEN__)

#define EXPORT
#define STATIC_EXPORT

#include <emscripten/bind.h>
#include <emscripten/val.h>
using namespace emscripten;

#define EMSDK 1

#define em_protected public
#define em_public public

#else

#define WINDOWS 1

#define EXPORT __declspec(dllexport)
#define STATIC_EXPORT __declspec(dllexport)

#define em_protected protected
#define em_public public


#endif



#ifndef EM_PORT_API
#	if defined(__EMSCRIPTEN__)
#		include <emscripten.h>
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#		else
#			define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#		endif
#	else
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype
#		else
#			define EM_PORT_API(rettype) rettype
#		endif
#	endif
#endif


/** 在非emscripten环境下将以下emscripten编译器专用的宏定义无效化 */
#if !EMSDK
#define EM_JS(a,b,c,d) ;;;;
#define EM_ASYNC_JS(a,b,c,d) ;;;;
#define EMSCRIPTEN_KEEPALIVE();
#endif

/** emscripten环境下字符串,主要用于和H5端bind */
#define emstring std::string

/*
*
* 1:定时器模式
* 2:wasm中emscripten_set_main_loop产生的循环模式
* 3.手动定义的循环模式
/
/* 渲染模式 */
#define RENDERERMODE 3


#endif