#include "arxHeaders.h"
#include "ReRendererManager.h"
#include "EdUserInputManager.h"
#include "EditorReactorManager.h"
#include "RxSystemVariable.h"
#include "RxEnvironmentVariable.h"
#include "RxConfigurationVariable.h"
#include "RxSystemVariableManager.h"
#include "RxEnvironmentVariableManager.h"
#include "RxConfigurationVariableManager.h"
#include "RxAsyncManager.h"
#include "DbObjectManager.h"
#include "acedTrans.h"
#include "adsdef.h"
#include "export.h"
#include "kernel.h"
#include "DbImpl.h"
#include "EdImpl.h"

void createSelectionSet(const DbObjectIdArray &selectionSets, ads_name ss)
{

}

int acedEntSel(const ACHAR *str, ads_name entres, ads_point ptres)
{

	return RTERROR;
}
int acedNEntSel(const ACHAR *str, ads_name entres, ads_point ptres, ads_point xformres[4], struct resbuf **refstkres)
{
	return RTNORM;
}
int acedNEntSelP(const ACHAR *str, ads_name entres, ads_point ptres, int pickflag,
				 ads_matrix xformres, struct resbuf **refstkres)
{
	return RTNORM;
}
int acedSSGet(const ACHAR *str, const void *pt1, const void *pt2, const struct resbuf *filter, ads_name ss)
{

	int error = RTNORM;

	return error;
}

int acedSSGetFirst(struct resbuf **gset, struct resbuf **pset)
{
	return RTNORM;
}
int acedSSSetFirst(const ads_name pset, const ads_name unused)
{
	return RTNORM;
}
int acedSSFree(const ads_name sname)
{

	return RTNORM;
}
#ifdef __cplusplus
int acedSSLength(const ads_name sname, Adesk::Int32 *len)
{

	return RTNORM;
}
#else
int acedSSLength(const ads_name sname, long *len)
{
	EdSelectionSetIndex *selectionSetIndex = ::kernel()->acedSelectionSetManager()->pushSelectionSetIndexAtIndex(sname[0]);
	if (selectionSetIndex == NULL)
	{
		len = 0;
	}
	else
	{
		*len = selectionSetIndex->length;
	}
	return RTNORM;
}
#endif
int acedSSAdd(const ads_name ename, const ads_name sname, ads_name result)
{
	return RTNORM;
}
int acedSSDel(const ads_name ename, const ads_name ss)
{
	return RTNORM;
}
int acedSSMemb(const ads_name ename, const ads_name ss)
{
	return RTNORM;
}
int acedSSName(const ads_name ss, int i, ads_name entres)
{
	return RTNORM;
}
int acedSSNameX(struct resbuf **rbpp, const ads_name ss, int i)
{
	return RTNORM;
}
int acedSSNameXEx(struct resbuf **rbpp, const ads_name ss, int i, unsigned int flags)
{
	return RTNORM;
}

int acedSSGetKwordCallbackPtr(struct resbuf *(**pFunc)(const ACHAR *))
{
	return RTNORM;
}
int acedSSSetKwordCallbackPtr(struct resbuf *(*pFunc)(const ACHAR *))
{
	return RTNORM;
}
int acedSSGetOtherCallbackPtr(struct resbuf *(**pFunc)(const ACHAR *))
{
	return RTNORM;
}
int acedSSSetOtherCallbackPtr(struct resbuf *(*pFunc)(const ACHAR *))
{
	return RTNORM;
}

