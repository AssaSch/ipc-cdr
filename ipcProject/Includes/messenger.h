#ifndef __MESSENGER_H__
#define __MESSENGER_H__

/** 
 * @brief rapper functions for system V message queue API. 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include <sys/msg.h> /* masge */

#define DATA_TYPE 1
#define UI_TYPE 2
#define PROCESSOR_TYPE 3
#define MSG_KEY 672495067
#define PERMITIONS 0666

/*****************************************************************************/

typedef struct MsgBuf MsgBuf;

typedef struct Messenger Messenger;

/*****************************************************************************/

/** 
 * @brief create message queue. if already exist - connect to queue.
 * @return Messenger* - on success / NULL on fail. 
 */
Messenger* MessengerCreate();

/** 
 * @brief destroy message queue.
 * @param[in] _messenger - _messenger to be deallocated.
 */
void MessengerDestroy(Messenger* _messenger);

/** 
 * @brief sends message.
 * @param[in] _messenger - Messenger data structre.
 * @param[in] _flags - flags for massage.
 * @param[in] _type - type of massage.
 * @param[in] _data - data to be sent.
 * @param[in] _dataSize - size of data to be sent.
 * @return 0 for success / -1 for failure.
 */
int SendMessage(Messenger* _messenger, int _flags, long _type, void* _data, size_t _dataSize);

/** 
 * @brief recieve message.
 * @param[in] _messenger - Messenger data structre.
 * @param[in] _flags - flags for massage.
 * @param[in] _type - type of massage.
 * @param[in] _dataSize - size of data to be sent.
 * @param[in] _flagNoMsg -if function returns NULL and _flagNoMsg is 0 = error.
 * @return MsgBuf* for success / NULL for failure.
 */
MsgBuf* RecvMassage(Messenger* _messenger, int _flags, long _type, size_t _dataSize, int* _flagNoMsg);

/** 
 * @brief unpack message to void*.
 * @param[in] _msg - massage data structre.
 * @return void*.
 */
void* UnPackMsg(MsgBuf* _msg);

/** 
 * @brief check if queue is empty.
 * @param[in] _messenger - Messenger data structre.
 * @return 1 - if empty / 0 - if not empty.
 */
int IsQueueEmpty(Messenger* _messenger);


#endif /* __MESSENGER_H__ */
