#ifndef _EVENT_QUEUE_POOL_H_
#define _EVENT_QUEUE_POOL_H_

#include "IQueuePool.h"
#include "event/EventHandler.h"
#include "osDMCF.h"
#include "Guard.h"
#include "Locker.h"

// Decorator for Queue.
// A FIFO queue to buffer the tasks for thread
// Buffer the events of one thread till it is idle.
class EventQueuePool : public IQueuePool
{
public:
    EventQueuePool() : queue_(OSFactoryInstatnce->createDMCFOSQueue())
    {}
    ~EventQueuePool()
    {
        delete queue_;
    }

    void put(void* param)
    {
        Guard<Locker> guard(lock_);
        
        queue_->put(param);
    }

    bool isEmpty() const {return queue_->isEmpty();}

    void* takeMessage()
    {
        Guard<Locker> guard(lock_);
        
        return queue_->takeMessage();
    }

    void setOwnerThread(IThread* owner)
    {
        owner_ = owner;
        queue_->setOSThreadHandle(static_cast<dmcfOSThread*>(owner_->getOSThreadHandle()));
    }
private:
    //Queue queue_;
    dmcfOSQueue* queue_;
    IThread* owner_;
    Locker lock_;
};

#endif // _EVENT_QUEUE_POOL_H_