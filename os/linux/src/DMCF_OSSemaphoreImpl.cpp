#include "dmcfos.h"
#include "definition.h"

#include <semaphore.h>
#include <errno.h>

bool DMCF_OSInit()
{
    return true;
}

bool DMCF_OSCreateThread()
{
    return true;
}

bool DMCF_OSCreateSem(DMCF_Sem** semPtr, u32 initValue)
{
	DMCF_Sem* tempMutex = NULL;
	bool ret = true;
	
	tempMutex = (DMCF_Sem*)malloc(sizeof(DMCF_Sem));
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

	loger << debug << "Sem created sucessfully!";
	return ret;
}

bool DMCF_OSDestroySem(DMCF_Sem* semPtr)
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

void DMCF_OSWaitSem(DMCF_Sem *semPtr)
{
	// Wait till success
	while (-1 == sem_wait((sem_t*)semPtr->sem))
	{
		loger << debug << "Wait sem failed with errno:" << errno << ", continue to request the waiting process";
	}
}

void DMCF_OSPostSem(DMCF_Sem *semPtr)
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