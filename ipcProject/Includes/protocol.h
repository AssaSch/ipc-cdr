#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/** 
 * @brief protocol for CDR files.
 * 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */

#define OUT_CALL "MOC"
#define IN_CALL "MTC"
#define OUT_MSG "SMS-MO"
#define IN_MSG "SMS-MT"
#define INTERNET "GPRS"

#define MSISDN_SIZE 16
#define IMSI_SIZE 16
#define BRAND_NAME_SIZE 65
#define TUPLE_SIZE 7
#define TYPE_SIZE 7


typedef struct Cdr Cdr;

struct Cdr
{
	size_t m_callDuration;
	size_t m_mbDownload;
	size_t m_mbUpload;
	char m_imsi[IMSI_SIZE];
	char m_msisdn[MSISDN_SIZE];
	char m_brandName[BRAND_NAME_SIZE];
	char m_operatorTuple[TUPLE_SIZE];
	char m_callType[TYPE_SIZE];
	char m_partyOperator[TUPLE_SIZE];
};


#endif /* __PROTOCOL_H__ */
