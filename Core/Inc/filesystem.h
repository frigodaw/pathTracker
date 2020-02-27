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
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
/* END OF THE ENUM AREA */


/* START OF THE TYPEDEF AREA */
//typedef to store information about SD card capacity
typedef struct
{
    FATFS *ptrFS;
    DWORD numFreeClusters;
    uint32_t totalSpace;
    uint32_t freeSpace;
}FS_SDcardCapacity_T;
/* END OF THE TYPEDEF AREA */


/* START OF THE EXTERN VARIABLES AREA */
/* END OF THE EXTERN VARIABLES AREA */


/* START OF THE FUNCTIONS PROTOTYPES AREA */
void FS_Init(void);
void FS_Main(void);
void FS_ReadFile(void);
FRESULT FS_GetSDcardCapacity(void);
/* END OF THE FUNCTIONS PROTOTYPES AREA */


# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* FILESYSTEM_H */
