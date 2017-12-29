#include "DsMembers.h"

/** 
 * @brief manage subscribers and operators data structures. 
 * @created 01.11.17
 * @author Asa Schneider
 */ 

#include <string.h> /* strcmp*/
#include <stdlib.h> /* strtol */

#define DS_MEM_MAGIC 0xDeaDBeaF
#define DECIMAL 10

/*****************************************************************************/

struct Subscriber
{
	size_t m_incomingCallDurationIn;
	size_t m_outgoingCallDurationIn;
	size_t m_incomingCallDurationOut;
	size_t m_outgoingCallDurationOut;
	size_t m_incomingSmsMsgIn;
	size_t m_outgoingSmsMsgIn;
	size_t m_incomingSmsMsgOut;
	size_t m_outgoingSmsMsgOut;
	size_t m_mbDownload;
	size_t m_mbUpload;
	char m_imsi[IMSI_SIZE];
};

/*****************************************************************************/

struct Operator
{
	size_t m_incomingCallDuration;
	size_t m_outgoingCallDuration;
	size_t m_incomingSmsMsg;
	size_t m_outgoingSmsMsg;
	char m_brandName[BRAND_NAME_SIZE];
	char m_operatorTuple[TUPLE_SIZE];
};

/*****************************************************************************/

static void InsertSubsSameOperator(Subscriber* _subscriber, Cdr* _cdr);
static void InsertSubsDifferOperator(Subscriber* _subscriber, Cdr* _cdr);

/*****************************************************************************/
			/* KeyValue Action Functions  */
/*****************************************************************************/
void UpdateSubs(void* _oldValue, void* _value)
{
	Subscriber* oldSubs = _oldValue;
	Subscriber* subs = _value;
	
	oldSubs->m_incomingCallDurationIn += subs->m_incomingCallDurationIn;
	oldSubs->m_outgoingCallDurationIn += subs->m_outgoingCallDurationIn;
	oldSubs->m_incomingCallDurationOut += subs->m_incomingCallDurationOut;
	oldSubs->m_outgoingCallDurationOut += subs->m_outgoingCallDurationOut;
	oldSubs->m_incomingSmsMsgIn += subs->m_incomingSmsMsgIn;
	oldSubs->m_outgoingSmsMsgIn += subs->m_outgoingSmsMsgIn;
	oldSubs->m_incomingSmsMsgOut += subs->m_incomingSmsMsgOut;
	oldSubs->m_outgoingSmsMsgOut += subs->m_outgoingSmsMsgOut;
	oldSubs->m_mbDownload += subs->m_mbDownload;
	oldSubs->m_mbUpload += subs->m_mbUpload;
}

/*****************************************************************************/
void UpdateOprs(void* _oldValue, void* _value)
{
	Operator* oldOpr = _oldValue;
	Operator* opr = _value;
	
	oldOpr->m_incomingCallDuration += opr->m_incomingCallDuration;
	oldOpr->m_outgoingCallDuration += opr->m_outgoingCallDuration;
	oldOpr->m_incomingSmsMsg += opr->m_incomingSmsMsg;
	oldOpr->m_outgoingSmsMsg += opr->m_outgoingSmsMsg;
}

/*****************************************************************************/
			/* Create Subscriber */
/*****************************************************************************/
Subscriber* CreateSubscriber(Cdr* _cdr)
{
	Subscriber* subscriber;
	
	subscriber = (Subscriber*)calloc(1, sizeof(Subscriber));
	if (! subscriber)
	{
		return NULL;
	}
	
	/* if cdr inside mobile operator */
	if (! strcmp(_cdr->m_operatorTuple, _cdr->m_partyOperator))
	{
		InsertSubsSameOperator(subscriber, _cdr);
	}
	else /* if cdr between two mobile operators */
	{
		InsertSubsDifferOperator(subscriber, _cdr);
	}
	
	subscriber->m_mbDownload = _cdr->m_mbDownload;
	subscriber->m_mbUpload = _cdr->m_mbUpload;
	strcpy(subscriber->m_imsi, _cdr->m_imsi);
	
	return subscriber;
}

/*****************************************************************************/
static void InsertSubsSameOperator(Subscriber* _subscriber, Cdr* _cdr)
{
	if (! strcmp(_cdr->m_callType, OUT_CALL))
	{
		_subscriber->m_outgoingCallDurationIn = _cdr->m_callDuration;
	}
	else if (! strcmp(_cdr->m_callType, IN_CALL))
	{
		_subscriber->m_incomingCallDurationIn = _cdr->m_callDuration;
	}
	else if (! strcmp(_cdr->m_callType, OUT_MSG))
	{
		_subscriber->m_outgoingSmsMsgIn = 1;
	}
	else if (! strcmp(_cdr->m_callType, IN_MSG))
	{
		_subscriber->m_incomingSmsMsgIn = 1;
	}
}

