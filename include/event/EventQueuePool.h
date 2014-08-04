#ifndef _EVENT_QUEUE_POOL_H_
#define _EVENT_QUEUE_POOL_H_

#include "IQueuePool.h"
#include "Queue.h"
#include "event/EventHandler.h"

// Decorator for Queue.
// A FIFO queue to buffer the tasks for thread
// Buffer the events of one thread till it is idle.
class EventQueuePool : public IQueuePool
{
public:
    EventQueuePool()
    {}
    ~EventQueuePool()
    {}

    void put(void* param){queue_.put(param);}

    bool isEmpty() const {return queue_.isEmpty();}

    void* takeMessage(){return queue_.takeMessage();}

private:
    Queue queue_;
};

#endif // _EVENT_QUEUE_POOL_H_