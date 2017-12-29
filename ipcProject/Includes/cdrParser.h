#ifndef __CDR_PARSER_H__
#define __CDR_PARSER_H__

/** 
 * @brief cdr parser API.
 *	
 * @details Tools for getting the needed information out of CDR file.
 * 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 


#include <stddef.h>  /* size_t */
#include <stdio.h>	/* FILE* */

/** 
 * @brief copy one line from cdr file.
 * @param[in] _file - file pointer.
 * @return buffer pointer for success / NULL for failure.
 */
char* GetLine(FILE* _file);

/** 
 * @brief parse one line from cdr file according to relevant fields.
 * @param[in] _buffer - pointer to allocated buffer to save line into.
 * @return void pointer for success / NULL for failure.
 */
void* ParseCdr(char* _buffer);


#endif /* __CDR_PARSER_H__ */
