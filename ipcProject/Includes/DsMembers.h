#ifndef __DS_MEMBERS_H__
#define __DS_MEMBERS_H__

/** 
 * @brief functions to manage subscribers and operators data structures. 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include <stdio.h>	/* FILE* */
#include "protocol.h"

typedef struct Subscriber Subscriber;

typedef struct Operator Operator;


/**  
 * @brief Dynamically create a new Subscriber.
 * @param[in] _cdr - parsed cdr.  
 * @return Subscriber* - on success / NULL on fail. 
 */
Subscriber* CreateSubscriber(Cdr* _cdr);

/**  
 * @brief Dynamically deallocate a previously allocated Subscriber .  
 * @param[in] _subscriber - subscriber to be deallocated.
 */
void SubscriberDestroy(Subscriber* _subscriber);

/**  
 * @brief Dynamically create a new Operator.
 * @param[in] _cdr - parsed cdr.    
 * @return Operator* - on success / NULL on fail. 
 */
Operator* CreateOperator(Cdr* _cdr);

/**  
 * @brief Dynamically deallocate a previously allocated Operator .  
 * @param[in] _operator - operator to be deallocated.
 */
void OperatorDestroy(Operator* _operator);

/**  
 * @brief Get a subscriber key.
 * @param[in] _cdr - parsed cdr.    
 * @return char* - on success / NULL on fail. 
 */
char* GetSubsKey(Cdr* _cdr);

/**  
 * @brief Get a operator key.
 * @param[in] _cdr - parsed cdr.    
 * @return char* - on success / NULL on fail. 
 */
char* GetOperKey(Cdr* _cdr);

/**  
 * @brief Prints Subscriber info line into file.
 * 
 * @param[in] _file - file to write into.
 * @param[in] _subs - Subscriber information to be printed,
 */
void PrintSubsToFile(FILE* _file, Subscriber* _subs);

/**  
 * @brief Prints Operator info line into file.
 * 
 * @param[in] _file - file to write into.
 * @param[in] _opr - Operator information to be printed,
 */
void PrintOprToFile(FILE* _file, Operator* _opr);

/**  
 * @brief update old aggregate value of subscriber .
 * 
 * @param[in] _oldValue - old value.
 * @param[in] _value - new value to be added.
 */
void UpdateSubs(void* _oldValue, void* _value);

/**  
 * @brief update old aggregate value of operator .
 * 
 * @param[in] _oldValue - old value.
 * @param[in] _value - new value to be added.
 */
void UpdateOprs(void* _oldValue, void* _value);

/**  
 * @brief zero subscriber members.
 * 
 * @param[in] _subs - Subscriber information.
 */
void ZeroSubs(Subscriber* _subs);

/**  
 * @brief zero operator members.
 * 
 * @param[in] _opr - Operator information.
 */
void ZeroOpr(Operator* _opr);


#endif /* __DS_MEMBERS_H__ */











