#include "osDMCF.h"
#include <iostream>

dmcfOSMutex::dmcfOSMutex() : sem_(0), count_(0), curThread_(0), log_("osDMCF")
{
	DMCF_OSCreateSem(&sem_, 1);
}

dmcfOSMutex::~dmcfOSMutex()
{
	DMCF_OSDestroySem(sem_);
}

void dmcfOSMutex::lock()
{
    u32 uid = DMCF_OSGetCurrentThread();

   log_ << debug << "dmcfOSMutex::lock, uid=" << uid << " oldThread="<< curThread_;
   
    if (uid != curThread_)
    {   
       log_ << debug<< "dmcfOSMutex::lock, not current thread. waiting..........";
        DMCF_OSWaitSem(sem_); // Critical secion begin here.
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
    u32 uid = DMCF_OSGetCurrentThread();

    log_ << debug << "dmcfOSMutex::unlock, uid=" << uid << " oldThread="<< curThread_ ;
    
    if (uid == curThread_) // Only owner thread can unblock the locker.
    {
        log_ << debug<< "dmcfOSMutex::unlock, count=" << count_ ;
        if ( 0 == count_)
        {
            curThread_ = 0;
            DMCF_OSPostSem(sem_);
        }
        else
        {
            --count_;
        }
    }
}


dmcfOSSemaphore::dmcfOSSemaphore(u32 initValue/* = 0*/) : sem_(0)
{
	DMCF_OSCreateSem(&sem_, initValue);
}
dmcfOSSemaphore::~dmcfOSSemaphore()
{
	DMCF_OSDestroySem(sem_);
}

void dmcfOSSemaphore::signal()
{
	DMCF_OSPostSem(sem_);
}
bool dmcfOSSemaphore::wait(u32 timeout/* = -1*/)
{
	DMCF_OSWaitSem(sem_);
	
	return true;
}

dmcfOSThread::dmcfOSThread()
{}
dmcfOSThread::~dmcfOSThread()
{}

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
dmcfOSThread* dmcfOSFactory::createDMCFOSThread()
{
	return new dmcfOSThread();
}