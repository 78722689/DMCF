#include "BaseThread.h"
#include "event/EventQueuePool.h"

class WorkThread : public BaseThread<EventQueuePool>
{
public:
   explicit WorkThread(u32 priority = EPriority_Low, u32 stacksize = THREAD_STACK_MIN_SUPPORT)
        : BaseThread("WorkThread", processEntry, this, priority, stacksize)
        , loger_("WorkThread")
    {
        
    }
    ~WorkThread(){}

    void init_thread(const char* name, void func(void*), void* param,  const long stackSize);

    void dispatch();
    
    static void processEntry(void* param);
private:
    Loger loger_;
};