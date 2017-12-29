#define _GNU_SOURCE
/** 
 * @brief print reports to file. 
 * @created 01.11.17
 * @author Asa Schneider
 */ 

#include "processorCommands.h"  
#include <sys/msg.h> /* massage queue */
#include <string.h> /* strcmp */

#define LENGTH_OF_ORDER 1

/*****************************************************************************/
                        /* Activate Command */
/*****************************************************************************/
int ActivateCommand(void* _command, DsManager* _dsManager, Semaphore* _semaphore)
{
	Command* com = _command;
	
	if (!com)
	{
		return -1;
	}
	
	if (! strncmp(com->m_order, MSISDN_QUERY, LENGTH_OF_ORDER))
	{
		if(MsisdnQuery(_dsManager, com->m_key))
		{
			return -1;
		}
	}
	else if (! strncmp(com->m_order, OPERATOR_QUERY, LENGTH_OF_ORDER))
	{
		if(OperatorQuery(_dsManager, com->m_key))
		{
			return -1;
		}
	}
	else if (! strncmp(com->m_order, SUBSRIBERS_REPORT, LENGTH_OF_ORDER))
	{
		if(AllSubscribersReport(_dsManager))
		{
			return -1;
		}
	}
	else if (! strncmp(com->m_order, OPERATORS_REPORT, LENGTH_OF_ORDER))
	{
		if(AllOperatorsReport(_dsManager))
		{
			return -1;
		}
	}
	
	return 0;
}

/*****************************************************************************/
                        /* Get Command Message */
/*****************************************************************************/
void* GetCommandMessage(Messenger* _messenger, int* _flagNoMsg)
{
	MsgBuf* msg;
	
	*_flagNoMsg  = 0;
	
	msg = RecvMassage(_messenger, IPC_NOWAIT, UI_TYPE, sizeof(Command), _flagNoMsg);
	if (!msg) /* if no message OR error on allocation */
	{	
		return NULL;			
	}
	else
	{
		return (void*)UnPackMsg(msg);
	}
}

/*****************************************************************************/
                        /* Get Cdr Message*/
/*****************************************************************************/
void* GetCdrMessage(Messenger* _messenger, int* _flagNoMsg)
{
	MsgBuf* msg;
	
	*_flagNoMsg  = 0;
	
	msg = RecvMassage(_messenger, IPC_NOWAIT, DATA_TYPE, sizeof(Cdr), _flagNoMsg);
	if (!msg) /* if no message OR error on allocation */
	{	
		return NULL;			
	}
	else
	{
		return (void*)UnPackMsg(msg);
	}
}




















