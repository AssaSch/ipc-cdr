#include "DsManager.h"  

/** 
 * @brief manage subscribers and operators data structures. 
 * @created 01.11.17
 * @author Asa Schneider
 */ 
#include "DsMembers.h"
#include <string.h> /* strcmp*/
#include <stdlib.h> /* strtol */
 
#define DS_MAGIC 0xDeaDBeaF
#define DECIMAL 10

/*****************************************************************************/

struct DsManager
{
	size_t m_magic;
	HashMap* m_subscribers;
	HashMap* m_operators;
};

/*****************************************************************************/

size_t HashFunc(const void* _key)
{	
	size_t sum = 0;
	char* str = (char*)_key;
	
	sum = (size_t)strtol(str, NULL, DECIMAL);
	
	return sum;
}

/*****************************************************************************/

int EqualityFunc(const void* _firstKey, const void* _secondKey)
{
	return strcmp((char*)_firstKey, ((char*)_secondKey));
}

/*****************************************************************************/
                        /* Create DsManager */
/*****************************************************************************/
DsManager* DsManagerCreate(size_t _subsCapacity, size_t _oprCapacity)
{
	DsManager* dsManager;
	
	if (!_subsCapacity || !_oprCapacity)
	{
		return NULL;
	}
	
	dsManager = (DsManager*)malloc(sizeof(DsManager));
	if (! dsManager)
	{
		return NULL;
	}
	
	dsManager->m_subscribers = HashMapCreate(_subsCapacity, HashFunc, EqualityFunc);
	if (! dsManager->m_subscribers)
	{
		free(dsManager);
		return NULL;
	}
	
	dsManager->m_operators = HashMapCreate(_oprCapacity, HashFunc, EqualityFunc);
	if (! dsManager->m_operators)
	{
		free(dsManager->m_subscribers);
		free(dsManager);
		return NULL;
	}
	
	dsManager->m_magic = DS_MAGIC;
	
	return dsManager;
}

/*****************************************************************************/
                        /* Destroy DsManager */
/*****************************************************************************/
void DsManagerDestroy(DsManager* _dsManager)
{

	_dsManager->m_magic = 0;
	HashMapDestroy(&_dsManager->m_subscribers, free, free);
	HashMapDestroy(&_dsManager->m_operators, free, free);
	free(_dsManager);
}

/*****************************************************************************/
                        /* GetOneItem */
/*****************************************************************************/
void* GetOneItem(DsManager* _dsManager, char* _searchKey, Ds _flag)
{
	void* value = NULL;
	
	if (SUBS == _flag)
	{
		HashMapFind(_dsManager->m_subscribers, _searchKey, &value);
	}
	else
	{
		HashMapFind(_dsManager->m_operators, _searchKey, &value);
	}
	
	return value;
}

/*****************************************************************************/
                        /* ForEach */
/*****************************************************************************/
size_t ForEach(const DsManager* _dsManager, KeyValueActionFunction _action, void* _context, Ds _flag)
{
	size_t numOfRuns;
	
	if (SUBS == _flag)
	{
		numOfRuns = HashMapForEach(_dsManager->m_subscribers, _action, _context);
	}
	else
	{
		numOfRuns = HashMapForEach(_dsManager->m_operators, _action, _context);
	}
	
	return numOfRuns;
}

/*****************************************************************************/
                        /* Upsert */
/*****************************************************************************/
int Upsert(DsManager* _dsManager, void* _data)
{
	Subscriber* subscriber;
	Operator* operator;
	char* subsKey;
	char* operKey;
	Cdr* cdr = _data;
	
	subscriber = CreateSubscriber(cdr);
	if (!subscriber)
	{
		return -1;
	}
	
	operator = CreateOperator(cdr);
	if (!operator)
	{
		free(subscriber);
		return -1;
	}
	
	subsKey = GetSubsKey(cdr);
	if (!subsKey)
	{
		free(operator);
		free(subscriber);
		return -1;
	}
	
	operKey = GetOperKey(cdr);
	if (!operKey)
	{
		free(subsKey);
		free(operator);
		free(subscriber);
		return -1;
	}
	
	HashMapUpsert(_dsManager->m_subscribers, subsKey, (void*)subscriber, UpdateSubs);
	HashMapUpsert(_dsManager->m_operators, operKey, (void*)operator, UpdateOprs);
	
	return 0;
}



