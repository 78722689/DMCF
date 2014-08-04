#include "os.h"
#include <semaphore.h>
#include<pthread.h>

bool DMCS_OSInit()
{
    return true;
}

bool DMCS_OSCreateThread()
{
    return true;
}

bool DMCS_OSCreateMutex(void* mutex)
{

	return true;
}

u32 DMCS_OSGetCurrentThread()
{
	return pthread_self();
}

void DMCS_OSWaitSemaphore(DMCS_Mutex *mutex)
{

}

void DMCS_OSPostSemaphore(DMCS_Mutex *mutex)
{

}