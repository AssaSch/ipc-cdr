#include "HashMapCdr.h"
#include "genericList.h"
#include "genericList_itr.h"
#include "list_functions.h"
#include <stdlib.h> /* for malloc & NULL & free */
#include <stdio.h>
#include <pthread.h>
#include <math.h> /* sqrt */

#define HASH_MAGIC 0xDeaDBeaF
#define IS_HASH(N) ((N) && (N)->m_magic == HASH_MAGIC)
#define MUTEX_MODULE 2000

/*#define NDEBUG 1*/

/*****************************************************************************/
/*****************************************************************************/

/*functiond for generic hash map data structre */
/*created by Asa Schneider */
/* 17.09.2017 */

/*****************************************************************************/
/*****************************************************************************/

struct HashMap
{
	size_t m_magic;
	void** m_buckets;
	size_t m_size;
	size_t m_numOfPairs;
	pthread_mutex_t* m_mutexArr;
	HashFunction m_hashFunc;
	EqualityFunction m_keysEqualFunc;
	
	#ifndef NDEBUG
	Map_Stats* m_stats;
	#endif /* NDEBUG */ 
};

/*****************************************************************************/

typedef struct Pair
{
	void* m_key;
	void* m_value;
} Pair;

/*****************************************************************************/

static void InitializeHash(HashMap* _hash, size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc);
static Map_Result InsertPair(HashMap* _map, Pair* pair);
static Pair* CreatePair(void* _key, void* _value);
static int IsKeyInList(List* _list, const void* _searchKey, EqualityFunction _equality, ListItr* _pairItr, HashMap* _map);
int Predicate(void* _element, void* _auxpair);
static void ReleaseBuckets(HashMap* _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value));
static Map_Result NewBucketInsert(HashMap *_map, void** newBuckets, size_t _oldArrSize);
static void LockAllMutex(HashMap* _map);
static void UnlockAllMutex(HashMap* _map);

#ifndef NDEBUG
static void UpdateStatsInsert(HashMap* _map, size_t _arrIndex);
static void UpdateStatsRemove(HashMap* _map, size_t _arrIndex);
static void InitializeStats(Map_Stats* _stats);
#endif /* NDEBUG */ 

/*****************************************************************************/
                         /* HashMapCreate */
/*****************************************************************************/
HashMap* HashMapCreate(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
	HashMap* hash;
	
	if (!_capacity || !_hashFunc || !_keysEqualFunc)
	{
		return NULL;
	}
	
	hash = (HashMap*)malloc(sizeof(HashMap));
	if (!hash)
	{
		return NULL;
	}
	
	/* creation of List* only on insertion to hash */
	hash->m_buckets = (void**)calloc(_capacity,sizeof(void*));
	if (! hash->m_buckets)
	{
		free(hash);
		return NULL;
	}
	
	hash->m_mutexArr = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * (size_t)sqrt((double)_capacity)); /* (_capacity / MUTEX_MODULE));*/
	if (! hash->m_mutexArr)
	{	
		free(hash->m_buckets);
		free(hash);
		return NULL;
	}
	
	#ifndef NDEBUG
	hash->m_stats = (Map_Stats*)calloc(1,sizeof(Map_Stats));
	if (! hash->m_stats)
	{	
		free(hash->m_mutexArr);
		free(hash->m_buckets);
		free(hash);
		return NULL;
	}
	#endif /* NDEBUG */
	
	InitializeHash(hash, _capacity, _hashFunc, _keysEqualFunc); 
	
	return hash;
}

/*****************************************************************************/

static void InitializeHash(HashMap* _hash, size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
	int i;
	size_t mutexCapacity = (size_t)sqrt((double)_capacity); /*_capacity / MUTEX_MODULE;*/
	
	_hash->m_size = _capacity;	
	_hash->m_hashFunc = _hashFunc;
	_hash->m_keysEqualFunc = _keysEqualFunc;
	_hash->m_numOfPairs = 0;
	
	for (i = 0; i < mutexCapacity; ++i)
	{
		pthread_mutex_init(&(_hash->m_mutexArr[i]), NULL);	
	}
	
	_hash->m_magic = HASH_MAGIC;
}

/*****************************************************************************/
                      /* HashDestroy */
