#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

/** 
 * @brief Handle files. 
 * @details Tools for handle files.
 *
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include <stdio.h> /* FILE* */
#include "semaphores.h" 

#define PATH_MAX_SIZE 1000

/**  
 * @brief open file from cdr files directory.
 * @param[in] _path - path of file.
 * @returns file pointer on seccuss / NULL on failure.
 */
FILE* OpenFile(const char* _path);

/**  
 * @brief move file to another directory.
 * @param[in] _oldpath - old path of file.
 * @param[in] _newpath - new path of file.
 * @returns file pointer on seccuss / NULL on failure.
 */
char* MoveFile(const char* _oldPath, char* _newDir);

/**  
 * @brief Detect new file in folder..
 * @param[in] _folderPath - Folder to detect.
 * @param[in] _fileName - return the new file name detected.
 * @return 0 - on success / -1 on fail. 
 */
int DetectFile(const char* _folderPath, char** _fileName, Semaphore* _semaphore);

/**  
 * @brief combine folder name and file name to one path.
 * @param[in] _currFolder - folder name.
 * @param[in] _fileName - new path of file.
 * @returns char pointer on seccuss / NULL on failure.
 */
char* GetFilePath(const char* _currFolder, const char* _fileName);


#endif /* __FILE_MANAGER_H__ */