int acedTrans(const ads_point pt, const struct resbuf *from, const struct resbuf *to, int disp, ads_point result)
{

	if (from->resval.rint < 0 || from->resval.rint > 4)
	{
		return RTERROR;
	}
	if (to->resval.rint < 0 || to->resval.rint > 3)
	{
		return RTERROR;
	}

	if (from->resval.rint == to->resval.rint)
	{
		result[0] = pt[0];
		result[1] = pt[1];
		result[2] = pt[2];
	}

	// wcs转ucs
	else if (from->resval.rint == 0 && to->resval.rint == 1)
	{

		GePoint3d pos = ::trans_wcs_to_ucs(GePoint3d(pt[0], pt[1], pt[2]));

		result[0] = pos.x;
		result[1] = pos.y;
		result[2] = pos.z;
	}
	// wcs转dcs
	else if (from->resval.rint == 0 && to->resval.rint == 2)
	{
		GePoint2d pos = ::trans_wcs_to_dcs(GePoint3d(pt[0], pt[1], pt[2]));

		result[0] = pos.x;
		result[1] = pos.y;
		result[2] = 0.0;
	}

	// ucs转wcs
	else if (from->resval.rint == 1 && to->resval.rint == 0)
	{

		GePoint3d pos = ::trans_ucs_to_wcs(GePoint3d(pt[0], pt[1], pt[2]));

		result[0] = pos.x;
		result[1] = pos.y;
		result[2] = pos.z;
	}
	// ucs转dc
	else if (from->resval.rint == 1 && to->resval.rint == 2)
	{
		GePoint2d pos = ::trans_ucs_to_dcs(GePoint3d(pt[0], pt[1], pt[2]));

		result[0] = pos.x;
		result[1] = pos.y;
		result[2] = 0.0;
	}

	// dc转wcs
	else if (from->resval.rint == 2 && to->resval.rint == 0)
	{
		GePoint3d pos = ::trans_dcs_to_wcs(GePoint2d(pt[0], pt[1]));

		result[0] = pos.x;
		result[1] = pos.y;
		result[2] = pos.z;
	}
	// dc转ucs
	else if (from->resval.rint == 2 && to->resval.rint == 1)
	{
		GePoint3d pos = ::trans_dcs_to_ucs(GePoint2d(pt[0], pt[1]));

		result[0] = pos.x;
		result[1] = pos.y;
		result[2] = pos.z;
	}

	return RTNORM;
}

int acedSetVar(const ACHAR *sym, const struct resbuf *val)
{
	RxSystemVariable *variable = ::kernel()->acrxSystemVariableManager()->get(sym);
	if (variable == NULL)
	{
		return RTERROR;
	}

	// 获得反应器
	AcArray<RxEventReactor *> editorReactors = *&ED_IMP_EDITTORREACTORMANAGER(EditorReactorManager::cast(acedEditor)->m_pImpl)->reactors;

	// 触发开始反应器
	for (int i = 0; i < editorReactors.length(); i++)
	{
		EditorReactor *reactor = EditorReactor::cast(editorReactors.at(i));
		if (reactor != NULL)
		{
			reactor->sysVarWillChange(sym);
		}
	}

	// 设置系统变量
	Acad::ErrorStatus err = variable->setVariable(val);

	// 触发完成反应器
	for (int i = 0; i < editorReactors.length(); i++)
	{
		EditorReactor *reactor = EditorReactor::cast(editorReactors.at(i));
		if (reactor != NULL)
		{
			if (err != Acad::eOk)
			{
				reactor->sysVarChanged(sym, false);
			}
			else
			{
				reactor->sysVarChanged(sym, true);
			}
		}
	}

	if (err != Acad::eOk)
	{
		return RTERROR;
	}

	return RTNORM;
}

int acedGetVar(const ACHAR *sym, struct resbuf *result)
{
	RxSystemVariable *variable = ::kernel()->acrxSystemVariableManager()->get(sym);
	if (variable == NULL)
	{
		return RTERROR;
	}
	variable->variable(result);
	return RTNORM;
}

#if EMSDK

