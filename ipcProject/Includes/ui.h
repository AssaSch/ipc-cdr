#ifndef __UI_H__
#define __UI_H__

/** 
 * @brief UI API. 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include "messenger.h"
#include "semaphores.h"

/** 
 * @brief Get user command from stdin. 
 * @return void pointer for success / NULL for failure.
 */
void* GetUserCommand();

/** 
 * @brief send command massage.
 * @param[in] _messenger - inter process message queue.
 * @param[in] _command - command to send.
 * @return 0 for success / -1 for failure.
 */
int SendCommandMsg(Messenger* _messenger, void* _command);

/** 
 * @brief Do system command according to command from user.
 * @param[in] _command - command to perform.
 * @param[in] _semaphore - semaphores for controling processes. 
 * @param[in] _flag - flag for shutdown mark. 
 * @return 0 for success / -1 for failure.
 */
int DoSystemCommand(void* _command, Semaphore* _semaphore, int* _flag);

#endif /* __UI_H__ */
