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

#define FS_MAXCHARLEN       13u

#define FS_MAXINPUTFILES    5u
#define FS_MAXOUTPUTFILES   5u

#define FS_INPUTPATH        "in"
#define FS_OUTPUTPATH       "out"
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
//enum to select sc card mount mdoe
enum FS_mountMode
{
    FS_MOUNTLATER,      //to be mounted on the first access to the volume
    FS_MOUNTNOW         //force mounted the volume to check if it is ready to work
};

//enum to distinguish sd card initialization state
enum FS_cardInitState
{
    FS_UNINITIALIZED,
    FS_INITIALIZED
};
/* END OF THE ENUM AREA */


/* START OF THE TYPEDEF AREA */
//typedef to store information about SD card
typedef struct
{
    FATFS *ptrFS;
    DWORD numFreeClusters;
    uint32_t totalSpace;
    uint32_t freeSpace;
    enum FS_cardInitState state;
}FS_SDcardInfo_T;

//typedef to store path to file or directory
typedef char FS_PathType[FS_MAXCHARLEN];

//typedef to store path to file name
typedef char FS_NameType[FS_MAXCHARLEN];

//typedef to store info about input and
//output files in given directory
typedef struct
{
    FILINFO in[FS_MAXINPUTFILES];
    FILINFO out[FS_MAXOUTPUTFILES];
    FS_PathType pathIn;
    FS_PathType pathOut;
}FS_DirInfo_T;

//typedef to store info about opened file
typedef struct
{
    FIL object;
    FS_NameType name;
    uint16_t lastLineNumber;
    bool isMoreLines;
    bool isOpen;
}FS_File_T;

//typedef to store info about all opened read
//and write files
typedef struct
{
    FS_File_T in;
    FS_File_T out;
}FS_OpenFiles_T;
/* END OF THE TYPEDEF AREA */


/* START OF THE EXTERN VARIABLES AREA */
/* END OF THE EXTERN VARIABLES AREA */


/* START OF THE FUNCTIONS PROTOTYPES AREA */
void FS_Init(void);
void FS_Main(void);
uint8_t FS_OpenFile(FS_File_T* file, FS_PathType path, uint8_t mode);
uint8_t FS_CloseFile(FS_File_T* file);
uint8_t FS_ReadFile(FS_File_T* file, uint8_t *buff, uint16_t len);
uint8_t FS_WriteFile(FS_File_T* file, uint8_t *buff);
FRESULT FS_ReadDir(FS_PathType path, FILINFO fileInfo[], uint8_t len);
FRESULT FS_GetSDcardCapacity(void);
FRESULT FS_GetSdCardInfo(void);
FRESULT FS_ReInit(void);
/* END OF THE FUNCTIONS PROTOTYPES AREA */


# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* FILESYSTEM_H */
