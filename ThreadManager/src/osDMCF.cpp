#include "osDMCF.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <semaphore.h>

namespace OS_TOOLS
{
    u32 OSGetCurrentThreadId()
    {
        return syscall(SYS_gettid);
    }
};  // TOOLS


dmcfOSFactory::dmcfOSFactory()
{}
dmcfOSFactory::~dmcfOSFactory()
{}

dmcfOSMutex* dmcfOSFactory::createDMCFOSMutex()
{
	return new dmcfOSMutex();
}
dmcfOSSemaphore* dmcfOSFactory::createDMCFOSSemphore(u32 initValue/* = 0*/)
{
	return new dmcfOSSemaphore(initValue);
}
dmcfOSThread* dmcfOSFactory::createDMCFOSThread(dmcf_threadinfo_t* threadinfo/* = NULL*/)
{
	return new dmcfOSThread(threadinfo);
}

dmcfOSQueue* dmcfOSFactory::createDMCFOSQueue()
{
    return new dmcfOSQueue();
}

dmcfOSMutex::dmcfOSMutex() : sem_(0), count_(0), curThread_(0), log_("osDMCF")
{
    sem_ = OSFactoryInstatnce->createDMCFOSSemphore(1);
}

dmcfOSMutex::~dmcfOSMutex()
{
    delete sem_;
}

void dmcfOSMutex::lock()
{
    u32 uid = OS_TOOLS::OSGetCurrentThreadId();

   log_ << debug << "dmcfOSMutex::lock, uid=" << uid << " oldThread="<< curThread_;
   
    if (uid != curThread_)
    {   
        log_ << debug<< "dmcfOSMutex::lock, not current thread. waiting..........";
        sem_->wait(); // Critical section begin here.
        curThread_ = uid;
    }
    else
    {
        ++count_; // safe in current thread for ++count
        log_ << debug<< "dmcfOSMutex::lock, current thread. ++count=" << count_;
    }
}

void dmcfOSMutex::unlock()
{
    u32 uid = OS_TOOLS::OSGetCurrentThreadId();

    log_ << debug << "dmcfOSMutex::unlock, uid=" << uid << " oldThread="<< curThread_ ;
    
    if (uid == curThread_) // Only owner thread can unblock the locker.
    {
        log_ << debug<< "dmcfOSMutex::unlock, count=" << count_ ;
        if ( 0 == count_)
        {
            curThread_ = 0;
            sem_->signal();
        }
        else
        {
            --count_;
        }
    }
}


dmcfOSSemaphore::dmcfOSSemaphore(u32 initValue/* = 0*/) : sem_(0)
{
    createSem(initValue);
}
dmcfOSSemaphore::~dmcfOSSemaphore()
{
    destroySem();
}

void dmcfOSSemaphore::signal()
{
    postSem();
}
bool dmcfOSSemaphore::wait(u32 timeout/* = -1*/)
{
    waitSem();

    return true;
}

bool dmcfOSSemaphore::createSem( u32 initValue)
{
	bool ret = true;
	
	sem_ = (DMCF_Sem*)malloc(sizeof(DMCF_Sem));
	if (NULL != sem_)
	{
		sem_->sem = malloc(sizeof(sem_t));
		if (NULL != sem_->sem)
		{
			if (0 != sem_init((sem_t*)sem_->sem, 0, initValue))
			{
				free(sem_->sem);
				free(sem_);
				sem_ = NULL;
				ret = false;
				loger_ << debug << "Create sem failed with errno:" << errno;
			}
		}
	}

	loger_ << debug << "Sem created sucessfully!";
	return ret;
}

void dmcfOSSemaphore::postSem()
{
	if (NULL != sem_)
	{
		if ( -1 == sem_post((sem_t*)sem_->sem))
		{
			loger_ << debug << "Post sem failed with errno:" << errno;
		}

		loger_ << debug << "Post sem sucessfully!";
	}
}

void dmcfOSSemaphore::waitSem()
{
    // Wait till success
    while (-1 == sem_wait((sem_t*)sem_->sem))
    {
    	loger_ << debug << "Wait sem failed with errno:" << errno << ", continue to request the waiting process";
    }
}

void dmcfOSSemaphore::destroySem()
{
    if (NULL != sem_)
    {
        if (-1 != sem_destroy((sem_t*)(sem_->sem)))
        {
        	free(sem_->sem);
        	sem_->sem = NULL;
        }
        else
        {
        	loger_ << debug << "Destroy sem failed with errno:" << errno;
        }
    }

    free(sem_);
    sem_ = NULL;
}