/*****************************************************************************/
static void InsertSubsDifferOperator(Subscriber* _subscriber, Cdr* _cdr)
{
	if (! strcmp(_cdr->m_callType, OUT_CALL))
	{
		_subscriber->m_outgoingCallDurationOut = _cdr->m_callDuration;
	}
	else if (! strcmp(_cdr->m_callType, IN_CALL))
	{
		_subscriber->m_incomingCallDurationOut = _cdr->m_callDuration;
	}
	else if (! strcmp(_cdr->m_callType, OUT_MSG))
	{
		_subscriber->m_outgoingSmsMsgOut = 1;
	}
	else if (! strcmp(_cdr->m_callType, IN_MSG))
	{
		_subscriber->m_incomingSmsMsgOut = 1;
	}
}

/*****************************************************************************/
                        /* Destroy Subscriber */
/*****************************************************************************/
void SubscriberDestroy(Subscriber* _subscriber)
{
	free(_subscriber);
}

/*****************************************************************************/
			/* Create Operator */
/*****************************************************************************/
Operator* CreateOperator(Cdr* _cdr)
{
	Operator* operator;
	
	operator = (Operator*)calloc(1, sizeof(Operator));
	if (! operator)
	{
		return NULL;
	}
	
	strcpy(operator->m_brandName, _cdr->m_brandName);
	strcpy(operator->m_operatorTuple, _cdr->m_operatorTuple);
	
	if (! strcmp(_cdr->m_callType, OUT_CALL))
	{
		operator->m_outgoingCallDuration = _cdr->m_callDuration;
	}
	else if (! strcmp(_cdr->m_callType, IN_CALL))
	{
		operator->m_incomingCallDuration = _cdr->m_callDuration;
	}
	else if (! strcmp(_cdr->m_callType, OUT_MSG))
	{
		++operator->m_outgoingSmsMsg;
	}
	else if (! strcmp(_cdr->m_callType, IN_MSG))
	{
		++operator->m_incomingSmsMsg;
	}
	
	return operator;
}

/*****************************************************************************/
                        /* Destroy Operator */
/*****************************************************************************/
void OperatorDestroy(Operator* _operator)
{
	free(_operator);
}

/*****************************************************************************/
			/* GetSubsKey */
/*****************************************************************************/
char* GetSubsKey(Cdr* _cdr)
{
	char* subsKey;
	
	subsKey = (char*)malloc(sizeof(char) * MSISDN_SIZE);
	if (! subsKey)
	{
		return NULL;
	}
	
	strcpy(subsKey, _cdr->m_msisdn);
	
	return subsKey;
}

/*****************************************************************************/
			/* GetSubsKey */
/*****************************************************************************/
char* GetOperKey(Cdr* _cdr)
{
	char* OperKey;
	
	OperKey = (char*)malloc(sizeof(char) * TUPLE_SIZE);
	if (! OperKey)
	{
		return NULL;
	}
	
	strcpy(OperKey, _cdr->m_operatorTuple);
	
	return OperKey;
}

/*****************************************************************************/
                        /* PrintSubsToFile */
/*****************************************************************************/
void PrintSubsToFile(FILE* _file, Subscriber* _subs)
{
	fprintf(_file, "%s,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n", _subs->m_imsi,
		 _subs->m_outgoingCallDurationIn, _subs->m_incomingCallDurationIn, _subs->m_outgoingCallDurationOut, 
		 _subs->m_incomingCallDurationOut, _subs->m_outgoingSmsMsgIn, _subs->m_incomingSmsMsgIn,
		  _subs->m_outgoingSmsMsgOut, _subs->m_incomingSmsMsgOut, _subs->m_mbDownload, _subs->m_mbUpload);
}

/*****************************************************************************/
                        /* PrintSubsToFile */
/*****************************************************************************/
void PrintOprToFile(FILE* _file, Operator* _opr)
{	
	fprintf(_file, "%s,%s,%lu,%lu,%lu,%lu\n", _opr->m_operatorTuple,
	 _opr->m_brandName,_opr->m_incomingCallDuration, _opr->m_outgoingCallDuration,
	  _opr->m_incomingSmsMsg, _opr->m_outgoingSmsMsg);
}

/*****************************************************************************/
                        /* PrintSubsToFile */
/*****************************************************************************/
void ZeroSubs(Subscriber* _subs)
{
	_subs->m_incomingCallDurationIn = 0;
	_subs->m_outgoingCallDurationIn = 0;
	_subs->m_incomingCallDurationOut = 0;
	_subs->m_outgoingCallDurationOut = 0;
	_subs->m_incomingSmsMsgIn = 0;
	_subs->m_outgoingSmsMsgIn = 0;
	_subs->m_incomingSmsMsgOut = 0;
	_subs->m_outgoingSmsMsgOut = 0;
	_subs->m_mbDownload = 0;
	_subs->m_mbUpload = 0;
}

/*****************************************************************************/
                        /* PrintSubsToFile */
/*****************************************************************************/
void ZeroOpr(Operator* _opr)
{
	_opr->m_incomingCallDuration = 0;
	_opr->m_outgoingCallDuration = 0;
	_opr->m_incomingSmsMsg = 0;
	_opr->m_outgoingSmsMsg = 0;
}
