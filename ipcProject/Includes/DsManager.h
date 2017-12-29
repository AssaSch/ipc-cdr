#ifndef __DS_MANAGER_H__
#define __DS_MANAGER_H__

/** 
 * @brief functions to manage two data structures. 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include "DsMembers.h"
#include "HashMapCdr.h"


typedef struct DsManager DsManager;

typedef enum Ds
{
	SUBS = 0,	/* subscribers */
	OPR			/* operators */
} Ds;


/**  
 * @brief Dynamically create a new DsManager.  
 * @return DsManager* - on success / NULL on fail. 
 */
DsManager* DsManagerCreate(size_t _subsCapacity, size_t _oprCapacity);

/**  
 * @brief Dynamically deallocate a previously allocated DsManager .  
 * @param[in] _dsManager - dsManager to be deallocated.
 */
void DsManagerDestroy(DsManager* _dsManager);

/**  
 * @brief Get a key-value pair from the hash map.
 * @param[in] _dsManager - dsManager pointer.
 * @param[in] _key - key to get item.
 * @param[in] _flag - flag to decide on which hash to act.
 * @return void* - pointer to struct of Subscribers or Operators on success / NULL on fail. 
 */
void* GetOneItem(DsManager* _dsManager, char* _key, Ds _flag);

/**  
 * @brief insert or update item in hash according to key.
 * @param[in] _dsManager - dsManager pointer.
 * @param[in] _cdr - key to get item.
 * @return 0 - on success / -1 on fail. 
 */
int Upsert(DsManager* _dsManager, void* _cdr);

/**  
 * @brief Iterate over all key-value pairs in the map and call a function for each pair
 * The user provided KeyValueActionFunction will be called for each element.  
 * Iteration will stop if the called function returns a zero for a given pair
 * 
 * @param[in] _dsManager - dsManager pointer
 * @param[in] _action - User provided function pointer to be invoked for each element
 * @param[in] _context - User provided function pointer to be invoked for each element
 * @param[in] _flag - flag to decide on which hash to act.
 * @returns number of times the user functions was invoked
 */
size_t ForEach(const DsManager* _dsManager, KeyValueActionFunction _action, void* _context, Ds _flag);


#endif /* __DS_MANAGER_H__ */











