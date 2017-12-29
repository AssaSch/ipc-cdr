#ifndef __PROCESSOR_COMMANDS_H__
#define __PROCESSOR_COMMANDS_H__

/** 
 * @brief handle massages and commands of processor. 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include "messenger.h"
#include "reportsGenerator.h" 
#include "semaphores.h"

#define PAUSE "1"
#define RESUME "2"
#define SHUT_DOWN "3"
#define MSISDN_QUERY "4"
#define OPERATOR_QUERY "5"
#define SUBSRIBERS_REPORT "6"
#define OPERATORS_REPORT "7"

#define ORDER_SIZE 20
#define KEY_SIZE 16

/*****************************************************************************/

typedef struct Command Command;

struct Command
{
	char m_order[ORDER_SIZE];
	char m_key[KEY_SIZE];
};


/*****************************************************************************/

/** 
 * @brief Activate report according to command.
 * @param[in] _command - command to perform.
 * @param[in] _dsManager - Data Structre for storing subscribers and operators information. 
 * @param[in] _semaphore - semaphores for controling processes. 
 * @return 0 for success / -1 for failure.
 */
int ActivateCommand(void* _command, DsManager* _dsManager, Semaphore* _semaphore);

/** 
 * @brief Receive command massage.
 * @param[in] _messenger - inter process message queue.
 * @param[in] _flagNoMsg -if function returns NULL and _flagNoMsg is 0 = error.
 * @return void* for success / NULL for failure.
 */
void* GetCommandMessage(Messenger* _messenger, int* _flagNoMsg);

/** 
 * @brief Receive cdr massage.
 * @param[in] _messenger - inter process message queue.
 * @param[in] _flagNoMsg -if function returns NULL and _flagNoMsg is 0 = error.
 * @return void* for success / NULL for failure.
 */
void* GetCdrMessage(Messenger* _messenger, int* _flagNoMsg);


#endif /* __PROCESSOR_COMMANDS_H__ */
