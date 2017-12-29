#ifndef __REPORT_COMMANDS_H__
#define __REPORT_COMMANDS_H__

/** 
 * @brief Send commands from UI. 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */

/**  
 * @brief generate report of one subscriber.
 * @param[in] _msqId - message queue id.
 * @param[in] _key - key to get item.
 * @return 0 for success / -1 for failure.
 */
int MsisdnQueryCommand(int _msqId, char* _key);

/**  
 * @brief generate report of one operator.
 * @param[in] _msqId - message queue id.
 * @param[in] _key - key to get item.
 * @return 0 for success / -1 for failure.
 */
int Operator​QueryCommand(int _msqId, char* _key);

/**  
 * @brief generate report of all subscriber.
 * @param[in] _msqId - message queue id.
 * @param[in] _key - key to get item.
 * @return 0 for success / -1 for failure.
 */
int All​SubscribersCommand(int _msqId);

/**  
 * @brief generate report of all operator.
 * @param[in] _msqId - message queue id.
 * @param[in] _key - key to get item.
 * @return 0 for success / -1 for failure.
 */
int All​OperatorsCommand(int _msqId);


#endif /* __REPORT_COMMANDS_H__ */











