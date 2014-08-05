#ifndef _DMCF_OS_H_
#define _DMCF_OS_H_

/**
* Basic type of DMCS definition
*/
typedef unsigned short u16;
typedef unsigned long int u32;


/**
* Mutex:
* For counting semaphore
*/
typedef struct DMCF_Sem_t
{
    	void* sem;
	u16 count;
	u32 owner_uid;
	u32 priority;
}DMCF_Sem;



bool DMCF_OSInit();
bool DMCF_OSCreateThread();


// begin semaphore
bool DMCF_OSCreateSem(DMCF_Sem** semPtr, u32 initValue);
bool DMCF_OSDestroySem(DMCF_Sem* semPtr);

u32 DMCF_OSGetCurrentThread();

void DMCF_OSWaitSem(DMCF_Sem *semPtr);

void DMCF_OSPostSem(DMCF_Sem *semPtr);

// end semaphore


#endif // _DMCF_OS_H_