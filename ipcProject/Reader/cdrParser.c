#include "cdrParser.h"  

#include "protocol.h"
#include <string.h> /* strtol */
#include <stdlib.h> /* malloc */

/** 
 * @brief cdr parser. 
 * @created 01.11.17
 * @author Asa Schneider
 */ 

#define LINE_SIZE 220
#define MAX_ARG_SIZE 20
#define DECIMAL 10

/*****************************************************************************/
                        /* GetLine */
/*****************************************************************************/
char* GetLine(FILE* _file)
{
	char* buffer;
	
	buffer = (char*)malloc(sizeof(char) * LINE_SIZE);
	if (! buffer)
	{
		return NULL;
	}

	return fgets(buffer, LINE_SIZE, _file);
}

/*****************************************************************************/
                        /* GetLine */
/*****************************************************************************/
void* ParseCdr(char* _buffer)
{
	Cdr* cdr;
	char temp[MAX_ARG_SIZE];
	char callDuration[MAX_ARG_SIZE];
	char mbDownload[MAX_ARG_SIZE];
	char mbUpload[MAX_ARG_SIZE];
	
	cdr = (Cdr*)calloc(1, sizeof(Cdr));
	if (! cdr)
	{
		return NULL;
	}
	
	sscanf(_buffer, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s", cdr->m_imsi, cdr->m_msisdn,
	 temp, cdr->m_brandName, cdr->m_operatorTuple, cdr->m_callType, temp, temp,
	  callDuration, mbDownload, mbUpload, temp, cdr->m_partyOperator);
	  
	cdr->m_callDuration = (size_t)strtol(callDuration, NULL, DECIMAL);
	cdr->m_mbDownload = (size_t)strtol(mbDownload, NULL, DECIMAL);
	cdr->m_mbUpload = (size_t)strtol(mbUpload, NULL, DECIMAL);
	
	free(_buffer);
 	
	return (void*)cdr;
}



