#include <stdlib.h> /* for malloc & NULL & free */
#include <stdio.h>
#include <string.h>
#include "reportsGenerator.h"
#include "fileManager.h"
#include <unistd.h>
#include "mu_test.h"

#define SUBS_SIZE 100000
#define OPR_SIZE 100000


/*
int	KeyValueActionFunc(const void* _key, void* _value, void* _context)
{
	printf("%d, %d, %s\n", *((int*)_key), *((int*)_value), (char*)_context);
	
	return 1;
}

void KeyDestroy(void* _item)
{	
	free(_item);
}

void ValueDestroy(void* _item)
{	
	free(_item);
}*/

/*****************************************************************************/
                 /** cdrParser tests **/
/*****************************************************************************/
UNIT(getLine_ok)
	
	char* buf;
	FILE* file;

	file = fopen("cdrFile1", "r");
	buf = GetLine(file);		 
	printf("%s\n", buf);
	free(buf);
	
END_UNIT

/*****************************************************************************/

UNIT(ParseCdr_ok)
	
	char* buf;
	FILE* file;
	Cdr* cdr;

	file = fopen("cdrFile1", "r");
	buf = GetLine(file);		 
	cdr = ParseCdr(buf);
	printf("%lu, %lu, %lu, %s, %s, %s, %s, %s, %s\n", cdr->m_callDuration, cdr->m_mbDownload, cdr->m_mbUpload, cdr->m_imsi, cdr->m_msisdn, cdr->m_brandName, cdr->m_operatorTuple, cdr->m_callType, cdr->m_partyOperator);
	free(buf);
	
END_UNIT

/*****************************************************************************/
                 /** dsManager tests **/
/*****************************************************************************/
UNIT(dsManagerCreate_null)
	
	ASSERT_THAT(! CreateDsManager(0, 0));

END_UNIT

/*****************************************************************************/

UNIT(dsManagerCreate_ok)
	
	DsManager* dsManager;

	dsManager = CreateDsManager(SUBS_SIZE, OPR_SIZE);		 
	ASSERT_THAT(dsManager);
	DestroyDsManager(dsManager);
	
END_UNIT

/*****************************************************************************/

UNIT(dsManagerinsert_ok)
	
	DsManager* dsManager;
	char* buf;
	Cdr* cdr1;
	Cdr* cdr2;
	FILE* file;
	char subKey[16] = "888888888888888";
	char oprKey[6] = "42502";

	file = fopen("cdrFile1", "r");
	buf = GetLine(file);		 
	cdr1 = ParseCdr(buf);
	buf = GetLine(file);		 
	cdr2 = ParseCdr(buf);
	
	dsManager = CreateDsManager(SUBS_SIZE, OPR_SIZE);
	ASSERT_THAT(! Upsert(dsManager, cdr1));
	ASSERT_THAT(! Upsert(dsManager, cdr1));
	ASSERT_THAT(! Upsert(dsManager, cdr2));
	ASSERT_THAT(! Upsert(dsManager, cdr2));
	MsisdnQuery(dsManager, subKey);
	OperatorQuery(dsManager, oprKey);
	AllSubscribersReport(dsManager);
	AllOperatorsReport(dsManager);
	DestroyDsManager(dsManager);
	
END_UNIT

/*****************************************************************************/
                 /** fileManager tests **/
/*****************************************************************************/
UNIT(moveFile_ok)
	
	 MoveFile("oldPath/test.txt", "newPath/");
	
END_UNIT

/*****************************************************************************/
UNIT(detectAndMoveFile_ok)
	
	char* file;
	
	while (1)
	{
		/*DetectFile();*/
		sleep(1);
		file = GetFileName("oldPath/");
		if (file)
		{
			printf("%s\n", file);
			file = MoveFile(file, "newPath/");
			printf("%s\n", file);
		}
		
		
		
		/*printf("%s\n", file);
		file = MoveFile(file, "./newPath/");
		printf("%s\n", file);*/
	}
	
END_UNIT

/*****************************************************************************/
/*****************************************************************************/

TEST_SUITE(framework test)

	/** cdrParser tests **/
	TEST(getLine_ok)
	TEST(ParseCdr_ok)
	
	/** dsManager tests **/
	TEST(dsManagerCreate_null)
	TEST(dsManagerCreate_ok)
	TEST(dsManagerinsert_ok)
	
	/** fileManager tests **/	
	/*TEST(moveFile_ok)*/
	TEST(detectAndMoveFile_ok)
	
	/*TEST(hash_remove_ok)
	TEST(HashMapDestroy_ok)
	TEST(HashMapFind_null)
	TEST(HashMapFind_key_null)
	TEST(HashMapFind_ok)
	TEST(HashMapSize_ok)
	TEST(HashMapRehash_ok)
	TEST(HashMapForEach_ok)
	TEST(HashMapInsertAlotInt_ok)
	TEST(HashMapInsertAlot_ok)*/
		
END_SUITE

/*****************************************************************************/
/*****************************************************************************/
	
