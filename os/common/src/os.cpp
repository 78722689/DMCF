#include "os.h"
#include "loger/loger_definition.h"

#include <semaphore.h>
#include<pthread.h>
#include <stdlib.h>
#include <errno.h>

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
		tempMutex->mutex = malloc(sizeof(sem_t));
		if (NULL != tempMutex->mutex)
		{
			if (0 != sem_init((sem_t*)tempMutex->mutex, 0, initValue))
			{
				free(tempMutex->mutex);
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
		if (-1 != sem_destroy((sem_t*)(semPtr->mutex)))
		{
			free(semPtr->mutex);
			semPtr->mutex = NULL;
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
	return pthread_self();
}

void DMCF_OSWaitSemaphore(DMCF_Mutex *semPtr)
{
	// Wait till success
	while (-1 == sem_wait((sem_t*)semPtr->mutex))
	{
		loger << debug << "Wait sem failed with errno:" << errno << " continue to request the waiting process";
	}
}

void DMCF_OSPostSemaphore(DMCF_Mutex *semPtr)
{
	if (NULL != semPtr)
	{
		if ( -1 == sem_post((sem_t*)semPtr->mutex))
		{
			loger << debug << "Post sem failed with errno:" << errno;
		}

		loger << debug << "Post sem sucessfully!";
	}
}