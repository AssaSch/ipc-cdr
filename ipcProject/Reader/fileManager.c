#define _GNU_SOURCE     /* needed to get the defines */
/** 
 * @brief file manager. 
 * @created 01.11.17
 * @author Asa Schneider
 */
  
#include "fileManager.h"
#include "logger.h"

#include <libgen.h> /* basename */
#include <string.h> /* strcpy, strcat */
#include <stdlib.h> /* malloc */
#include <dirent.h> /* readdir */
#include <sys/types.h> /* rewinddir */
#include <unistd.h> /* sleep */

#define DEFUALT_NUM_FILES 2

/*******************************************************/
/* global */
Logger* logger;
/*******************************************************/

/*****************************************************************************/
                        /* DetectFile */
/*****************************************************************************/
char* GetFilePath(const char* _currFolder, const char* _fileName)
{
	char* filePath;
	
	filePath = (char*)malloc(sizeof(char) * PATH_MAX_SIZE);
	if (! filePath)
	{
		return NULL;
	}
	
	strcpy(filePath, _currFolder);
	strcat(filePath, _fileName);
	free((void*)_fileName);
	
	return filePath;
}

/*****************************************************************************/
                        /* OpenFile */
/*****************************************************************************/
FILE* OpenFile(const char* _path)
{
	FILE* file;
	
	if (! _path)
	{
		return NULL;
	}
	
	file = fopen(_path, "r");
	
	return file;
}

/*****************************************************************************/
                        /* MoveFile */
/*****************************************************************************/
char* MoveFile(const char* _oldPath, char* _newDir)
{
	char* newPath;
	char* base;
	char tmpPath[PATH_MAX_SIZE];
	
	newPath = (char*)malloc(sizeof(char) * PATH_MAX_SIZE);
	if (!newPath)
	{
		return NULL;
	}
	
	strcpy(tmpPath, _oldPath);
	
	/* prepare new path */
	strcpy(newPath, _newDir);
	base = basename(tmpPath);
	strcat(newPath, base);

	/* change to new path */
	if (rename(_oldPath, newPath))
	{
		return NULL;
	}
	
	return newPath;
}

/*****************************************************************************/
                        /* DetectFile */
/*****************************************************************************/
int DetectFile(const char* _folderPath, char** _fileName, Semaphore* _semaphore)
{
	DIR* dir;
	struct dirent* entry;
	int i;
	int flag = 0;
	
	LOG_WRITE(logger, T, "open dir");
	if (! (dir = opendir(_folderPath)))
	{
		return -1;
	}
		
	while (1)
	{
		if (SemaphoreIsShutDown(_semaphore))
		{
			closedir(dir);
			return 1;
		}
		
		for (i = 0; i <= DEFUALT_NUM_FILES; ++i)
		{
			entry = readdir(dir);
			if (entry && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
			{
				break;
			}
		}
		
    	if (entry)
    	{
    		*_fileName = malloc(strlen(entry->d_name) + 1);
    		if (! *_fileName)
    		{
    			return -1;
    		}
    		
    		strcpy(*_fileName, entry->d_name);
    		flag = 1;
    	}
    	
    	rewinddir(dir);
		
		if (flag)
		{
			break;
		}
		
		sleep(1);
	}
	
	closedir(dir);
	LOG_WRITE(logger, T, "close dir");
		
	return 0;
}













