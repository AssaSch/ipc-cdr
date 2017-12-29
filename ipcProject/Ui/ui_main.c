#define _GNU_SOURCE   /* needed to get the defines */

#include "ui.h"
#include <stdio.h>

/*******************************************************/

static void PrintManu();
static int InitializeForUi(Messenger** _messenger, Semaphore** _semaphore);

/*******************************************************/
/*******************************************************/

int main()
{
	Messenger* messenger;
	Semaphore* semaphore;
	void* command;
	int flag = 0;
	
	if (InitializeForUi(&messenger, &semaphore))
	{
		return -1;
	}
	
	while (1)
	{
		PrintManu();
		command = GetUserCommand();
		
		if (DoSystemCommand(command, semaphore, &flag))
		{
			return -1;
		}
		if (SendCommandMsg(messenger, command))
		{
			return -1;
		}
		
		if (flag)
		{
			break;
		}
	}
	
	return 0;
}

/*******************************************************/
static void PrintManu()
{
	printf("choose option:\n\n");
	printf("pause: 1\n");
	printf("resume: 2\n");
	printf("shutdown: 3\n");
	printf("msisdn query: 4 and msisdn number\n");
	printf("operator query: 5 and mcc number\n");
	printf("subscribers report: 6\n");
	printf("operators report: 7\n\n");
	
}

/*******************************************************/
static int InitializeForUi(Messenger** _messenger, Semaphore** _semaphore)
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

	return 0;
}

/*******************************************************/