/*****************************************************************************/
void HashMapDestroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{	
	if (!_map || !(*_map))
	{
		return;
	}
	
	LockAllMutex(*_map);
	
	if ((*_map)->m_magic != HASH_MAGIC)
	{
		return;
	}
	
	(*_map)->m_magic = 0;
	
	UnlockAllMutex(*_map);
	
	ReleaseBuckets(*_map, _keyDestroy, _valDestroy);
	
	#ifndef NDEBUG
	free((*_map)->m_stats);
	#endif /* NDEBUG */
	
	free((*_map)->m_buckets);
	free(*_map);
	*_map = NULL;
	
}

/*****************************************************************************/

static void ReleaseBuckets(HashMap* _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{
	size_t arrSize = _map->m_size;
	size_t i;
	List** bucket = (List**)_map->m_buckets;
	Pair* element;
	
	for (i = 0; i < arrSize; ++i, ++bucket)
	{
		if (!bucket || !(*bucket))
		{
			continue;
		}
		
		while (!(ListIsEmpty(*bucket)))
		{
			if (!(ListPopHead(*bucket, (void**)&element)))
			{
				if (_valDestroy)
				{
					_valDestroy(element->m_value);
				}
				
				if (_keyDestroy)
				{
					_keyDestroy(element->m_key);
				}
			}
			free(element);
		}
		ListDestroy(bucket, NULL);
	}	
}

/*****************************************************************************/
                  /* HashMapInsert */
/*****************************************************************************/
Map_Result HashMapInsert(HashMap* _map, const void* _key, const void* _value)
{
	Pair* pair;
	ListItr pairItr;
	size_t arrIndex;
	
	if (!IS_HASH(_map))
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	if (!_key)
	{
		return MAP_KEY_NULL_ERROR;
	}
	
	arrIndex = ((_map->m_hashFunc)(_key) % _map->m_size);
	if (IsKeyInList(*(_map->m_buckets + arrIndex), _key, _map->m_keysEqualFunc, &pairItr, _map))
	{
		return MAP_KEY_DUPLICATE_ERROR;
	}
	
	if (!(pair = CreatePair((void*)_key, (void*)_value)))
	{
		return MAP_ALLOCATION_ERROR;
	}
	
	return InsertPair(_map, pair);
}

/*****************************************************************************/

static Pair* CreatePair(void* _key, void* _value)
{
	Pair* pair;
	
	pair = (Pair*)malloc(sizeof(Pair));
	if (!pair)
	{
		return NULL;
	}
	
	pair->m_key = _key;
	pair->m_value = _value;
	
	return pair;
}

/*****************************************************************************/

static Map_Result InsertPair(HashMap* _map, Pair* pair)
{
	size_t arrIndex;
	List_Result err;
	
	arrIndex = ((_map->m_hashFunc)(pair->m_key) % _map->m_size);
	
	err = ListPushHead((List*)(*(_map->m_buckets + arrIndex)), (void*)pair);
	
	/* if List not allready created in index, create one and insert pair */
	if (LIST_UNINITIALIZED_ERROR == err)
	{
		/* check for null on creation of list */
		if (! (*(_map->m_buckets + arrIndex) = ListCreate()))
		{
			return MAP_ALLOCATION_ERROR;
		}
		
		err = ListPushHead((List*)(*(_map->m_buckets + arrIndex)), (void*)pair);
		
		#ifndef NDEBUG
		++(_map->m_stats->buckets);
		++(_map->m_stats->chains);
		#endif /* NDEBUG */
	}
	
	if (LIST_ALLOCATION_ERROR == err)
	{
		return MAP_ALLOCATION_ERROR;
	}

	++_map->m_numOfPairs;
	
	#ifndef NDEBUG	
	UpdateStatsInsert(_map, arrIndex);
	#endif /* NDEBUG */
	
	return MAP_SUCCESS;
} 

/*****************************************************************************/

#ifndef NDEBUG	
static void UpdateStatsInsert(HashMap* _map, size_t _arrIndex)
{
	++(_map->m_stats->pairs);
	_map->m_stats->averageChainLength = _map->m_stats->pairs / _map->m_stats->chains;
	if (_map->m_stats->maxChainLength < ListSize(*(_map->m_buckets + _arrIndex)))
	{
		_map->m_stats->maxChainLength = ListSize(*(_map->m_buckets + _arrIndex));
	}
}
#endif /* NDEBUG */

/*****************************************************************************/
                  /* HashMapFind */
/*****************************************************************************/
Map_Result HashMapFind(const HashMap* _map, const void* _searchKey, void** _pValue)
{
	size_t arrIndex;
	ListItr pairItr;
	
	if (!IS_HASH(_map) || !_pValue)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	if (!_searchKey)
	{
		return MAP_KEY_NULL_ERROR;
	} 
	
	arrIndex = ((_map->m_hashFunc)(_searchKey) % _map->m_size);
		
	if (IsKeyInList(*(_map->m_buckets + arrIndex), _searchKey, _map->m_keysEqualFunc, &pairItr, (HashMap*)_map))
	{
		*_pValue = ((Pair*)ListItrGet(pairItr))->m_value;
		return MAP_SUCCESS;
	}
	
	return MAP_KEY_NOT_FOUND_ERROR;
}

/*****************************************************************************/

static int IsKeyInList(List* _list, const void* _searchKey, EqualityFunction _equality, ListItr* _pairItr, HashMap* _map)
{
	ListItr begin;
	ListItr end;
	ListItr curr;	
	
	if (!_list)
	{
		return 0;
	}
		
	begin = ListItrBegin(_list);
	end = ListItrEnd(_list); 
	for (curr = begin; !(ListItrEquals(curr, end)); curr = ListItrNext(curr))
	{
		/* equality function returns 0 if equal */
		if (!(_equality(((Pair*)ListItrGet(curr))->m_key, _searchKey)))
		{
			*_pairItr = curr;
			return 1;
		}
		
		#ifndef NDEBUG
		++_map->m_stats->collisions;
		#endif /* NDEBUG */
	}
	
	return 0;
}

/*****************************************************************************/
                  /* HashMapRemove */
/*****************************************************************************/
Map_Result HashMapRemove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue)
{
	ListItr pairItr;
	size_t arrIndex;
	Pair* pair;
			
	if (!IS_HASH(_map) || !_pValue || !_pKey)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	if (!_searchKey)
	{
		return MAP_KEY_NULL_ERROR;
	} 
	
	arrIndex = ((_map->m_hashFunc)(_searchKey) % _map->m_size);
		
	if (IsKeyInList(*(_map->m_buckets + arrIndex), _searchKey, _map->m_keysEqualFunc, &pairItr, _map))
	{
		/* save pair key and value and free pair */ 
		pair = ListItrRemove(pairItr);
		*_pKey = pair->m_key;
		*_pValue = pair->m_value;
		free(pair);

		--(_map->m_numOfPairs);
		#ifndef NDEBUG
		UpdateStatsRemove(_map, arrIndex);
		#endif /* NDEBUG */
		
		return MAP_SUCCESS;
	}
	
	return MAP_KEY_NOT_FOUND_ERROR;
}

