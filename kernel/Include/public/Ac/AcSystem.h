#ifndef ACSYSTEM_H
#define ACSYSTEM_H


#include "AcString.h"
#include "AcArray.h"

class AcFile {
public:
	enum ECode
	{
		utf8,
		binary,
	};

	AcFile() {
		this->size = 0;
		this->lastModified = 0;
		this->data = NULL;
	}

	AcString name;
	unsigned int size;
	unsigned long long lastModified;
	const char *data;
};


class FTimerAssociation;
class AcSystem {
public:
	AcSystem() {}
	virtual ~AcSystem() {}


public:

	/** 定时器 */
	static unsigned int setInterval(void(*fun)(), unsigned int time);
	static unsigned int setInterval(void(*fun)(void*), unsigned int time, void* param);
	static unsigned int setTimeout(void(*fun)(), unsigned int time);
	static unsigned int setTimeout(void(*fun)(void*), unsigned int time, void* param);
	static void clearInterval(unsigned int id);
	static void clearTimeout(unsigned int id);

	/** 休眠 */
	static void sleep(unsigned int time);

	/** 选择并加载文件 */
	static void selectLoadFile(const AcString& accept, AcArray<AcFile*>& files, const AcFile::ECode code = AcFile::ECode::utf8);
};


#endif


