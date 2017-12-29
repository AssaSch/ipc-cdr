#ifndef __REPORTS_GENERATOR_H__
#define __REPORTS_GENERATOR_H__

/** 
 * @brief generate reports. 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 
 
#include "DsManager.h"  
#include <stddef.h>  /* size_t */
#include <stdio.h>	/* FILE* */

/**  
 * @brief retrieving​ information​ ​for​ ​one​ subscriber​​ at​​ a​​ time.​ The​​ subscriber​​ will
 * be​ ​identified​ ​by​ ​his​ ​MSISDN.
 * @param[in] _dsManager - dsManager pointer.
 * @param[in] _key - key to get item.
 * @returns file pointer on seccuss / NULL on failure.
 */
int MsisdnQuery(DsManager* _dsManager, char* _key);

/**  
 * Support​ ​retrieving​ information​ for​ one​​ operator​ at​ a​​ time.​ ​The​​ operator​​ will​​ be
 * identified​ ​by​​ his​​ MCC/MNC.
 * @param[in] _dsManager - dsManager pointer.
 * @param[in] _key - key to get item.
 * @returns file pointer on seccuss / NULL on failure.
 */
int OperatorQuery(DsManager* _dsManager, char* _key);

/**  
 * Produce​ ​one​​ file​ with​​ one​ record​ for​​ each​​ subscriber​ ​containing​ ​aggregated
 * information.​ ​This​ ​aggregated​ ​information​ ​will​ be​​ imported​  to​ other​ ​systems​  for
 * purposes​ ​such​ ​as​  billing.
 * @param[in] _dsManager - dsManager pointer.
 * @returns file pointer on seccuss / NULL on failure.
 */
int AllSubscribersReport(DsManager* _dsManager);

/**  
 * Produce​ ​one​​ file​ with​​ one​ record​ for​​ each​​ operator ​containing​ ​aggregated
 * information.​ ​This​ ​aggregated​ ​information​ ​to​ be​ ​used​  in​ ​inter-operator​ ​settlement​  process.
 * @param[in] _dsManager - dsManager pointer.
 * @returns file pointer on seccuss / NULL on failure.
 */
int AllOperatorsReport(DsManager* _dsManager);

#endif /* __REPORTS_GENERATOR_H__ */











