#ifndef _I_Thread_POOL_H_
#define _I_Thread_POOL_H_

#include "define/framework_dll_definition.h"

class IThreadPool
{
public:
	virtual ~IThreadPool()
	{}

	virtual bool CreateThreads(u16 threadNum ) = 0;
	virtual IThread* getThread() = 0;
	virtual bool stopThread(IThread* thread) = 0;
	virtual void  stopAllThread() = 0;
};

#endif // _I_Thread_POOL_H_