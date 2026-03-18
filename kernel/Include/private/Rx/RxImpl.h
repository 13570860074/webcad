#ifndef RXIMPL_H
#define RXIMPL_H

#include "AcArray.h"
#include "AcMap.h"
#include "AcEvent.h"
#include "AcEmsdkEvent.h"
#include "RxObject.h"
#include "RxObjectImpl.h"
#include "RxWorkerThread.h"
#include "RxSystemVariable.h"
#include "RxEnvironmentVariable.h"
#include "RxConfigurationVariable.h"
#include "RxDictionaryRecord.h"
#include <functional>
#include <mutex>
#include <thread>
#include <atomic>

#define RX_IMP_CLASS(x) ((RxClassImpl *)x)
#define RX_IMP_CLASSMANAGER(x) ((RxClassManagerImpl *)x)

#define RX_IMP_ITERATOR(x) ((RxIteratorImpl *)x)
#define RX_IMP_DICTIONARYITERATOR(x) ((RxDictionaryIteratorImpl *)x)

#define RX_IMP_ARXAPPMANAGER(x) ((RxArxAppManagerImpl *)x)

#define RX_IMP_DICTIONARY(x) ((RxDictionaryImpl *)x)
#define RX_IMP_SYSREGISTRYDICTIONARY(x) ((RxSysRegistryDictionaryImpl *)x)
#define RX_IMP_SYSREGISTRYDICTIONARYITERATOR(x) ((RxSysRegistryDictionaryIteratorImpl *)x)

#define RX_IMP_SYSTEMVARIABLEMANAGER(x) ((RxSystemVariableManagerImpl *)x)
#define RX_IMP_ENVIRONMENTVARIABLEMANAGER(x) ((RxEnvironmentVariableManagerImpl *)x)
#define RX_IMP_CONFIGURATIONVARIABLEMANAGER(x) ((RxConfigurationVariableManagerImpl *)x)

#define RX_IMP_PROPERTYREACTOR(x) ((RxPropertyReactorImpl *)x)
#define RX_IMP_PROPERTYMANAGER(x) ((RxPropertyManagerImpl *)x)

#define RX_IMP_CONTEXDOCUMMENT(x) ((RxContexDocumentImpl *)x)
#define RX_IMP_CONTEXMANAGER(x) ((RxContexManagertImpl *)x)

#define RX_IMP_FILESYSTEMMANAGER(x) ((RxFileSystemManagerImpl *)x)

#define RX_IMP_WORKERTHREAD(x) ((RxWorkerThreadImpl *)x)
#define RX_IMP_WORKERTHREADMANAGER(x) ((RxWorkerThreadManagerImpl *)x)

class RxClassImpl : public RxObjectImpl
{
public:
	RxClassImpl()
	{
		this->name = NULL;
	}
	virtual ~RxClassImpl()
	{
	}

	char *name;
	std::function<RxObject *()> function;
};
class RxClassManagerImpl : public RxObjectImpl
{
public:
	RxClassManagerImpl()
	{
	}
	virtual ~RxClassManagerImpl()
	{
	}

	static AcMap<AcString, RxClass *> classMaps;
	static bool isInitClass;
};



class RxObjectReactorImpl:public RxObjectImpl
{
public:
	RxObjectReactorImpl()
	{
		this->pEmsdkEvent = new AcEmsdkEvent();
	}
	virtual ~RxObjectReactorImpl()
	{
		delete this->pEmsdkEvent;
	}

	AcEmsdkEvent *pEmsdkEvent;
};


class RxFileSystemManagerImpl : public RxObjectImpl
{
public:
	RxFileSystemManagerImpl()
	{
		this->isInitFileSystemDone = false;
		this->isInitFileSystemEvent = false;
	}
	virtual ~RxFileSystemManagerImpl()
	{
	}

	bool isInitFileSystemDone;
	bool isInitFileSystemEvent;
};


