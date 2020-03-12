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
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
#define FS_MAXBUFFSIZE      _MAX_SS
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
//enum to select sc card mount mdoe
enum FS_mountMode
{
    FS_MOUNTLATER,
    FS_MOUNTNOW
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
/* END OF THE TYPEDEF AREA */


/* START OF THE EXTERN VARIABLES AREA */
/* END OF THE EXTERN VARIABLES AREA */


/* START OF THE FUNCTIONS PROTOTYPES AREA */
void FS_Init(void);
void FS_Main(void);
uint8_t FS_ReadFile(uint8_t *buff, uint8_t *len);
FRESULT FS_GetSDcardCapacity(void);
FRESULT FS_ReInit(void);
/* END OF THE FUNCTIONS PROTOTYPES AREA */


# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* FILESYSTEM_H */