/*****************************************************************************/

#ifndef NDEBUG
static void UpdateStatsRemove(HashMap* _map, size_t _arrIndex)
{
	size_t i;
	
	--(_map->m_stats->pairs);
	if (ListIsEmpty(*(_map->m_buckets + _arrIndex)))
	{
		--(_map->m_stats->chains);
	}
	
	if (_map->m_stats->chains)
	{
		_map->m_stats->averageChainLength = _map->m_stats->pairs / _map->m_stats->chains;
	}
	
	for (i = 0; i < _map->m_size; ++i)
	{
		if (!(*(_map->m_buckets + _arrIndex)))
		{
			continue;
		}
		
		if (_map->m_stats->maxChainLength < ListSize(*(_map->m_buckets + _arrIndex)))
		{
			_map->m_stats->maxChainLength = ListSize(*(_map->m_buckets + _arrIndex));
		}
	}
}
#endif /* NDEBUG */

/*****************************************************************************/
                  /* HashMapUpsert */
/*****************************************************************************/
int HashMapUpsert(HashMap* _map, void* _Key, void* _value, UpdateFunc _updateValue)
{
	void* oldValue;
	size_t arrIndex;
	size_t mutexNum;

	arrIndex = ((_map->m_hashFunc)(_Key) % _map->m_size);
	mutexNum = arrIndex % ((size_t)sqrt((double)_map->m_size)); /*_map->m_size / MUTEX_MODULE);*/
	
	pthread_mutex_lock(&(_map->m_mutexArr[mutexNum]));
	
	if (MAP_SUCCESS == HashMapFind(_map, _Key, &oldValue))
	{
		_updateValue(oldValue, _value);
		free(_value);
		free(_Key);
	}
	else
	{
		HashMapInsert(_map, _Key, _value);
	}

	pthread_mutex_unlock(&(_map->m_mutexArr[mutexNum]));
	
	return 0;	
}

/*****************************************************************************/

