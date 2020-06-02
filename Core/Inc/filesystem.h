/******************************************************************************
* @file   : filesystem.h
* @brief  : Header for filesystem.c file.
*           This file contains the common defines and datatypes for FS module.
******************************************************************************/

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */


/* START OF THE INCLUDE AREA */
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "fatfs_sd.h"
#include "ffconf.h"
#include "types.h"
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
#define FS_MAXBUFFSIZE      _MAX_SS
#define FS_BUFFSIZE         256u

#define FS_SHORTCHARLEN     13u

#define FS_MAXFAILSNUM      10u

#if ((_USE_LFN == 2u) || (_USE_LFN == 3u))
# define FS_FULLCHARLEN     _MAX_LFN
#else
# define FS_FULLCHARLEN     FS_SHORTCHARLEN
#endif

#define FS_SAVEDFILESNUM    5u
#define FS_MAXFILESNUM      255u

#define FS_INPUTPATH        "in"
#define FS_OUTPUTPATH       "out"
#define FS_REPORTSPATH      "reports"
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
//enum to select sc card mount mdoe
typedef enum
{
    FS_MOUNTLATER,      //to be mounted on the first access to the volume
    FS_MOUNTNOW         //force mounted the volume to check if it is ready to work
}FS_mountMode;

//enum to distinguish sd card initialization state
typedef enum
{
    FS_UNINITIALIZED,
    FS_INITIALIZED
}FS_cardInitState;

//enum to select mode during opening a new file
typedef enum
{
    FS_NOMODE,
    FS_MODEREAD,
    FS_MODEWRITE,
    FS_MODEAPPEND
}FS_fileMode;
/* END OF THE ENUM AREA */


/* START OF THE TYPEDEF AREA */
//typedef to store information about SD card
typedef struct
{
    FATFS *ptrFS;
    DWORD numFreeClusters;
    uint32_t totalSpace;
    uint32_t freeSpace;
    FS_cardInitState state;
}FS_SDcardInfo_T;

//typedefs to store path to file or directory
typedef char  FS_ShortPathType[FS_SHORTCHARLEN];
typedef char  FS_FullPathType[FS_FULLCHARLEN];

//typedef to store info about files
//in given directory
typedef struct
{
    FS_ShortPathType path;
    //FS_FullPathType fileName[FS_SAVEDFILESNUM];
    uint8_t filesNum;
    FRESULT state;
}FS_Dir_T;

//typedef to store info about given
//directories and files inside them
typedef struct
{
    FS_Dir_T in;
    FS_Dir_T out;
    FS_Dir_T reports;
}FS_DirsCollection_T;

//typedef to store info about opened file
typedef struct
{
    FIL object;
    FS_FullPathType name;
    uint16_t lastLineNumber;
    boolean isMoreLines;
    boolean isOpen;
    FS_fileMode mode;
}FS_File_T;

//typedef to store info about all opened read
//and write files
typedef struct
{
    FS_File_T in;
    FS_File_T out;
}FS_FilesCollection_T;
/* END OF THE TYPEDEF AREA */


/* START OF THE EXTERN VARIABLES AREA */
extern FS_SDcardInfo_T sdCardInfo;
extern FS_DirsCollection_T dirInfo;
/* END OF THE EXTERN VARIABLES AREA */


/* START OF THE FUNCTIONS PROTOTYPES AREA */
void FS_Init(void);
void FS_Main(void);
uint8_t FS_OpenFile(FS_File_T** file, FS_FullPathType path, FS_fileMode mode);
uint8_t FS_CloseFile(FS_File_T** file);
uint8_t FS_RenameFile(FS_File_T** file, FS_FullPathType newPath);
uint8_t FS_Lseek(FS_File_T** file, uint32_t offset);
uint8_t FS_LseekEnd(FS_File_T** file);
uint8_t FS_ReadFile(FS_File_T* file, uint8_t *buff, uint16_t len, boolean *isMoreLines);
uint8_t FS_WriteFile(FS_File_T* file, uint8_t *buff);
FRESULT FS_ReadDir(FS_Dir_T* dir);
FRESULT FS_GetSDcardCapacity(void);
FRESULT FS_GetSdCardInfo(void);
FRESULT FS_ReInit(void);
/* END OF THE FUNCTIONS PROTOTYPES AREA */


# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* FILESYSTEM_H */
