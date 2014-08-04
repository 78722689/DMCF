#ifndef _I_QUEUE_POOL_H_
#define _I_QUEUE_POOL_H_

// Buffer the tasks of one thread till it is idle.
class IQueuePool
{
public:
	virtual ~IQueuePool()
	{}
	
	virtual void put(void* param) = 0;

	virtual bool isEmpty() const = 0;

	virtual void* takeMessage() = 0;
};

#endif // _I_QUEUE_POOL_H_