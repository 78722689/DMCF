#include "BaseThread.h"
#include "event/EventQueuePool.h"

class WorkThread : public BaseThread<EventQueuePool>
{
public:
    WorkThread()
        : BaseThread("WorkThread", processEntry, this)
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