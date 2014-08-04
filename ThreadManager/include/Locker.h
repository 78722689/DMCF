#ifndef _LOCKER_H_
#define _LOCKER_H_

#include "os.h"

// Locker with counting semaphore of DMCS Mutex 
class Locker
{
public:
    Locker() : mutex_(0)
    {
    	initMutex();
    }
    ~Locker()
    {
    	if (NULL != mutex_)
    	{
            delete mutex_;
			mutex_ = NULL;
    	}
    }

public:
    void lock() const
	{
		u32 uid = DMCS_OSGetCurrentThread();

		if (uid != mutex_->uid)
		{
			DMCS_OSWaitSemaphore(mutex_);
			mutex_->uid = uid;
		}
		else
		{
			++mutex_->count; // safe in current thread for ++count
		}
	};
    void unlock() const
	{
		u32 uid = DMCS_OSGetCurrentThread()
		if (uid != mutex_->uid)
		{
			;
		}
		else
		{
			--mutex_->count;
		}
	};
private:
	void initMutex()
	{
		DMCS_OSCreateMutex(mutex_);
	}
private:
	DMCS_Mutex* mutex_;
};

#endif // _LOCKER_H_