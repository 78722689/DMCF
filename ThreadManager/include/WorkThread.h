#include "BaseThread.h"
#include "event/EventQueuePool.h"

class WorkThread : public BaseThread<EventQueuePool>
{
public:
    WorkThread(){}
    ~WorkThread(){}

    void init_thread(const char* name, void func(void*), void* param,  const long stackSize);

    static void* processEntry(void* param);
};