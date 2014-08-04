#ifndef _PTHREAD_POOL_H_
#define _PTHREAD_POOL_H_

#include <vector>
#include "singleton/Singleton.h"
#include "IThread.h"
#include "IThreadPool.h"
#include "WorkThread.h"

const u16 DEFAULT_THREAD_NUM = 12;

template<class T>
class ThreadPool : public IThreadPool
{
public:
    ThreadPool()
    {}
    ~ThreadPool()
    {
        stopAllThread();
    }

    // Create threads according to DEFAULT_THREAD_NUM
    bool CreateThreads(u16 threadNum = DEFAULT_THREAD_NUM)
    {
        for (int i = 0; i < threadNum; ++i)
        {
            IThread *thread = new Thread_Base_Type;
            BaseThreadVec_.push_back(thread);
        }

        return true;
    }

    // Get idle thread
    IThread* getThread()
    {
        for (std::vector< IThread* >::iterator it = BaseThreadVec_.begin();
        it != BaseThreadVec_.end(); ++it)
        {
            if ((*it)->getThreadState() == EUninitilization)
            {
                return *it;
            }
        }
        return NULL;
    }

    // Stop thread
    inline bool stopThread(IThread* thread) {return thread->stop();}

    inline void  stopAllThread()
    {
        for (std::vector<IThread* >::iterator it = BaseThreadVec_.begin();
            it != BaseThreadVec_.end(); ++it)
        {
            (*it)->stop();
        }
    }

    inline void joinThreads()
    {
        for (std::vector<IThread* >::iterator it = BaseThreadVec_.begin();
            it != BaseThreadVec_.end(); ++it)
        {
            if ((*it)->getThreadState() == EBusy)
            {
                (*it)->joinThread();
            }
        }
    }
private:
    typedef T Thread_Base_Type;
    std::vector<IThread* > BaseThreadVec_;
};

// Singleton for ThreadPool instance.
typedef Singleton<ThreadPool<WorkThread> > ThreadPool_Instance;
#define threadPool ThreadPool_Instance::getPtr()

#endif // _PTHREAD_POOL_H_