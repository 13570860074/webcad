#ifndef DEBUG_H
#define DEBUG_H


#include "AcString.h"

#define PRINT_FUNCTION_NAME\
	{ std::string funcName = __FUNCTION__;\
	std::cout << "function:" << funcName << std::endl; }\

#define PRINT_CODE_POSITION\
	{ std::string fileName = __FILE__;\
	int line = __LINE__;\
	std::string name;\
	for (int i = 0;i<fileName.length();i++){\
		if (fileName[i] == '\\' || fileName[i] == '/') {\
			name.clear();\
			continue;\
		}\
		name += fileName[i];\
	}\
	std::cout << "fileName:" << name << ",line:" << line << std::endl; }\


#define DEBUG_PRINT_FUNCTION_NAME\
	if (AcDebug::isPrintDebug() == true)\
	{\
		PRINT_FUNCTION_NAME;\
	}\

#define DEBUG_PRINT_CODE_POSITION\
	if (AcDebug::isPrintDebug() == true){\
		PRINT_CODE_POSITION;\
	}\

class GePoint2d;
class GePoint3d;
class GeVector2d;
class GeVector3d;
class GeMatrix2d;
class GeMatrix3d;
class GeLineSeg2d;
class GeLineSeg3d;
class GeRay2d;
class GeRay3d;
class GeLine2d;
class GeLine3d;
class AcDebug {

private:
	static bool isOpenDebug;

public:

	/** 是否打印 */
	static bool isPrintDebug();

	/** 开启debug模式 */
	static void openDebug();

	/** 关闭debug模式 */
	static void closeDebug();

	/** 程序开始标签(用于计算代码运行时间) */
	static void startTag();

	/** 程序结束标签(用于计算代码运行时间) */
	static unsigned int endTag(bool _isPrint = true);

	/** 打印(需要打开debug模式) */
	static void print_debug(int val);
	static void print_debug(double val);
	static void print_debug(const std::string& format);
	static void print_debug(const ACHAR* args, ...);

	/** 打印(不打开debug模式也会打印) */
	static void print(const std::string& format);
	static void print(const ACHAR* args, ...);
	static void print(int val, const AcString& prompt = "");
	static void print(double val, const AcString& prompt = "");
	static void print(const GePoint2d& pt, const AcString& prompt = "");
	static void print(const GePoint3d& pt, const AcString& prompt = "");
	static void print(const GeVector2d& ver, const AcString& prompt = "");
	static void print(const GeVector3d& ver, const AcString& prompt = "");
	static void print(const GeMatrix2d& ver, const AcString& prompt = "");
	static void print(const GeMatrix3d& ver, const AcString& prompt = "");
	static void print(const GeLineSeg2d& line, const AcString& prompt = "");
	static void print(const GeLineSeg3d& line, const AcString& prompt = "");
	static void print(const GeRay2d& line, const AcString& prompt = "");
	static void print(const GeRay3d& line, const AcString& prompt = "");
	static void print(const GeLine2d& line, const AcString& prompt = "");
	static void print(const GeLine3d& line, const AcString& prompt = "");
};




#endif
