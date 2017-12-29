#define _GNU_SOURCE     /* needed to get the defines */

#include "messenger.h"
#include "DsManager.h" 
#include "aggregator.h"
#include "reportsGenerator.h"  
#include "config.h"
#include "logger.h"

#include <unistd.h> /* execv */
#include <sys/wait.h> /* waitpid */

#define SUBS_SIZE 1500000
#define OPR_SIZE 1000

/*******************************************************/
/* global */
Logger* logger;
/*******************************************************/

/*******************************************************/

static int InitializeForProcessor(Messenger** _messenger, Semaphore** _semaphore, DsManager** _dsManager, Config** _config);

/*******************************************************/
/*******************************************************/

int main()
{
	Aggregator* aggregator;
	DsManager* dsManager;
	Messenger* messenger;
	Semaphore* semaphore;
	Config* config;
	char *newargv[] = {UI_PATH, NULL};
	pid_t  pid;
	
	logger = LogOpen("logger/configLogger", "logger/logFileProcess");
		
	pid = fork();
	LOG_WRITE(logger, T, "fork processor process");
	
	if (pid == -1)
	{
		return -1;
	}
	else if (pid == 0)
	{
		/* start ui process */
		LOG_WRITE(logger, T, "start ui process");
		execv(*newargv, newargv);
		return -1;
	}
	else
	{
		if (InitializeForProcessor(&messenger, &semaphore, &dsManager, &config))
		{
			return -1;
		}
	
		aggregator = AggregatorsCreate(dsManager, messenger, GetProcThreads(config), semaphore);
		if (!aggregator)
		{
			return -1;
		}
			
		if (AggregatorJoin(aggregator))
		{
			return -1;
		}			
	
		/* generate reports before shutdown */
		if (AllSubscribersReport(dsManager))
		{
			return -1;
		}
	
		if (AllOperatorsReport(dsManager))
		{
			return -1;
		}
	
		AggregatorDestroy(aggregator);
		ConfigDestroy(config);
		
		if (-1 == waitpid(pid, NULL, 0))
		{
			return -1;
		}
	}
	
	LogClose(logger);
	
	return 0;
}

/*******************************************************/
static int InitializeForProcessor(Messenger** _messenger, Semaphore** _semaphore, DsManager** _dsManager, Config** _config)
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
	
	*_dsManager = DsManagerCreate(SUBS_SIZE, OPR_SIZE);
	if (! *_dsManager)
	{
		return -1;
	}
	
	/* get info from config file */
	*_config = ConfigCreate();
	if (! *_config)
	{
		return -1;
	}

	return 0;
}






