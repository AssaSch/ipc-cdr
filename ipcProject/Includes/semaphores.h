#ifndef __SEMAPHORES_H__
#define __SEMAPHORES_H__

/** 
 * @brief rapper functions for system V message queue API. 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */

#define SEM_KEY 376453201
#define PERMITIONS 0666
#define NUM_OF_SEM 3
#define PAUSE_SEM 0
#define SHUT_DOWN_SEM 1
#define COUNTER_SEM 2

/*****************************************************************************/

typedef struct Semaphore Semaphore;

/*****************************************************************************/

typedef struct sembuf sembuf;

/*****************************************************************************/

typedef union Semun
{
   int              val;    /* Value for SETVAL */
   struct semid_ds* buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short* array;  /* Array for GETALL, SETALL */
   struct seminfo* __buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
} Semun;

/*****************************************************************************/

/** 
 * @brief create Semaphore.
 * @return Semaphore* - on success / NULL on fail. 
 */
Semaphore* SemaphoreCreate();

/** 
 * @brief destroy Semaphore.
 * @param[in] _semaphore - Semaphore to be deallocated.
 */
void SemaphoreDestroy(Semaphore* _semaphore);

/** 
 * @brief change semaphore status to paused.
 * @param[in] _semaphore - Semaphore data structure.
 * @return 0 for success / -1 for failure.
 */
int SemaphorePauseThreads(Semaphore* _semaphore);

/** 
 * @brief change semaphore status to resume.
 * @param[in] _semaphore - Semaphore data structure.
 * @return 0 for success / -1 for failure.
 */
int SemaphoreResumeThreads(Semaphore* _semaphore);

/** 
 * @brief pause if in pause status.
 * @param[in] _semaphore - Semaphore data structure.
 * @return 0 for success / -1 for failure.
 */
int SemaphorePauseIfNeeded(Semaphore* _semaphore);

/** 
 * @brief change semaphore status to shut down.
 * @param[in] _semaphore - Semaphore data structure.
 * @return 0 for success / -1 for failure.
 */
int SemaphoreShutDown(Semaphore* _semaphore);

/** 
 * @brief pause if in pause status.
 * @param[in] _semaphore - Semaphore data structure.
 * @return 1 if shut down status is on / other if not.
 */
int SemaphoreIsShutDown(Semaphore* _semaphore);

/** 
 * @brief add 1 to counter.
 * @param[in] _semaphore - Semaphore data structure.
 * @return 0 for success / -1 for failure.
 */
int SemaphoreAddCounter(Semaphore* _semaphore);

/** 
 * @brief decrease 1 from counter.
 * @param[in] _semaphore - Semaphore data structure.
 * @return 0 for success / -1 for failure.
 */
int SemaphoreDecreaseCounter(Semaphore* _semaphore);

/** 
 * @brief wait until semaphore counter is zero.
 * @param[in] _semaphore - Semaphore data structure.
 * @return 0 for success / -1 for failure.
 */
int SemaphoreWaitUntilLast(Semaphore* _semaphore);

/** 
 * @brief initialize Semaphore.
 * @param[in] _semaphore - Semaphore data structure.
 * @return 0 for success / -1 for failure.
 */
int InitializeSemaphores(Semaphore* _semaphore);

/** 
 * @brief get semaphore counter value.
 * @param[in] _semaphore - Semaphore data structure.
 * @return semaphore counter value / -1 for failure.
 */
int SemaphoreCounterVal(Semaphore* _semaphore);


#endif /* __SEMAPHORES_H__ */
