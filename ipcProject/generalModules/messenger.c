#define _GNU_SOURCE

/** 
 * @brief rapper functions for system V message queue API. 
 * @created 01.11.17
 * @author Asa Schneider
 */ 

#include "messenger.h"

#include <stdlib.h>
#include <string.h> /* strcpy, strcat */

#define MESSENGER_MAGIC 0xDeaDBeaF

struct Messenger
{
	size_t m_magic;
	int m_msqId;
};

struct MsgBuf
{
	long m_type;
	char m_data[1];
};

/*****************************************************************************/
                        /* Static Functions */
/*****************************************************************************/



/*****************************************************************************/
                        /* Messenger Create */
/*****************************************************************************/
Messenger* MessengerCreate()
{
	Messenger* messenger;

	messenger = (Messenger*)malloc(sizeof(Messenger));
	if (!messenger)
	{
		return NULL;
	}
	
	messenger->m_msqId = msgget(MSG_KEY, PERMITIONS | IPC_CREAT);
	if (-1 == messenger->m_msqId) 
	{	
		free(messenger);
		return NULL;
	}
	
	messenger->m_magic = MESSENGER_MAGIC;
	
	return messenger;
}

/*****************************************************************************/
                        /* Messenger Destroy */
/*****************************************************************************/
void MessengerDestroy(Messenger* _messenger)
{
	msgctl(_messenger->m_msqId, IPC_RMID, NULL);
	_messenger->m_magic = 0;
	free(_messenger);
}

/*****************************************************************************/
                        /* Send Massage */
/*****************************************************************************/
int SendMessage(Messenger* _messenger, int _flags, long _type, void* _data, size_t _dataSize)
{
	MsgBuf* msg;
	int err;
	
    msg = (MsgBuf*)malloc(sizeof(msg->m_type) + _dataSize);
    if (! msg)
	{
		return -1;
	}
	
    msg->m_type = _type;
    memcpy(msg->m_data, _data, _dataSize);
    err = msgsnd(_messenger->m_msqId, msg, _dataSize, _flags);
    free(_data);
    free(msg);
    
    return err;
}

/*****************************************************************************/
                        /* Recv Massage */
/*****************************************************************************/
MsgBuf* RecvMassage(Messenger* _messenger, int _flags, long _type, size_t _dataSize, int* _flagNoMsg)
{
	MsgBuf* msg;
	
	*_flagNoMsg = 0;
	
	msg = (MsgBuf*)malloc(sizeof(msg->m_type) + _dataSize);
	if (! msg)
	{
		return NULL;
	}
	
	if (-1 == msgrcv(_messenger->m_msqId, msg, _dataSize, _type, _flags))
	{
		free(msg);
		*_flagNoMsg = 1;
		return NULL;
	}
	
	return msg;
}

/*****************************************************************************/
                        /* UnPackMsg */
/*****************************************************************************/
void* UnPackMsg(MsgBuf* _msg)
{	
	return _msg->m_data;
}

/*****************************************************************************/
                        /* IsQueueEmpty */
/*****************************************************************************/
int IsQueueEmpty(Messenger* _messenger)
{
	struct msqid_ds buf;

	msgctl(_messenger->m_msqId, IPC_STAT, &buf);
	
	return buf.msg_qnum ? 0 : 1;
}















