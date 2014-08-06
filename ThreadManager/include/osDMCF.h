// Encapsulation for DMCF OS API.

#ifndef _OS_DMCF_H_
#define _OS_DMCF_H_

#include "dmcfos.h"
#include "singleton/Singleton.h"
#include "loger/loger_definition.h"

class dmcfOSMutex
{
public:
	dmcfOSMutex();
	~dmcfOSMutex();

	void lock();
	void unlock();

protected:
	DMCF_Sem* sem_;
	u16 count_;
	u32 curThread_;
       Loger log_;
};

class dmcfOSSemaphore
{
public:
	dmcfOSSemaphore(u32 initValue = 0);
	~dmcfOSSemaphore();

	void signal();
	bool wait(u32 timeout = -1);

protected:
	DMCF_Sem* sem_;
};

class dmcfOSThread
{
public:
	dmcfOSThread();
	~dmcfOSThread();
};

class dmcfOSFactory
{
public:
	dmcfOSFactory();
	~dmcfOSFactory();

	dmcfOSMutex* createDMCFOSMutex();
	dmcfOSSemaphore* createDMCFOSSemphore(u32 initValue = 0);
	dmcfOSThread* createDMCFOSThread();
};

// singletion services to all caller
#define OSFactoryInstatnce Singleton<dmcfOSFactory>::getPtr()

#endif // _OS_DMCF_H_