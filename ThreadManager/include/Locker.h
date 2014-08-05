#ifndef _LOCKER_H_
#define _LOCKER_H_

#include "osDMCF.h"
#include <memory>

// Locker with counting semaphore of DMCF Mutex 
class Locker
{
public:
    Locker() : mutexPtr_(new dmcfOSMutex()), loger_("Locker")
    {
        loger_ << debug << "Locker()...";
    }
    ~Locker()
    {
        loger_ << debug << "UnLocker()...";
    }

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
    std::auto_ptr<dmcfOSMutex> mutexPtr_;
    Loger loger_;
};

#endif // _LOCKER_H_