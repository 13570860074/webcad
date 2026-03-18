
#ifndef _acedads_h
#define _acedads_h 1



#include "adesk.h"
#include "adsdef.h"
#include "adscodes.h"
#include "acutmem.h"



/* AutoCAD Entity access routines */

int            acedEntSel(const ACHAR* str, ads_name entres,
    ads_point ptres);
int            acedNEntSel(const ACHAR* str, ads_name entres,
    ads_point ptres, ads_point xformres[4],
    struct resbuf** refstkres);
int            acedNEntSelP(const ACHAR* str, ads_name entres,
    ads_point ptres, int pickflag,
    ads_matrix xformres,
    struct resbuf** refstkres);
int            acedSSGet(const ACHAR* str, const void* pt1,
    const void* pt2, const struct resbuf* filter,
    ads_name ss);

int            acedSSGetFirst(struct resbuf** gset, struct resbuf** pset);
int            acedSSSetFirst(const ads_name pset, const ads_name unused);
int            acedSSFree(const ads_name sname);
#ifdef __cplusplus
int            acedSSLength(const ads_name sname, Adesk::Int32* len);
#else
int            acedSSLength(const ads_name sname, long* len);
#endif
int            acedSSAdd(const ads_name ename, const ads_name sname,
    ads_name result);
int            acedSSDel(const ads_name ename, const ads_name ss);
int            acedSSMemb(const ads_name ename, const ads_name ss);
int            acedSSName(const ads_name ss, int i, ads_name entres);
int            acedSSNameX(struct resbuf** rbpp, const ads_name ss,
    int i);
int            acedSSNameXEx(struct resbuf** rbpp, const ads_name ss,
    int i, unsigned int flags);

int            acedSSGetKwordCallbackPtr(struct resbuf* (**pFunc)(const ACHAR*));
int            acedSSSetKwordCallbackPtr(struct resbuf* (*pFunc)(const ACHAR*));
int            acedSSGetOtherCallbackPtr(struct resbuf* (**pFunc)(const ACHAR*));
int            acedSSSetOtherCallbackPtr(struct resbuf* (*pFunc)(const ACHAR*));

int            acedTrans(const ads_point pt, const struct resbuf* from,
    const struct resbuf* to, int disp,
    ads_point result);

    /* General AutoCAD utility routines */
int            acedSetVar (const ACHAR *sym, const struct resbuf *val);

/*
* val参数的取值(允许同时设置多个):
    1 RSG_NONULL 禁止空输入
    2 RSG_NOZERO 禁止输入0
    4 RSG_NONEG 禁止输入负数
    8 RSG_NOLIM 不检查绘图限制，即使LIMCHECK是打开的
    32 RSG_DASH 在画橡皮筋线或框时使用虚线
    64 RSG_2D 忽略3D点的Z坐标(仅acedGetDist())
    128 RSG_OTHER 允许任意输入——无论用户类型是什么
    256 RSG_DDISTFIRST 强制直接距离输入优先于任意输入
    512 RSG_TRACKUCS 允许UCS跟随面
    1024 RSG_NOORTHOZ 在Z方向禁用极性/otrack/ortho跟踪
    2048 RSG_NOOSNAP 禁用对象快照
*/
int            acedInitGet(int val, const ACHAR* kwl);

/* Functions that get system variables */
int            acedGetVar (const ACHAR *sym, struct resbuf *result);
#if EMSDK
const emsdk_resbuf* emsdk_acedGetVar(const std::string& sym);
int emsdk_acedSetVar(const std::string& sym, emsdk_resbuf* val);
#endif


/* Functions that get/set environment variables */
int            acedGetEnv (const ACHAR *sym, ACHAR  *var, size_t nBufLen);
int            acedSetEnv (const ACHAR *sym, const ACHAR *val);
#if EMSDK
const std::string emsdk_acedGetEnv (const std::string& sym);
int emsdk_acedSetEnv (const std::string& sym, const std::string& val);
#endif

/* Functions that get/set configurations  variables */
int            acedGetCfg (const ACHAR *sym, ACHAR  *var, size_t len);
int            acedSetCfg (const ACHAR *sym, const ACHAR *val);
#if EMSDK
const std::string emsdk_acedGetCfg (const std::string& sym);
int emsdk_acedSetCfg (const std::string& sym, const std::string& val);
#endif


/* Functions used to get user input  */
int            acedGetAngle(const ads_point pt, const ACHAR* prompt,
    ads_real* result);
int            acedGetCorner(const ads_point pt, const ACHAR* prompt,
    ads_point result);
int            acedGetDist(const ads_point pt, const ACHAR* prompt,
    ads_real* result);
int            acedGetOrient(const ads_point pt, const ACHAR* prompt,
    ads_real* result);
int            acedGetPoint(const ads_point pt, const ACHAR* prompt,
    ads_point result);
int            acedGetInt(const ACHAR* prompt, int* result);
int            acedGetKword(const ACHAR* prompt, ACHAR* result, size_t nBufLen);
int            acedGetReal(const ACHAR* prompt, ads_real* result);
int            acedGetInput(ACHAR* str, size_t nBufLen);

/* Graphics related functions */
int            acedRedraw(const ads_name ent, int mode);
int            acedGrDraw(const ads_point from, const ads_point to,
    int color, int hl);
int            acedGrVecs(const struct resbuf* vlist, ads_matrix mat);


/* Functions that pass AutoCAD a single string  */
int            acedPrompt(const ACHAR* str);
int            acedAlert(const ACHAR* str);

/*
* cronly:0??????????????,??0????????????
*/
/* Functions that get a string */
int            acedGetString(int cronly, const ACHAR* prompt, ACHAR*& result, unsigned int& bufsize);


int acedGetString(int cronly, const ACHAR* prompt, AcString& sResult);
int acedGetKword (const ACHAR *prompt, AcString & sResult);
int acedGetInput(AcString& sOut);


template<size_t nBufLen> inline int acedGetKword(const ACHAR* pszPrompt, ACHAR(&buf)[nBufLen])
{
    return ::acedGetKword(pszPrompt, buf, nBufLen);
}

// Deprecated. Please use acedGetInput(AcString &sOut) instead
inline int acedGetFullInput(ACHAR*& pStr)
{
    AcString sOut;
    const int nRet = ::acedGetInput(sOut);
    ::acutNewString(sOut.constPtr(), pStr);
    return nRet;
}

// Deprecated. Please use acedGetKword (const ACHAR *prompt, AcString & sResult) instead
inline int acedGetFullKword(const ACHAR* pString, ACHAR*& pStr)
{
    AcString sOut;
    const int nRet = ::acedGetKword(pString, sOut);
    ::acutNewString(sOut.constPtr(), pStr);
    return nRet;
}

// Deprecated. Please use acedGetString (int cronly, const ACHAR *prompt, AcString &sResult)
inline int acedGetFullString(int cronly, const ACHAR* pString, ACHAR*& pResult)
{
    AcString sResult;
    const int nRet = ::acedGetString(cronly, pString, sResult);
    ::acutNewString(sResult.constPtr(), pResult);
    return nRet;
}


#endif 
 
