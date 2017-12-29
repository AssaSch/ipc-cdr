/** 
 * @brief print reports to file. 
 * @created 01.11.17
 * @author Asa Schneider
 */ 

#include "reportsGenerator.h"  
#include <time.h>
	
#define MSISDN_PATH "Reports/subQuery"
#define OPR_PATH "Reports/oprQuery"
#define ALL_SUBS_PATH "Reports/allSubs"
#define ALL_OPR_PATH "Reports/allopr"


int	PrintSubs(const void* _key, void* _value, void* _context);
int	PrintOpr(const void* _key, void* _value, void* _context);
static void PrintHeader(FILE* _file);

/*****************************************************************************/
                        /* MsisdnQuery */
/*****************************************************************************/
int MsisdnQuery(DsManager* _dsManager, char* _key)
{
	FILE* file;
	Subscriber* subs;
	
	file = fopen(MSISDN_PATH, "w");
	if (!file)
	{
		return -1;
	}
	
	subs = GetOneItem(_dsManager, _key, SUBS);
	if (subs)
	{
		PrintHeader(file);
		PrintSubsToFile(file, subs);	
	}
	
	fclose(file);
	
	return 0;
}

/*****************************************************************************/
                        /* OperatorQuery */
/*****************************************************************************/
int OperatorQuery(DsManager* _dsManager, char* _key)
{
	FILE* file;
	Operator* opr;
	
	file = fopen(OPR_PATH, "w");
	if (!file)
	{
		return -1;
	}
	
	opr = GetOneItem(_dsManager, _key, OPR);
	if (opr)
	{
		PrintHeader(file);
		PrintOprToFile(file, opr);	
	}
	
	fclose(file);
	
	return 0;
}

/*****************************************************************************/
                        /* AllSubscribersReport */
/*****************************************************************************/
int AllSubscribersReport(DsManager* _dsManager)
{
	FILE* file;
	
	file = fopen(ALL_SUBS_PATH, "w");
	if (!file)
	{
		return -1;
	}
	
	PrintHeader(file);
	ForEach(_dsManager, PrintSubs, file, SUBS);
	
	fclose(file);
	
	return 0;
}

/*****************************************************************************/
                        /* AllOperatorsReport */
/*****************************************************************************/
int AllOperatorsReport(DsManager* _dsManager)
{
	FILE* file;
	
	file = fopen(ALL_OPR_PATH, "w");
	if (!file)
	{
		return -1;
	}
	
	PrintHeader(file);
	ForEach(_dsManager, PrintOpr, file, OPR);
	
	fclose(file);
		
	return 0;
}

/*****************************************************************************/
static void PrintHeader(FILE* _file)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	fprintf(_file, "%04d/%02d/%02d,%02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}


/*****************************************************************************/
                        /* KeyValueActionFunc */
/*****************************************************************************/
int	PrintSubs(const void* _key, void* _value, void* _context)
{
	PrintSubsToFile((FILE*)_context, (Subscriber*)_value);
	ZeroSubs((Subscriber*)_value);
	
	return 1;
}

/*****************************************************************************/
                        /* KeyValueActionFunc */
/*****************************************************************************/
int	PrintOpr(const void* _key, void* _value, void* _context)
{
	PrintOprToFile((FILE*)_context, (Operator*)_value);
	ZeroOpr((Operator*)_value);
	
	return 1;
}








