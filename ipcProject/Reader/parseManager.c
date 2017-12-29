#define _GNU_SOURCE     /* needed to get the defines */
/** 
 * @brief parse manager. 
 * @created 01.11.17
 * @author Asa Schneider
 */
  
#include "parseManager.h"
#include "fileManager.h" 
#include "cdrParser.h" 
#include "messenger.h" 
#include "protocol.h"
#include "logger.h"

#include <pthread.h> /* pthread_create */
#include <stdlib.h> /* malloc */
#include <string.h> /* strcpy, strcat */

#define PARSER_MAGIC 0xDeaDBeaF

/*******************************************************/
/* global */
Logger* logger;
/*******************************************************/

/*****************************************************************************/

struct Parser
{
	size_t m_magic;
	Messenger* m_messenger;
	Semaphore* m_semaphore;
	char* m_filePath;
	char* m_endFolder;
	pthread_t m_thread;
	pthread_attr_t m_attr;
};

/*****************************************************************************/

void* parserFunc(void* _context);

/*****************************************************************************/
                        /* CreateParser */
/*****************************************************************************/
Parser* ParserCreate(Messenger* _messenger, const char* _filePath, Semaphore* _semaphore, const char* _endFolder, pthread_attr_t _attr)
{
	Parser* parser;
	int status;
	
	if (! _filePath)
	{
		return NULL;
	}
	
	parser = (Parser*)malloc(sizeof(Parser));
	if (! parser)
	{
		return NULL;
	}
	
	parser->m_filePath = (char*)malloc(sizeof(char) * strlen(_filePath));
	if (! parser->m_filePath)
	{
		free(parser);
		return NULL;
	}
	strcpy(parser->m_filePath, _filePath);
	free((void*)_filePath);
	
	parser->m_endFolder = (char*)malloc(sizeof(char) * strlen(_endFolder));
	if (! parser->m_endFolder)
	{
		free(parser->m_filePath);
		free(parser);
		return NULL;
	}
	strcpy(parser->m_endFolder, _endFolder);
	
	parser->m_messenger = _messenger;
	parser->m_semaphore = _semaphore;
	parser->m_attr = _attr;
	
	status = pthread_create(&(parser->m_thread), &_attr ,parserFunc, parser);
	if (status)
	{
		free(parser->m_endFolder);
		free(parser->m_filePath);
		free(parser);
		return NULL;
	}
	
	/*parserFunc((void*)parser);*/
	
	parser->m_magic = PARSER_MAGIC; 
	
	return parser;
	
}

/*****************************************************************************/
                        /* Parser Destroy */
/*****************************************************************************/
void ParserDestroy(Parser* _parser)
{
	if (!_parser)
	{
		return;
	}
	
	_parser->m_magic = 0;
	free(_parser->m_filePath);
	free(_parser->m_endFolder);
	free(_parser);
}

/*****************************************************************************/
                        /* parser Func */
/*****************************************************************************/
void* parserFunc(void* _context)
{
	FILE* file;
	char* line;
	void* cdr;
	Parser* parser = _context;

	if (SemaphoreAddCounter(parser->m_semaphore))
	{
		LOG_WRITE(logger, F, "parserFunc add counter ERROR");
		return NULL;
	}
	
	file = OpenFile(parser->m_filePath);
	if (!file)
	{
		LOG_WRITE(logger, F, "parserFunc open file ERROR");
		SemaphoreDecreaseCounter(parser->m_semaphore);
		return NULL;
	}
	
	/* get all lines from file */
	while ((line = GetLine(file)))
	{
		if (SemaphorePauseIfNeeded(parser->m_semaphore))
		{
			LOG_WRITE(logger, F, "parserFunc pause ERROR");
			SemaphoreDecreaseCounter(parser->m_semaphore);
			return NULL;
		}
		
		cdr = ParseCdr(line);
		if (!cdr)
		{
			LOG_WRITE(logger, F, "parserFunc parse cdr ERROR");
			SemaphoreDecreaseCounter(parser->m_semaphore);
			return NULL;
		}
		
		if (SendMessage(parser->m_messenger, 0, DATA_TYPE, cdr, sizeof(Cdr)))
		{
			LOG_WRITE(logger, F, "parserFunc send msg ERROR");
			SemaphoreDecreaseCounter(parser->m_semaphore);
			return NULL;
		}
	}
	
	free(MoveFile(parser->m_filePath, parser->m_endFolder));
	fclose(file);
	
	if (SemaphoreDecreaseCounter(parser->m_semaphore))
	{
		LOG_WRITE(logger, F, "parserFunc dec counter ERROR");
		return NULL;
	}

	LOG_WRITE(logger, T, "return parserFunc");
	return NULL;
}






