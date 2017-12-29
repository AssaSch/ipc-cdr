#ifndef __CONFIG_H__
#define __CONFIG_H__

/** 
 * @brief Config API.
 *	
 * @details handles configuration file information.
 * 
 * @created 30.10.2017
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */

/*#define PROCESSOR_PATH "../Proccesor/proccesor"
#define UI_PATH "../Ui/ui"*/
#define PROCESSOR_PATH "./processor"
#define UI_PATH "./ui"

typedef struct Config Config;

/** 
 * @brief Create a new Config.
 * insert configuration information into Config.
 * @return newly created Config or null on failure
 */
Config* ConfigCreate();


/**
 * @brief destroy Config.
 * @param[in] _config - Config to be destroyed.
 */
void ConfigDestroy(Config* _config);

/** 
 * @brief Get number of threads.
 * @param[in] _config - Config Data structure.
 * @return number of threads.
 */
size_t GetProcThreads(Config* _config);

/** 
 * @brief Get New File Folder path.
 * @param[in] _config - Config Data structure.
 * @return New File Folder path.
 */
char* GetNewFileFolder(Config* _config);

/** 
 * @brief Get In Work Folder path.
 * @param[in] _config - Config Data structure.
 * @return In Work Folder path.
 */
char* GetInWorkFolder(Config* _config);

/** 
 * @brief Get Done Folder path.
 * @param[in] _config - Config Data structure.
 * @return Done Folder path.
 */
char* GetDoneFolder(Config* _config);


#endif /* __CONFIG_H__ */
