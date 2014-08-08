// Encapsulation for DMCF OS API.

#ifndef _OS_DMCF_H_
#define _OS_DMCF_H_

#include "dmcfos.h"
#include "singleton/Singleton.h"
#include "loger/loger_definition.h"
#include <list>

struct dmcf_threadinfo_t;
class dmcfOSFactory;
class dmcfOSMutex;
class dmcfOSQueue;
class dmcfOSSemaphore;
class dmcfOSThread;


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

typedef void (*CALLBACK)(void* param);

struct dmcf_threadinfo_t
{
    char* name;
    CALLBACK cb;
    void* param;
    //void * owner;
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

class dmcfOSQueue
{
public:
    dmcfOSQueue(dmcfOSThread* threadHandle = NULL);
    ~dmcfOSQueue();

    void put(void* param);

    bool isEmpty() const;

    void* takeMessage();
    void setOSThreadHandle(dmcfOSThread* threadHandle);

protected:
    dmcfOSThread* osThread_;
private:
    std::list<void*> message_;
    Loger loger_;
};

// singletion services to all caller
#define OSFactoryInstatnce Singleton<dmcfOSFactory>::getPtr()

#endif // _OS_DMCF_H_