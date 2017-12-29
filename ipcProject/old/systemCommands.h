#ifndef __SYSTEM_COMMANDS_H__
#define __SYSTEM_COMMANDS_H__

/** 
 * @brief Send commands from UI. 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */

/**  
 * @brief pause system.
 * @param[in] _msqId - message queue id.
 * @return 0 for success / -1 for failure.
 */
int PauseCommand(int _msqId);

/**  
 * @brief resume work.
 * @param[in] _msqId - message queue id.
 * @return 0 for success / -1 for failure.
 */
int ResumeCommand(int _msqId);

/**  
 * @brief do reports and shut down system.
 * @param[in] _msqId - message queue id.
 * @return 0 for success / -1 for failure.
 */
int ShutDownCommand(int _msqId);


#endif /* __SYSTEM_COMMANDS_H__ */











