#define _GNU_SOURCE     /* needed to get the defines */
/** 
 * @brief parse manager. 
 * @created 01.11.17
 * @author Asa Schneider
 */
  
#include "aggregator.h"
#include "processorCommands.h"  

#include <stdlib.h> /* malloc */
#include <string.h> /* strcpy, strcat */
#include <pthread.h> /* pthread create */

#define AGGREGATOR_MAGIC 0xDeaDBeaF

/*****************************************************************************/

struct Aggregator
{
	size_t m_magic;
	DsManager* m_dsManager;
	Messenger* m_messenger;
	Semaphore* m_semaphore;
	pthread_t* m_threads;
	size_t m_numOfThreads;
};

/*****************************************************************************/

void* AggregatorFunc(void* _context);
static int EmptyQueue(Aggregator* _aggregator);
static int RecvCdrAndUpsert(Aggregator* _aggregator);

/*****************************************************************************/
                        /* Aggregators Create */
/*****************************************************************************/
Aggregator* AggregatorsCreate(DsManager* _dsManager, Messenger* _messenger, size_t _numOfThreads, Semaphore* _semaphore)
{
	Aggregator* aggregator;
	int status;
	int i;
	
	if (!_dsManager || !_numOfThreads)
	{
		return NULL;
	}
	
	aggregator = (Aggregator*)malloc(sizeof(Aggregator));
	if (! aggregator)
	{
		return NULL;
	}
	
	
	aggregator->m_threads = (pthread_t*)malloc(sizeof(pthread_t) * _numOfThreads);
	if (! aggregator->m_threads)
	{
		return NULL;
	}
	
	aggregator->m_dsManager = _dsManager;
	aggregator->m_messenger = _messenger;
	aggregator->m_numOfThreads = _numOfThreads;
	aggregator->m_semaphore = _semaphore;
	
	
	for (i = 0; i < _numOfThreads; ++i)
	{
		status = pthread_create(aggregator->m_threads + i, NULL ,AggregatorFunc, aggregator);
		if (-1 == status)
		{
			free(aggregator);
			return NULL;
		}
	}
	
	/*AggregatorFunc((void*)aggregator);*/
	
	aggregator->m_magic = AGGREGATOR_MAGIC;	
	
	return aggregator;	
}

/*****************************************************************************/
                        /* Aggregator Destroy */
/*****************************************************************************/
void AggregatorDestroy(Aggregator* _aggregator)
{		
	if (!_aggregator)
	{
		return;
	}
	
	_aggregator->m_magic = 0;
	DsManagerDestroy(_aggregator->m_dsManager);
	free(_aggregator->m_threads);
	free(_aggregator);
}

/*****************************************************************************/
                        /* Aggregator Join */
/*****************************************************************************/
int AggregatorJoin(Aggregator* _aggregator)
{
	int i;
		
	if (!_aggregator)
	{
		return -1;
	}
	
	/* join aggregator threads */
	for (i = 0; i < _aggregator->m_numOfThreads; ++i)
	{
		if (pthread_join(*(_aggregator->m_threads + i), NULL))
		{
			return -1;
		}
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Aggregator Func */
/*****************************************************************************/
void* AggregatorFunc(void* _context)
{
	Aggregator* aggregator = _context;
	void* command;
	int flagNoMsg = 0;
	
	while (! SemaphoreIsShutDown(aggregator->m_semaphore))
	{
		
		command = GetCommandMessage(aggregator->m_messenger, &flagNoMsg);
		if (command)
		{
			if (ActivateCommand(command, aggregator->m_dsManager, aggregator->m_semaphore))
			{
				return NULL;
			}
		}
		else if (! flagNoMsg) /* if error on allocation */
		{
			return NULL;
		}
		
		if (SemaphorePauseIfNeeded(aggregator->m_semaphore))
		{
			return NULL;
		}
		
		if (RecvCdrAndUpsert(aggregator))
		{
			return NULL;
		}
	}
	
	/* empty queue before exit */
	if (EmptyQueue(aggregator))
	{
		return NULL;
	}
	
	return NULL;
}

/*****************************************************************************/
static int EmptyQueue(Aggregator* _aggregator)
{
	int flagNoMsg = 0;
	int val;
	
	while(0 < (val = SemaphoreCounterVal(_aggregator->m_semaphore)))
	{
		if (RecvCdrAndUpsert(_aggregator))
		{
			return -1;
		}
	}
	
	while (! IsQueueEmpty(_aggregator->m_messenger))
	{
		if (RecvCdrAndUpsert(_aggregator))
		{
			return -1;
		}
		
		GetCommandMessage(_aggregator->m_messenger, &flagNoMsg);
	}
	
	return 0;
}

/*****************************************************************************/
static int RecvCdrAndUpsert(Aggregator* _aggregator)
{
	void* cdr;
	int flagNoMsg = 0;
	
	cdr = GetCdrMessage(_aggregator->m_messenger, &flagNoMsg);
	if (cdr)
	{
		if (Upsert(_aggregator->m_dsManager, cdr))
		{
			return -1;
		}			
	}
	else if (! flagNoMsg) /* if error on allocation */
	{
		return -1;
	}
	
	return 0;
}







