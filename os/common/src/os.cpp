#include "os.h"
#include "loger/loger_definition.h"

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

Loger loger("DMCF_API");

bool DMCF_OSInit()
{
    return true;
}

bool DMCF_OSCreateThread()
{
    return true;
}

bool DMCF_OSCreateSem(void** semPtr, u32 initValue)
{
	DMCF_Mutex* tempMutex = NULL;
	bool ret = true;
	
	tempMutex = (DMCF_Mutex*)malloc(sizeof(DMCF_Mutex));
	if (NULL != tempMutex)
	{
		tempMutex->sem = malloc(sizeof(sem_t));
		if (NULL != tempMutex->sem)
		{
			if (0 != sem_init((sem_t*)tempMutex->sem, 0, initValue))
			{
				free(tempMutex->sem);
				free(tempMutex);
				*semPtr = NULL;
				ret = false;
				loger << debug << "Create sem failed with errno:" << errno;
			}
		}
	}

	*semPtr = tempMutex;
	
	loger << debug << "Mutex created sucessfully!";
	return ret;
}

bool DMCF_OSDestroySem(DMCF_Mutex* semPtr)
{
	bool ret = true;
	
	if (NULL != semPtr)
	{
		if (-1 != sem_destroy((sem_t*)(semPtr->sem)))
		{
			free(semPtr->sem);
			semPtr->sem = NULL;
		}
		else
		{
			ret = false;
			loger << debug << "Destroy sem failed with errno:" << errno;
		}
	}

	free(semPtr);
	semPtr = NULL;

	return ret;
}

u32 DMCF_OSGetCurrentThread()
{
	return syscall(SYS_gettid);
}

void DMCF_OSWaitSem(DMCF_Mutex *semPtr)
{
	// Wait till success
	while (-1 == sem_wait((sem_t*)semPtr->sem))
	{
		loger << debug << "Wait sem failed with errno:" << errno << " continue to request the waiting process";
	}
}

void DMCF_OSPostSem(DMCF_Mutex *semPtr)
{
	if (NULL != semPtr)
	{
		if ( -1 == sem_post((sem_t*)semPtr->sem))
		{
			loger << debug << "Post sem failed with errno:" << errno;
		}

		loger << debug << "Post sem sucessfully!";
	}
}