#include <iostream> // testing include
static void* PROCESS(void* param)
{
    dmcf_threadinfo_t* threadinfo = static_cast<dmcf_threadinfo_t*>(param);
    
    while (true)
    {
        std::cout << "PROCESS: thread:"<< OS_TOOLS::OSGetCurrentThreadId()<< " Waiting........................."<<std::endl;
        threadinfo->sem->wait();
        std::cout << "PROCESS: thread:"<< OS_TOOLS::OSGetCurrentThreadId() << " received a semaphore...."<<std::endl;
        if (threadinfo->cb != (CALLBACK)0)
        {
            threadinfo->cb(threadinfo->param);
        }
        else // No callback, default process.
        {
            std::cout << " PROCESS: thread:"<< OS_TOOLS::OSGetCurrentThreadId() << " no callback." << std::endl;
        }
    }

    std::cout << " PROCESS: thread:"<< OS_TOOLS::OSGetCurrentThreadId() << " exit........................................" << std::endl;
}

// Nothing to do without thread information.
dmcfOSThread::dmcfOSThread()
{
}

dmcfOSThread::dmcfOSThread(dmcf_threadinfo_t* threadinfo) 
    : loger_("dmcfOSThread"),
    sem_(OSFactoryInstatnce->createDMCFOSSemphore())
{
    threadinfo->sem = sem_;
    initThread(threadinfo);
}
dmcfOSThread::~dmcfOSThread()
{
    // TODO: release the sem_ before deleting it.
    delete sem_;
}

void dmcfOSThread::initThread(dmcf_threadinfo_t* threadinfo)
{
    setStack(threadinfo->stacksize);
    setPriority(threadinfo->priority);
    
    if (pthread_create(&hThread_, NULL, PROCESS, (void*)threadinfo) != 0)
    {
        loger_<< debug << "DMCF_OSCreateThread: Create thread failed with errno:" << errno;
    }
    else
    {
        loger_ << debug << "DMCF_OSCreateThread: Create thread[" << hThread_ << "] sucessfully.";
    }
}

u32 dmcfOSThread::getThreadId() const
{
    return syscall(SYS_gettid);
}

void dmcfOSThread::signal() const
{
    sem_->signal();
}

void dmcfOSThread::join() //const
{
    loger_ << debug << "DMCF_OSCreateThread: Join thread[" << hThread_ << "] sucessfully.";
    
    if (pthread_join(hThread_, NULL) != 0)
    {
        loger_ << debug << "DMCF_OSCreateThread: Join thread[" << hThread_ << "]  NOT sucessfully.";
    }
}

bool dmcfOSThread::stop() const
{
    return pthread_cancel(hThread_);
}

void dmcfOSThread::setStack(u32 stacksize)
{
    u32 stack = THREAD_STACK_MIN_SUPPORT;

    if (stacksize <=0 || stacksize > THREAD_STACK_MAX_SUPPORT)
    {
        stack = THREAD_STACK_MAX_SUPPORT;
        loger_ << debug << "default stack size used.";
    }
    else if (stacksize > stack && stacksize <= THREAD_STACK_MAX_SUPPORT)
    {
        stack = stacksize;
    }
    else
    {
        loger_ << debug << "minior stack size used.";
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    if (0 != pthread_attr_setstacksize(&attr, stack))
    {
        loger_ << debug << "set stack size failed.";
    }
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

void dmcfOSThread::setPriority(u32 priority)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    if (0!= pthread_attr_setschedpolicy(&attr, SCHED_OTHER))
    {
        loger_ << debug << "set thread schedule policy failed.";
        return;
    }

    sched_param schedParam = {'\0'};
    u32 minPriority = sched_get_priority_min(SCHED_OTHER);
    u32 maxPriority = sched_get_priority_max(SCHED_OTHER);
    if (priority < minPriority || priority == EPriority_Low)
    {
        schedParam.sched_priority = minPriority;
        loger_ << debug <<  "Priority value is too low, set thread schedule priority with minor priority";
    }
    else if (priority > maxPriority || priority == EPriority_High)
    {
        schedParam.sched_priority = maxPriority;
         loger_ << debug <<  "Priority value is too high, set thread schedule priority with maximal priority";
    }
    else
    {
        schedParam.sched_priority = priority;
    }
    
    if (0 != pthread_attr_setschedparam(&attr, &schedParam))
    {
        loger_ << debug << "set thread priority failed.";
    }
}

dmcfOSQueue::dmcfOSQueue(dmcfOSThread* threadHandle/* = NULL*/)
    : osThread_(threadHandle)
    , loger_("dmcfOSQueue")
{
}

dmcfOSQueue::~dmcfOSQueue()
{
    // TODO: may you should release all the messages before release itself.
}

void dmcfOSQueue::put(void* param)
{
    Guard<Locker> guard(lock_);

    loger_ << debug << "queue::put()" ;    
    message_.push_back(param);

    // Take a message to thread for notifying the data ready.
    osThread_->signal();
}

bool dmcfOSQueue::isEmpty() const 
{
    return message_.empty();
}

void* dmcfOSQueue::takeMessage()
{
    Guard<Locker> guard(lock_);

    loger_ << debug  << "queue::takeMessage()" ;
    void *msg = message_.front();
    message_.pop_front(); // the message just be remove from list, it should be deleted after useage.

    return msg;
}

void dmcfOSQueue::setOSThreadHandle(dmcfOSThread* threadHandle)
{
    osThread_ = threadHandle;  
}