#define _GNU_SOURCE   /* needed to get the defines */

#include "fileManager.h"
#include "parseManager.h"
#include "config.h"
#include "genericList.h"
#include "logger.h"

#include <unistd.h> /* execv */
#include <sys/wait.h> /* waitpid */

/*******************************************************/

static int InitializeForReader(Messenger** _messenger, Semaphore** _semaphore, pthread_attr_t* _attr, Config** _config, List** _list);
static int DetectAndMoveFile(const char* _newFolder, Semaphore* _semaphore, char** _filePath, char* _currFolder);
static void DestroyThreads(List* _list);
static void ReaderDestroy(Messenger* _messenger, Semaphore* _semaphore, pthread_attr_t _attr, Config* _config, List* _list);

/*******************************************************/
/* global */
Logger* logger;
/*******************************************************/

int main()
{
	Parser* parser;
	Messenger* messenger;
	Semaphore* semaphore;
	Config* config;
	List* list;
	pthread_attr_t attr;
	char* filePath;
	char* newargv[] = {PROCESSOR_PATH, NULL};
	pid_t  pid;
	
	logger = LogOpen("logger/configLogger", "logger/logFileReader");
	
	pid = fork();
	LOG_WRITE(logger, T, "fork reader process");
	
	if (pid == -1)
	{
		return -1;
	}
	else if (pid == 0)
	{
		/* start processor process */
		LOG_WRITE(logger, T, "start processor process");
		execv(*newargv, newargv);
		LOG_WRITE(logger, F, "exec processor ERROR");
		return -1;
	}
	else
	{
		if (InitializeForReader(&messenger, &semaphore, &attr, &config, &list))
		{
			LOG_WRITE(logger, F, "initialize reader ERROR");
			return -1;
		}
	
		/* run until shutdown from UI */
		while (! SemaphoreIsShutDown(semaphore))
		{
			if (SemaphorePauseIfNeeded(semaphore))
			{
				LOG_WRITE(logger, F, "semaphore pause ERROR");
				return -1;
			}
		
			/* if shutdown happens while file detection */
			if (1 == DetectAndMoveFile(GetNewFileFolder(config), semaphore, &filePath, GetInWorkFolder(config)))
			{
				break;
			}
		
			parser = ParserCreate(messenger, filePath, semaphore, GetDoneFolder(config), attr);
			if (!parser)
			{
				LOG_WRITE(logger, F, "parser create ERROR");
				return -1;
			}
			
			ListPushHead(list, (void*)parser);
		}
	
		/* wait until all other threads finish */
		if (SemaphoreWaitUntilLast(semaphore))
		{
			LOG_WRITE(logger, F, "semaphore wait ERROR");
			return -1;
		}
	
		if (-1 == waitpid(pid, NULL, 0))
		{
			LOG_WRITE(logger, F, "wait pid ERROR");
			return -1;
		}
		
		ReaderDestroy(messenger, semaphore, attr, config, list);
	}
	
	LogClose(logger);
	
	return 0;	
}

/*******************************************************/
/*******************************************************/
static void ReaderDestroy(Messenger* _messenger, Semaphore* _semaphore, pthread_attr_t _attr, Config* _config, List* _list)
{
	ConfigDestroy(_config);
	MessengerDestroy(_messenger);
	SemaphoreDestroy(_semaphore);
	pthread_attr_destroy(&_attr);
	DestroyThreads(_list);
	ListDestroy(&_list,NULL);
}

/*******************************************************/
static int InitializeForReader(Messenger** _messenger, Semaphore** _semaphore, pthread_attr_t* _attr, Config** _config, List** _list)
{
	*_messenger = MessengerCreate();
	if (! *_messenger)
	{
		return -1;
	}
	
	*_semaphore = SemaphoreCreate();
	if (! *_semaphore)
	{
		return -1;
	}
	
	InitializeSemaphores(*_semaphore);
	
	/* get info from config file */
	*_config = ConfigCreate();
	if (! *_config)
	{
		return -1;
	}
	
	*_list = ListCreate();
	if (! *_list)
	{
		return -1;
	}
	
	pthread_attr_init(_attr);
	pthread_attr_setdetachstate(_attr, PTHREAD_CREATE_DETACHED);

	return 0;
}

/*******************************************************/
static int DetectAndMoveFile(const char* _newFolder, Semaphore* _semaphore, char** _filePath, char* _currFolder)
{
	char* fileName;
	
	if (1 == DetectFile(_newFolder, &fileName, _semaphore))
	{
		return 1;
	}
		
	*_filePath = GetFilePath(_newFolder, fileName);
	*_filePath = MoveFile(*_filePath, _currFolder);
	
	return 0;
}


/*******************************************************/
static void DestroyThreads(List* _list)
{
	Parser* parser;
	
	while (! ListIsEmpty(_list))
	{
		ListPopTail(_list, (void**)&parser);
		ParserDestroy(parser);
	}
}

/*******************************************************/

