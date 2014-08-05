#ifndef _LOCKER_H_
#define _LOCKER_H_

#include "dmcfos.h"
#include <memory>

// Locker with counting semaphore of DMCF Mutex 
class Locker
{
public:
    Locker() : mutexPtr_(new dmcfOSMutex())
    {}
    ~Locker()
    {}

public:
    	void lock() const
	{
		mutexPtr_->lock();
	}
    	void unlock() const
	{
		mutexPtr_->unlock();
	}

private:
	std::shared_ptr<dmcfOSMutex> mutexPtr_;
};

#endif // _LOCKER_H_