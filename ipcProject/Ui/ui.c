#define _GNU_SOURCE

/** 
 * @brief UI API. 
 * @created 01.11.17
 * @author Asa Schneider
 */ 

#include "ui.h"
#include "processorCommands.h" 

#include <sys/msg.h> /* massage queue */
#include <stdlib.h> /* malloc */
#include <string.h> /* strcpy, strtok */

#define MAX_LINE 50

/*****************************************************************************/

static void RemoveEnterSign(char* _str);

/*****************************************************************************/
                        /* Get User Command */
/*****************************************************************************/
void* GetUserCommand()
{
	char line[MAX_LINE];
	Command* command;
	char* token;
	
	printf("print your command: ");
	fgets(line, MAX_LINE, stdin);
	
	command = malloc(sizeof(Command));
	if (!command)
	{
		return NULL;
	}
	
	/* order */
	token = strtok(line, " ");
	strcpy(command->m_order, token);
	RemoveEnterSign(command->m_order);
	
	/* key */	
	token = strtok(NULL, " ");
	if (token)
	{
		strcpy(command->m_key, token);
		RemoveEnterSign(command->m_key);	
	}
	
	return command;
}

/*****************************************************************************/
                        /* Get User Command */
/*****************************************************************************/
int SendCommandMsg(Messenger* _messenger, void* _command)
{
	if (!_command)
	{
		return -1;	
	}
	
	if (SendMessage(_messenger, IPC_NOWAIT, UI_TYPE, _command, sizeof(Command)))
	{
		return -1;
	}
	
	return 0;
}

/*****************************************************************************/

static void RemoveEnterSign(char* _str)
{
	if ((strlen(_str) > 0) && (_str[strlen(_str) - 1] == '\n'))
	{
       _str[strlen(_str) - 1] = '\0';	 
	}
}

/*****************************************************************************/
                        /* Get User Command */
/*****************************************************************************/
int DoSystemCommand(void* _command, Semaphore* _semaphore, int* _flag)
{
	Command* com = _command;
		
	if (! strcmp(com->m_order, PAUSE))
	{
		if(SemaphorePauseThreads(_semaphore))
		{
			return -1;
		}
	}
	else if (! strcmp(com->m_order, RESUME))
	{
		if(SemaphoreResumeThreads(_semaphore))
		{
			return -1;
		}
	}
	else if (! strcmp(com->m_order, SHUT_DOWN))
	{
		if(SemaphoreResumeThreads(_semaphore))
		{
			return -1;
		}
		
		if(SemaphoreShutDown(_semaphore))
		{
			return -1;
		}
		
		*_flag = 1;
	}
	
	return 0;
}