const emsdk_resbuf *emsdk_acedGetVar(const std::string &sym)
{

	emsdk_resbuf *val = new emsdk_resbuf();

	AcString v;
	v.set(sym);
	resbuf result;
	acedGetVar(v, &result);
	val->rbnext = NULL;
	val->restype = result.restype;
	val->resval.rint = result.resval.rint;
	val->resval.rpoint_x = result.resval.rpoint[0];
	val->resval.rpoint_y = result.resval.rpoint[1];
	val->resval.rpoint_z = result.resval.rpoint[2];
	val->resval.rreal = result.resval.rreal;
	val->resval.rstring = result.resval.rstring;
	return val;
}
int emsdk_acedSetVar(const std::string &sym, emsdk_resbuf *val)
{
	resbuf result;
	result.restype = val->restype;
	result.rbnext = NULL;
	result.resval.rint = val->resval.rint;
	result.resval.rreal = val->resval.rreal;
	::acutNewString(val->resval.rstring.c_str(), result.resval.rstring);
	result.resval.rpoint[0] = val->resval.rpoint_x;
	result.resval.rpoint[1] = val->resval.rpoint_y;
	result.resval.rpoint[2] = val->resval.rpoint_z;
	return acedSetVar(sym.c_str(), &result);
}
#endif

/* Functions that get/set environment variables */
int acedGetEnv(const ACHAR *sym, ACHAR *var, size_t nBufLen)
{
	RxEnvironmentVariable *variable = ::kernel()->acrxEnvironmentVariableManager()->get(sym);
	if (variable == NULL)
	{
		return RTERROR;
	}
	AcString str = variable->variable();
	const unsigned int strLength = str.length();
	for (unsigned int i = 0; i < strLength; i++)
	{
		if (i >= nBufLen)
		{
			break;
		}
		var[i] = str.at(i);
		if (i == strLength - 1)
		{
			var[i + 1] = '\0';
			break;
		}
	}
	return RTNORM;
}
int acedSetEnv(const ACHAR *sym, const ACHAR *val)
{
	RxEnvironmentVariable *variable = ::kernel()->acrxEnvironmentVariableManager()->get(sym);
	if (variable != NULL)
	{
		variable->setVariable(val);
		return RTNORM;
	}
	AcString name = sym;
	if (name.length() == 0)
	{
		return RTERROR;
	}
	RxEnvironmentVariable *newVariable = new RxEnvironmentVariable();
	newVariable->setName(name);
	newVariable->setVariable(val);
	::kernel()->acrxEnvironmentVariableManager()->append(name, newVariable);
	return RTNORM;
}
#if EMSDK
const std::string emsdk_acedGetEnv(const std::string &sym)
{
	AcString name = sym.c_str();
	int len = 1024;
	ACHAR *var = new ACHAR[len];
	if (acedGetEnv(name.constPtr(), var, len) == RTNORM)
	{
		std::string v = var;
		delete[] var;
		return v;
	}
	return "";
}
int emsdk_acedSetEnv(const std::string &sym, const std::string &val)
{
	AcString v1 = sym.c_str();
	AcString v2 = val.c_str();
	return acedSetEnv(v1.constPtr(), v2.constPtr());
}
#endif

/* Functions that get/set configurations  variables */
int acedGetCfg(const ACHAR *sym, ACHAR *var, size_t len)
{
	RxConfigurationVariable *variable = ::kernel()->acrxConfigurationVariableManager()->get(sym);
	if (variable == NULL)
	{
		return RTERROR;
	}
	AcString str = variable->variable();
	const unsigned int strLength = str.length();
	for (unsigned int i = 0; i < strLength; i++)
	{
		if (i >= len)
		{
			break;
		}
		var[i] = str.at(i);
		if (i == strLength - 1)
		{
			var[i + 1] = '\0';
			break;
		}
	}
	return RTNORM;
}
int acedSetCfg(const ACHAR *sym, const ACHAR *val)
{
	RxConfigurationVariable *variable = ::kernel()->acrxConfigurationVariableManager()->get(sym);
	if (variable != NULL)
	{
		variable->setVariable(val);
		return RTNORM;
	}
	AcString name = sym;
	if (name.length() == 0)
	{
		return RTERROR;
	}
	RxConfigurationVariable *newVariable = new RxConfigurationVariable();
	newVariable->setName(name);
	newVariable->setVariable(val);
	::kernel()->acrxConfigurationVariableManager()->append(name, newVariable);
	return RTNORM;
}
#if EMSDK
const std::string emsdk_acedGetCfg(const std::string &sym)
{
	AcString name = sym.c_str();
	int len = 1024;
	ACHAR *var = new ACHAR[len];
	if (acedGetCfg(name.constPtr(), var, len) == RTNORM)
	{
		std::string v = var;
		delete[] var;
		return v;
	}
	return "";
}
int emsdk_acedSetCfg(const std::string &sym, const std::string &val)
{
	AcString v1 = sym.c_str();
	AcString v2 = val.c_str();
	return acedSetCfg(v1.constPtr(), v2.constPtr());
}
#endif

