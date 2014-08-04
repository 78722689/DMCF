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
typedef struct DMCF_Mutex_t
{
    	void* mutex;
	u16 count;
	u32 owner_uid;
	u32 priority;
}DMCF_Mutex;



bool DMCF_OSInit();
bool DMCF_OSCreateThread();

bool DMCF_OSCreateSem(void** semPtr, u32 initValue);
bool DMCF_OSDestroySem(void* semPtr);

u32 DMCF_OSGetCurrentThread();

void DMCF_OSWaitSemaphore(DMCF_Mutex *semPtr);

void DMCF_OSPostSemaphore(DMCF_Mutex *semPtr);
#endif // _I_OS_H_