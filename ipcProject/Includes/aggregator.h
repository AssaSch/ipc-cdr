#ifndef __AGGREGATOR_H__
#define __AGGREGATOR_H__

/** 
 * @brief Aggregator API.
 *	
 * @details Get the parsed information from the Reader process
 * and insert it to Data Structre.
 * Performs commands from Ui process. 
 * 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include "DsManager.h"  
#include "messenger.h"
#include "semaphores.h"

typedef struct Aggregator Aggregator;

/** 
 * @brief Create a new Aggregator.
 * @param[in] _dsManager - Data Structre for storing subscribers and operators information. 
 * @param[in] _messenger - inter process message queue.
 * @param[in] _numOfThreads - number of threads for creation.
 * @param[in] _semaphore - semaphores for controling processes. 
 * @return newly created Aggregator or null on failure
 */
Aggregator* AggregatorsCreate(DsManager* _dsManager, Messenger* _messenger, size_t _numOfThreads, Semaphore* _semaphore);

/**
 * @brief destroy Aggregator.
 * @param[in] _aggregator - Aggregator to be destroyed.
 */
void AggregatorDestroy(Aggregator* _aggregator);

/**
 * @brief Join Aggregator threads.
 * @param[in] _aggregator - Aggregator to be joined.
 * @return 0 on success / -1 on failure.
 */
int AggregatorJoin(Aggregator* _aggregator);


#endif /* __AGGREGATOR_H__ */











