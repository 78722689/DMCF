#ifndef _LOCKER_H_
#define _LOCKER_H_

#include "os.h"

// Locker with counting semaphore of DMCF Mutex 
class Locker
{
public:
    Locker() : mutex_(0)
    {
    	DMCF_OSCreateSem(&mutex_, 1);
    }
    ~Locker()
    {
    	DMCF_OSDestroySem(mutex_);
    }

public:
    	void lock() const
	{
		u32 uid = DMCF_OSGetCurrentThread();

		if (uid != mutex_->owner_uid)
		{
			DMCF_OSWaitSemaphore(mutex_);
			mutex_->owner_uid = uid;
		}
		else
		{
			++mutex_->count; // safe in current thread for ++count
		}
	}
    	void unlock() const
	{
		u32 uid = DMCF_OSGetCurrentThread()
		if (uid == mutex_->owner_uid) // Only owner thread can unblock the locker.
		{
			if ( 0 == mutex_->count)
			{
				mutex_->owner_uid = 0;
				DMCF_OSPostSemaphore(mutex_);
			}
			else
			{
				--mutex_->count;
			}
		}
	}

private:
	DMCF_Mutex* mutex_;
};

#endif // _LOCKER_H_