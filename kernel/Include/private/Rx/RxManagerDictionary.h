#ifndef RXMANAGERDICTIONARY_H
#define RXMANAGERDICTIONARY_H

#include "RxManager.h"
#include "AcString.h"

struct RxMangerStub
{
	RxMangerStub()
	{
		this->pt = NULL;
		this->isInit = false;
		this->disabledInit = false;
	}

	bool isInit;
	bool disabledInit;
	RxManager *pt;
	std::function<RxManager *()> fun;
};

class RxManagerDictionary
{
public:
	RxManagerDictionary();
	virtual ~RxManagerDictionary();

private:
	AcArray<RxMangerStub *> *pManagers;

public:
	/* 实例化 */
	void instantiate();

	/* 查询 */
	RxManager *query(const AcString& name);

	/* 添加管理器 */
	void appendManager(RxMangerStub *manager);

	/* 设置禁用初始化 */
	void disabledInit(const AcString& name, const bool v);
};

#endif