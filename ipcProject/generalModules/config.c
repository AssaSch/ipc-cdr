#define _GNU_SOURCE

/** 
 * @brief rapper functions for system V message queue API. 
 * @created 01.11.17
 * @author Asa Schneider
 */ 

#include "config.h"
#include <string.h> /* strtol */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* fopen, fclose */

#define CONFIG_MAGIC 0xDeaDBeaF
#define FIRST_LINE 2
#define SECOND_LINE 3
#define THIRD_LINE 3
#define FORTH_LINE 2
#define DECIMAL 10
#define MAX_PATH 1000
#define LINE_SIZE 220
#define CONFIG_PATH "configuration/configFile"

/*****************************************************************************/

struct Config
{
	size_t m_magic;
	size_t m_procThreads;
	char m_newFilesFolder[MAX_PATH];
	char m_inWorkFolder[MAX_PATH];
	char m_doneFolder[MAX_PATH];
};

/*****************************************************************************/

static int GetInfoFromFile(Config* _config);

/*****************************************************************************/
                        /* Config Create */
/*****************************************************************************/
Config* ConfigCreate()
{
	Config* config;

	config = (Config*)malloc(sizeof(Config));
	if (!config)
	{
		return NULL;
	}
	
	GetInfoFromFile(config);
	
	config->m_magic = CONFIG_MAGIC;
	
	return config;
}

/*****************************************************************************/
                        /* Config Destroy */
/*****************************************************************************/
void ConfigDestroy(Config* _config)
{
	_config->m_magic = 0;
	free(_config);
}

/*****************************************************************************/
                        /* Get Info From File */
/*****************************************************************************/
static int GetInfoFromFile(Config* _config)
{
	FILE* file;
	char buffer[LINE_SIZE];
	char temp[LINE_SIZE];
	char numOfThreads[LINE_SIZE];
	int i;
	
	file = fopen(CONFIG_PATH, "r");
	if (!file)
	{
		return -1;
	}
	
	for (i = 0; i < FIRST_LINE; ++i)
	{
		fgets(buffer, LINE_SIZE, file);	
	}
	
	sscanf(buffer, "%s %s", temp, _config->m_newFilesFolder);
	
	for (i = 0; i < SECOND_LINE; ++i)
	{
		fgets(buffer, LINE_SIZE, file);	
	}
	
	sscanf(buffer, "%s %s", temp, _config->m_inWorkFolder);
	
	for (i = 0; i < THIRD_LINE; ++i)
	{
		fgets(buffer, LINE_SIZE, file);	
	}
	
	sscanf(buffer, "%s %s", temp, _config->m_doneFolder);
	
	for (i = 0; i < FORTH_LINE; ++i)
	{
		fgets(buffer, LINE_SIZE, file);	
	}
	
	sscanf(buffer, "%s %s %s %s %s", temp, temp, temp, temp, numOfThreads);
	
	_config->m_procThreads = (size_t)strtol(numOfThreads, NULL, DECIMAL);
	
	fclose(file);
	
	return 0;
}

/*****************************************************************************/
                        /* Get proc Threads */
/*****************************************************************************/
size_t GetProcThreads(Config* _config)
{
	return _config->m_procThreads;
}

/*****************************************************************************/
                        /* Get New File Folder */
/*****************************************************************************/
char* GetNewFileFolder(Config* _config)
{
	return _config->m_newFilesFolder;
}

/*****************************************************************************/
                        /* Get In Work Folder */
/*****************************************************************************/
char* GetInWorkFolder(Config* _config)
{
	return _config->m_inWorkFolder;
}

/*****************************************************************************/
                        /* Get Done Folder */
/*****************************************************************************/
char* GetDoneFolder(Config* _config)
{
	return _config->m_doneFolder;
}








