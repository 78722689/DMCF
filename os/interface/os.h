#ifndef _OS_H_
#define _OS_H_

/**
* Basic type of DMCS definition
*/
typedef unsigned short u16;typedef unsigned long int u32;


/**
* Mutex:
* For counting semaphore
*/
typedef struct DMCS_Mutex_t
{
    void* mutex;
	u16 count;
	u32 uid;
}DMCS_Mutex;



bool DMCS_OSInit();
bool DMCS_OSCreateThread();

bool DMCS_OSCreateMutex(void* mutex);

u32 DMCS_OSGetCurrentThread();

void DMCS_OSWaitSemaphore(DMCS_Mutex *mutex);

void DMCS_OSPostSemaphore(DMCS_Mutex *mutex);
#endif // _I_OS_H_