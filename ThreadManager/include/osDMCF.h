// Encapsulation for DMCF OS API.

#ifndef _OS_DMCF_H_
#define _OS_DMCF_H_

#include "singleton/Singleton.h"
#include "loger/loger_definition.h"
#include <list>
#include <limits.h>

struct dmcf_threadinfo_t;
class dmcfOSFactory;
class dmcfOSMutex;
class dmcfOSQueue;
class dmcfOSSemaphore;
class dmcfOSThread;


/**
* Basic type of DMCS definition
*/
typedef unsigned short u16;
typedef unsigned long int u32;

typedef enum EThreadPriority
{
    EPriority_Low = 0,
    EPriority_High
}EThreadPriority;

const u32 THREAD_STACK_MIN_SUPPORT = PTHREAD_STACK_MIN;

/**
* Mutex:
* For counting semaphore
*/
typedef struct DMCF_Sem_t
{
    	void* sem;
	u16 count;
	u32 owner_uid;
	u32 priority;
}DMCF_Sem;

namespace OS_TOOLS
{
    u32 OSGetCurrentThreadId();
};  // TOOLS

class dmcfOSFactory
{
public:
    dmcfOSFactory();
    ~dmcfOSFactory();

    dmcfOSMutex* createDMCFOSMutex();
    dmcfOSSemaphore* createDMCFOSSemphore(u32 initValue = 0);
    dmcfOSThread* createDMCFOSThread(dmcf_threadinfo_t* threadinfo = NULL);
    dmcfOSQueue* createDMCFOSQueue();
};

class dmcfOSMutex
{
public:
	dmcfOSMutex();
	~dmcfOSMutex();

	void lock();
	void unlock();

private:
	u16 count_;
	u32 curThread_;
       Loger log_;
       dmcfOSSemaphore* sem_;
};

class dmcfOSSemaphore
{
public:
	dmcfOSSemaphore(u32 initValue = 0);
	~dmcfOSSemaphore();

	void signal();
	bool wait(u32 timeout = -1);
private:
    bool createSem(u32 initValue);
    void postSem();
    void waitSem();
    void destroySem();

private:
    DMCF_Sem* sem_;
    Loger loger_;
};

typedef void (*CALLBACK)(void* param);

struct dmcf_threadinfo_t
{
    char* name;
    u32 priority;
    u32 stacksize;
    CALLBACK cb;
    void* param;
    dmcfOSSemaphore* sem;  // Don't fill this field
};

class dmcfOSThread
{
public:
    dmcfOSThread();
    dmcfOSThread(dmcf_threadinfo_t* threadinfo);
    ~dmcfOSThread();

    u32 getThreadId() const;
    void signal() const;
    void join() ;//const;
    bool stop() const;

protected:
    void initThread(dmcf_threadinfo_t* threadinfo);

private:
    Loger loger_;
    u32 hThread_;
    dmcfOSSemaphore* sem_;
};

#include "Guard.h"
#include "Locker.h"
class dmcfOSQueue
{
public:
    dmcfOSQueue(dmcfOSThread* threadHandle = NULL);
    ~dmcfOSQueue();

    void put(void* param);

    bool isEmpty() const;

    void* takeMessage();
    void setOSThreadHandle(dmcfOSThread* threadHandle);

private:
    dmcfOSThread* osThread_;
    std::list<void*> message_;
    Loger loger_;
    Locker lock_;
};

// singletion services to all caller
#define OSFactoryInstatnce Singleton<dmcfOSFactory>::getPtr()

#endif // _OS_DMCF_H_