class RxWorkerThreadTask;
class RxWorkerThreadImpl : public RxObjectImpl
{
public:
	RxWorkerThreadImpl()
	{
		this->id = 0;
		this->index = 0;
		this->thread = NULL;
		this->isLock.store(false);
		this->isThread = false;
		this->isClose.store(false);
	}
	virtual ~RxWorkerThreadImpl()
	{
	}

	std::mutex mutex;
	std::thread* thread;

	bool isThread;
	std::atomic<bool> isLock;
	unsigned int id;
	unsigned int index;
	std::atomic<bool> isClose;
	AcArray<RxWorkerThreadTask*> tasks;
};
class RxWorkerThreadManagerImpl : public RxObjectImpl
{
public:
	RxWorkerThreadManagerImpl()
	{
		this->bPause = false;
		this->numCores = 0;
		this->lastWorkerThread = NULL;
	}
	virtual ~RxWorkerThreadManagerImpl()
	{
	}

	bool bPause;
	unsigned char numCores;
	RxWorkerThread* lastWorkerThread;
	AcArray<RxWorkerThread*> workerThreads;
};


class RxIteratorImpl : public RxObjectImpl
{
public:
	RxIteratorImpl()
	{
		this->index = 0;
		this->pRecords = NULL;
	}
	virtual ~RxIteratorImpl()
	{
	}

	int index;
	AcArray<RxDictionaryRecord*>* pRecords;

	bool done() const
	{
		if (this->index < 0 || this->index > this->pRecords->length() - 1 || this->pRecords->length() <= 0)
		{
			return true;
		}
		return false;
	}
	bool next()
	{
		this->index = this->index + 1;
		return this->done();
	}
	RxObject* object() const
	{
		if (this->done() == true)
		{
			return NULL;
		}
		return this->pRecords->at(this->index)->pObject;
	}
	const char* key()
	{
		if (this->done() == true)
		{
			return NULL;
		}
		return this->pRecords->at(this->index)->key;
	}
	unsigned int id()
	{
		if (this->done() == true)
		{
			return NULL;
		}
		return this->pRecords->at(this->index)->id;
	}
};
class RxDictionaryIteratorImpl : public RxIteratorImpl
{
public:
	RxDictionaryIteratorImpl()
	{
	}
	virtual ~RxDictionaryIteratorImpl()
	{
	}
};
class RxDictionaryImpl : public RxObjectImpl
{
public:
	RxDictionaryImpl()
	{
		this->pRecords = new AcArray<RxDictionaryRecord*>();
	}
	virtual ~RxDictionaryImpl()
	{
		realloc_all(this->pRecords);
	}

	AcArray<RxDictionaryRecord*>* pRecords;
};
class RxSysRegistryDictionaryImpl : public RxDictionaryImpl
{
public:
	RxSysRegistryDictionaryImpl()
	{
	}
	virtual ~RxSysRegistryDictionaryImpl()
	{
	}
};
class RxSysRegistryDictionaryIteratorImpl : public RxDictionaryIteratorImpl
{
public:
	RxSysRegistryDictionaryIteratorImpl()
	{
	}
	virtual ~RxSysRegistryDictionaryIteratorImpl()
	{
	}
};



class RxSystemVariableManagerImpl : public RxObjectImpl
{
public:
	RxSystemVariableManagerImpl()
	{
	}
	virtual ~RxSystemVariableManagerImpl()
	{
	}
	AcMap<AcString, RxSystemVariable*> recordMaps;
};
class RxEnvironmentVariableManagerImpl : public RxObjectImpl
{
public:
	RxEnvironmentVariableManagerImpl()
	{
	}
	virtual ~RxEnvironmentVariableManagerImpl()
	{
	}
	AcMap<AcString, RxEnvironmentVariable*> recordMaps;
};
class RxConfigurationVariableManagerImpl : public RxObjectImpl
{
public:
	RxConfigurationVariableManagerImpl()
	{
	}
	virtual ~RxConfigurationVariableManagerImpl()
	{
	}
	AcMap<AcString, RxConfigurationVariable*> recordMaps;
};



#endif