int acedInitGet(int val, const ACHAR *kwl)
{
	resbuf *var = acutNewRb(Db::kDxfXdAsciiString);
	::acutNewString(kwl, var->resval.rstring);
	acedSetVar("ADSJIGKWORD", var);
	acutRelRb(var);
	return RTNORM;
}

/**
 * 5100:拾取坐标或者输入坐标
 * -5005:输入关键字
 * -5002:esc按取消
 * 5000:直接空格或者回车
 */
/* Functions used to get user input  */
int acedGetAngle(const ads_point pt, const ACHAR *prompt, ads_real *result)
{
	int error = RTNONE;

	return error;
}
int acedGetCorner(const ads_point pt, const ACHAR *prompt, ads_point result)
{

	int error = RTNONE;

	return error;
}
int acedGetDist(const ads_point pt, const ACHAR *prompt, ads_real *result)
{

	int error = RTNONE;

	return error;
}
int acedGetOrient(const ads_point pt, const ACHAR *prompt, ads_real *result)
{
	return acedGetAngle(pt, prompt, result);
}
int acedGetPoint(const ads_point pt, const ACHAR *prompt, ads_point result)
{

	int error = RTNONE;


	return error;
}
int acedGetInt(const ACHAR *prompt, int *result)
{
	int error = RTNONE;

	return error;
}
int acedGetKword(const ACHAR *prompt, ACHAR *result, size_t nBufLen)
{
	int error = RTNONE;

	return error;
}
int acedGetReal(const ACHAR *prompt, ads_real *result)
{
	int error = RTNONE;

	return error;
}
int acedGetInput(ACHAR *str, size_t nBufLen)
{
	resbuf *var = acutNewRb(Db::kDxfXdAsciiString);
	acedGetVar("ADSJIGINPUT", var);
	Acad::ErrorStatus err = ::acutNewString(var->resval.rstring, str);
	acutRelRb(var);
	nBufLen = strlen(str);
	if (err == Acad::ErrorStatus::eOk)
	{
		return RTNORM;
	}
	return RTERROR;
}

/* Functions that pass AutoCAD a single string  */
int acedPrompt(const ACHAR *str)
{
	return RTNORM;
}
int acedAlert(const ACHAR *str)
{
	return RTNORM;
}

int acedRedraw(const ads_name ent, int mode)
{
	return RTNORM;
}
int acedGrDraw(const ads_point from, const ads_point to, int color, int hl)
{


	return RTNORM;
}
int acedGrVecs(const struct resbuf *vlist, ads_matrix mat)
{

	while (true)
	{
	}

	return 0;
}

/* Functions that get a string */
int acedGetString(int cronly, const ACHAR *prompt, ACHAR *&result, unsigned int &bufsize)
{
	int error = RTNONE;

	return error;
}

int acedGetString(int cronly, const ACHAR *prompt, AcString &sResult)
{
	ACHAR *result = NULL;
	unsigned int bufsize;
	int err = acedGetString(cronly, prompt, result, bufsize);
	if (err == RTNORM)
	{
		sResult = result;
		delete result;
		result = NULL;
	}
	return err;
}
int acedGetKword(const ACHAR *prompt, AcString &sResult)
{
	size_t sizeKey = 256;
	ACHAR *pInputKey = new ACHAR[sizeKey];
	int err = acedGetKword(prompt, pInputKey, sizeKey);
	if (err == RTNORM)
	{
		sResult = pInputKey;
		delete[] pInputKey;
	}
	return err;
}
int acedGetInput(AcString &sOut)
{
	resbuf var;
	int err = acedGetVar("ADSJIGINPUT", &var);
	if (err == RTNORM)
	{
		sOut = var.resval.rstring;
	}
	return err;
}