static void LockAllMutex(HashMap* _map)
{
	int i;
	size_t mutexCapacity = (size_t)sqrt((double)_map->m_size); /*_map->m_size / MUTEX_MODULE;*/
	
	for (i = 0; i < mutexCapacity; ++i)
	{
		pthread_mutex_lock(&(_map->m_mutexArr[i]));	
	}
}

/*****************************************************************************/

static void UnlockAllMutex(HashMap* _map)
{
	int i;
	size_t mutexCapacity = (size_t)sqrt((double)_map->m_size); /*_map->m_size / MUTEX_MODULE;*/
	
	for (i = 0; i < mutexCapacity; ++i)
	{
		pthread_mutex_unlock(&(_map->m_mutexArr[i]));	
	}
}

/*****************************************************************************/
                  /* HashMapSize */
/*****************************************************************************/

size_t HashMapSize(const HashMap* _map)
{
	return _map->m_numOfPairs;
}

/*****************************************************************************/
                   /* HashMapRehash */
/*****************************************************************************/
Map_Result HashMapRehash(HashMap *_map, size_t _newCapacity)
{
	void** newBuckets;
	size_t oldArrSize;
	Map_Result err;
	
	if (!IS_HASH(_map) || !_newCapacity)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	LockAllMutex(_map);
	
	newBuckets = (void**)calloc(_newCapacity,sizeof(void*));
	if (!(newBuckets))
	{
		return MAP_ALLOCATION_ERROR;
	}
	
	oldArrSize = _map->m_size;
	_map->m_size = _newCapacity;
	
	#ifndef NDEBUG
	InitializeStats(_map->m_stats);
	#endif /* NDEBUG */
	
	err = NewBucketInsert(_map, newBuckets, oldArrSize);
	
	UnlockAllMutex(_map);
	
	return err; /*NewBucketInsert(_map, newBuckets, oldArrSize);*/
}

/*****************************************************************************/

static Map_Result NewBucketInsert(HashMap *_map, void** newBuckets, size_t _oldArrSize)
{
	size_t i;
	List** bucket = (List**)_map->m_buckets;
	Pair* element;
	Map_Result err;
	
	_map->m_buckets = newBuckets; 
	
	for (i = 0; i < _oldArrSize; ++i, ++bucket)
	{
		if (!(*bucket))
		{
			continue;
		}
		
		while (!(ListIsEmpty(*bucket)))
		{
			/* pop pair from old array of lists and insert to new array */
			if (!(ListPopHead(*bucket, (void**)&element)))
			{	
				if (MAP_SUCCESS != (err = InsertPair(_map, element)))
				{
					/* user need to call HashMapDestroy if Rehash fails */
					return err;
				}
			}
		}
		ListDestroy(bucket, NULL);
	}
	free(bucket);
	
	return MAP_SUCCESS;
}

/*****************************************************************************/

#ifndef NDEBUG
static void InitializeStats(Map_Stats* _stats)
{
	_stats->pairs = 0;              
	_stats->collisions = 0;         
	_stats->buckets = 0;    
	_stats->chains = 0;              
	_stats->maxChainLength = 0 ;      
	_stats->averageChainLength = 0;
}
#endif /* NDEBUG */

/*****************************************************************************/
                   /* HashMapForEach */
/*****************************************************************************/
size_t HashMapForEach(const HashMap* _map, KeyValueActionFunction _action, void* _context)
{
	size_t actionCounter = 0;
	size_t i;
	ListItr begin;
	ListItr end;
	ListItr curr;
	List** bucket = (List**)_map->m_buckets;
	size_t ArrSize = _map->m_size;
	
	if (!IS_HASH(_map) || !_action)
	{
		return 0;
	}
	
	LockAllMutex((HashMap*)_map);
	
	for (i = 0; i < ArrSize; ++i, ++bucket)
	{
		if (!(*bucket))
		{
			continue;
		}
		
		begin = ListItrBegin(*bucket);
		end = ListItrEnd(*bucket); 
		for (curr = begin; !(ListItrEquals(curr, end)); curr = ListItrNext(curr))
		{
			if (!(_action(((Pair*)ListItrGet(curr))->m_key, ((Pair*)ListItrGet(curr))->m_value, _context)))
			{
				return actionCounter;
			}
			++actionCounter;
		}
	}
	
	UnlockAllMutex((HashMap*)_map);
	
	return actionCounter;
} 

/*****************************************************************************/
#ifndef NDEBUG
Map_Stats HashMapGetStatistics(const HashMap* _map)
{
	return *(_map->m_stats);
}
#endif /* NDEBUG */

/*****************************************************************************/

