#define _GNU_SOURCE

/** 
 * @brief rapper functions for system V message queue API. 
 * @created 01.11.17
 * @author Asa Schneider
 */ 

#include "semaphores.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <string.h> /* strcpy, strcat */

#define SEMAPHORE_MAGIC 0xDeaDBeaF


struct Semaphore
{
	size_t m_magic;
	int m_semId;
};

/*****************************************************************************/
                        /* Static Functions */
/*****************************************************************************/



/*****************************************************************************/
                        /* Semaphore Create */
/*****************************************************************************/
Semaphore* SemaphoreCreate()
{
	Semaphore* semaphore;

	semaphore = (Semaphore*)malloc(sizeof(Semaphore));
	if (!semaphore)
	{
		return NULL;
	}
	
	semaphore->m_semId = semget(SEM_KEY, NUM_OF_SEM, PERMITIONS | IPC_CREAT);
	if (-1 == semaphore->m_semId) 
	{	
		free(semaphore);
		return NULL;
	}
	
	semaphore->m_magic = SEMAPHORE_MAGIC;
	
	return semaphore;
}

/*****************************************************************************/
                        /* Semaphore Destroy*/
/*****************************************************************************/
void SemaphoreDestroy(Semaphore* _semaphore)
{
	_semaphore->m_magic = 0;
	semctl(_semaphore->m_semId, PAUSE_SEM, IPC_RMID);
	semctl(_semaphore->m_semId, SHUT_DOWN_SEM, IPC_RMID);
	free(_semaphore);
}

/*****************************************************************************/
                        /* Semaphore Pause Threads */
/*****************************************************************************/
int InitializeSemaphores(Semaphore* _semaphore)
{
	Semun semun;
		
	semun.val = 0;
	if (-1 == semctl(_semaphore->m_semId, PAUSE_SEM, SETVAL, semun))
	{	
		semctl(_semaphore->m_semId, PAUSE_SEM, IPC_RMID);
		free(_semaphore);
		return -1;
	}
	
	if (-1 == semctl(_semaphore->m_semId, SHUT_DOWN_SEM, SETVAL, semun))
	{	
		semctl(_semaphore->m_semId, SHUT_DOWN_SEM, IPC_RMID);
		semctl(_semaphore->m_semId, PAUSE_SEM, IPC_RMID);
		free(_semaphore);
		return -1;
	}
	
	if (-1 == semctl(_semaphore->m_semId, COUNTER_SEM, SETVAL, semun))
	{	
		semctl(_semaphore->m_semId, COUNTER_SEM, IPC_RMID);
		semctl(_semaphore->m_semId, SHUT_DOWN_SEM, IPC_RMID);
		semctl(_semaphore->m_semId, PAUSE_SEM, IPC_RMID);
		free(_semaphore);
		return -1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Semaphore Pause Threads */
/*****************************************************************************/
int SemaphorePauseThreads(Semaphore* _semaphore)
{
	Semun semun;
	
	semun.val = 1;
	if (-1 == semctl(_semaphore->m_semId, PAUSE_SEM, SETVAL, semun))
	{	
		return -1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Semaphore Resume Threads */
/*****************************************************************************/
int SemaphoreResumeThreads(Semaphore* _semaphore)
{
	Semun semun;
	
	semun.val = 0;
	if (-1 == semctl(_semaphore->m_semId, PAUSE_SEM, SETVAL, semun))
	{	
		return -1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Semaphore Resume Threads */
/*****************************************************************************/
int SemaphorePauseIfNeeded(Semaphore* _semaphore)
{
	sembuf buf;
	
	buf.sem_num = PAUSE_SEM;
	buf.sem_op = 0;
	buf.sem_flg = 0;

	if (-1 == semop(_semaphore->m_semId, &buf, 1))
	{
		return -1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Semaphore ShutDown */
/*****************************************************************************/
int SemaphoreShutDown(Semaphore* _semaphore)
{
	Semun semun;
	
	semun.val = 1;
	if (-1 == semctl(_semaphore->m_semId, SHUT_DOWN_SEM, SETVAL, semun))
	{	
		return -1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Semaphore IsShutDown */
/*****************************************************************************/
int SemaphoreIsShutDown(Semaphore* _semaphore)
{
	Semun semun;
	
	return semctl(_semaphore->m_semId, SHUT_DOWN_SEM, GETVAL, semun);
}

/*****************************************************************************/
                        /* Semaphore IsShutDown */
/*****************************************************************************/
int SemaphoreAddCounter(Semaphore* _semaphore)
{
	sembuf buf;
	
	buf.sem_num = COUNTER_SEM;
	buf.sem_op = 1;
	buf.sem_flg = 0;

	if (-1 == semop(_semaphore->m_semId, &buf, 1))
	{
		return -1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Semaphore IsShutDown */
/*****************************************************************************/
int SemaphoreDecreaseCounter(Semaphore* _semaphore)
{
	sembuf buf;
	
	buf.sem_num = COUNTER_SEM;
	buf.sem_op = -1;
	buf.sem_flg = 0;

	if (-1 == semop(_semaphore->m_semId, &buf, 1))
	{
		return -1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Semaphore Resume Threads */
/*****************************************************************************/
int SemaphoreWaitUntilLast(Semaphore* _semaphore)
{
	sembuf buf;
	
	buf.sem_num = COUNTER_SEM;
	buf.sem_op = 0;
	buf.sem_flg = 0;

	if (-1 == semop(_semaphore->m_semId, &buf, 1))
	{
		return -1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Semaphore IsShutDown */
/*****************************************************************************/
int SemaphoreCounterVal(Semaphore* _semaphore)
{
	Semun semun;
	
	return semctl(_semaphore->m_semId, COUNTER_SEM, GETVAL, semun);
}














