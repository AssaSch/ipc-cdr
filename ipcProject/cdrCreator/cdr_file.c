#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	char* m_name;
	int m_mcc;
	size_t m_msisdn[10000];
}Operator;

int main()
{
	int		 n;
	int 	 p;
	size_t 	 imsi;
	size_t 	 msisdn;
	size_t 	 imei;	
	char* 	 obn;
	int 	 mcc_mnc;
	char* 	 type;
	char 	 cDate[9] = "19/10/17";
	char 	 cTime[9] = "10:14:20";
	int 	 duration;
	int	 	 download;
	int	 	 upload;
	size_t 	 pMsisdn;
	int 	 pMcc_mnc;
	char*    pObn;
	char* 	 ptype[5] = {"MOC", "MTC", "SMSMO", "SMSMT", "GPRS"};
	FILE* 	 file;
	int i;
	char* operator[12] = {"Cellcom Israel Ltd.", "Golan Telecom Ltd", "Home Cellular Ltd", "Hot Mobile Ltd.", "Partner Communications Company Ltd.", "Pelephone Communications Ltd.", "Rami Levy Communications Ltd.", "Azi Communications Ltd.", "Alon Cellular Ltd", "Jawwal", "Wataniya Palestine Ltd.", "0"};
	int mcc[12] = {42502, 42508, 42515, 42577, 42501, 42512, 42516, 42519, 42514, 42505, 42506, 0};
	Operator* operators;
	
	operators = malloc(11 * sizeof(Operator));
	
	for(i = 0; i < 11; i++)
	{
		operators[i].m_name = operator[i];
		operators[i].m_mcc = mcc[i];
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[0].m_msisdn[i] = 972520000000 + (rand() % 500000);
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[1].m_msisdn[i] = 972530000000 + rand() % 500000;
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[2].m_msisdn[i] = 972540000000 + rand() % 500000;
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[3].m_msisdn[i] = 972550000000 + rand() % 500000;
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[4].m_msisdn[i] = 972560000000 + rand() % 500000;
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[5].m_msisdn[i] = 972570000000 + rand() % 500000;
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[6].m_msisdn[i] = 972580000000 + rand() % 500000;
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[7].m_msisdn[i] = 972590000000 + rand() % 500000;
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[8].m_msisdn[i] = 972558000000 + rand() % 500000;
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[9].m_msisdn[i] = 972567000000 + rand() % 500000;
	}
	
	for(i = 0; i < 10000; i++)
	{
		operators[10].m_msisdn[i] = 972529000000 + rand() % 500000;
	}
	
	file = fopen("cdrFile.txt", "w");

	
	for(i = 0; i < 7000000; i++)
	{
		if(i % 9 == 0)
		{
			n = rand() % 11;
			p = rand() % 10000;
			imsi = 342483927439275 + rand() % (1500000);
			imei = 827364958172648 + rand() % (1500000);
			msisdn = operators[n].m_msisdn[p];
			obn = operators[n].m_name;
			mcc_mnc = operators[n].m_mcc;
			download = rand() % 30 + 1;
			duration = 0;
			upload = 0;
			type = "GPRS";
			pMsisdn = 0;
			pMcc_mnc = 0;
			pObn = operator[11];
		}
		
		else if(i % 7 == 0)
		{
			n = rand() % 11;
			p = rand() % 10000;
			imsi = 342483927439275 + rand() % (1500000);
			imei = 827364958172648 + rand() % (1500000);
			msisdn = operators[n].m_msisdn[p];
			obn = operators[n].m_name;
			mcc_mnc = operators[n].m_mcc;
			upload = rand() % 30 + 1;
			download = 0;
			duration = 0;
			type = "GPRS";
			pMsisdn = 0;
			pMcc_mnc = 0;
			pObn = operator[11];
			
		}
		
		else if(i % 3 == 0)
		{
			n = rand() % 11;
			p = rand() % 10000;
			imsi = 342483927439275 + rand() % (1500000);
			imei = 827364958172648 + rand() % (1500000);
			msisdn = operators[n].m_msisdn[p];
			obn = operators[n].m_name;
			mcc_mnc = operators[n].m_mcc;
			upload = 0;
			download = 0;
			duration = 0;
			n = rand() % 11;
			p = rand() % 10000;
			pMsisdn = operators[n].m_msisdn[p];
			pMcc_mnc = operators[n].m_mcc;
			duration = 0;
			type = ptype[rand() % 2 + 2];
			pObn = operators[n].m_name;
		}
		
		else
		{
			n = rand() % 11;
			p = rand() % 10000;
			imsi = 342483927439275 + rand() % (1500000);
			imei = 827364958172648 + rand() % (1500000);
			msisdn = operators[n].m_msisdn[p];
			obn = operators[n].m_name;
			mcc_mnc = operators[n].m_mcc;
			duration = rand() % 10000 + 1;
			n = rand() % 11;
			p = rand() % 10000;
			pMsisdn = operators[n].m_msisdn[p];
			pMcc_mnc = operators[n].m_mcc;
			type = ptype[rand() % 2];
			download = 0;
			upload = 0;
			pObn = operators[n].m_name;
		}
		
		
		fprintf(file, "%lu|%lu|%lu|%s|%u|%s|%s|%s|%d|%d|%d|%lu|%d\n", imsi, msisdn, imei, obn, mcc_mnc, type, cDate, cTime, duration, download, upload, pMsisdn, pMcc_mnc);
	}
	
	free(operators);
	
	fclose(file);
	
	return 1;
}
