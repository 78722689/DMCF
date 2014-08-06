#include "dmcfos.h"
#include "definition.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

static void* process(void* param)
{
    while (true)
    {
        ;
    }
}
void DMCF_OSPutMessage(void* data)
{

}

bool DMCF_OSCreateThread1(u32& hThread, void callback(void*), void* param)
{

}


bool DMCF_OSCreateThread(u32& hThread, void*process(void*), void* param)
{
    bool ret = true;
    if (pthread_create(&hThread, NULL, process, param) != 0)
    {
        ret = false;
        loger << debug << "DMCF_OSCreateThread: Create thread failed with errno:" << errno;
    }

    return ret;
}

u32 DMCF_OSGetCurrentThread()
{
    return syscall(SYS_gettid);
}

void DMCF_OSJoinThread(u32 hThread)
{
    pthread_join(hThread, NULL);
}

bool DMCF_OSCancelThread(u32 hThread)
{
    return pthread_cancel(hThread);
}