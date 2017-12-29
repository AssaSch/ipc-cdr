#ifndef __PARSE_MANAGER_H__
#define __PARSE_MANAGER_H__

/** 
 * @brief Parse Manager API.
 *	
 * @details Parse lines from CDR file, insert to uniform protocol
 * and send to processor process.
 * 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include "semaphores.h"
#include "messenger.h"
#include <pthread.h>

typedef struct Parser Parser;

/** 
 * @brief Create a new Parser.
 * @param[in] _messenger - inter process message queue.
 * @param[in] _filePath - path of CDR file for reading. 
 * @param[in] _semaphore - semaphores for controling processes. 
 * @param[in] _endFolder - folder to save done files.
 * @param[in] _attr - pthread_attr_t structure.
 * @return newly created Parser or null on failure
 */
Parser* ParserCreate(Messenger* _messenger, const char* _filePath, Semaphore* _semaphore, const char* _endFolder, pthread_attr_t _attr);

/**
 * @brief destroy Parser.
 * @param[in] _parser - Parser to be destroyed.
 */
void ParserDestroy(Parser* _parser);


#endif /* __PARSE_MANAGER_H__ */











