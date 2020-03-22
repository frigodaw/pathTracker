/******************************************************************************
* @file           : filesystem.c
* @brief          : Source for filesystem.c file.
*                   This file contains the source code for files-related data
******************************************************************************/

#include "filesystem.h"
#include "types.h"
#include <string.h>
#include <stdio.h>

/* Main file system variable */
static FATFS fs = {0u};

/* The main info about current SD card */
static FS_SDcardInfo_T sdCardInfo = {0u};

/* Variable with a list of directories and a files inside them */
static FS_DirsCollection_T dirInfo = {0u};

/* Component's buffer to read and write data to and from a test file */
static uint8_t fsBuffer[FS_BUFFSIZE] = {0u};

/* Variable to store currently opened read and write file */
static FS_FilesCollection_T files = {0u};


/* Init function for FS module to mount SD card and
   initialize varaibles */
void FS_Init(void)
{
    FRESULT fresult = FR_OK;

    /* Set path to 'in' and 'out' directories */
    memcpy(&dirInfo.in.path, FS_INPUTPATH, strlen(FS_INPUTPATH) + 1u);
    memcpy(&dirInfo.out.path, FS_OUTPUTPATH, strlen(FS_OUTPUTPATH) + 1u);

    /* Mount SD Card */
    fresult = f_mount(&fs, "", (uint8_t)FS_MOUNTNOW);

    if(FR_OK == fresult)
    {
        sdCardInfo.state = FS_INITIALIZED;
    }
    else
    {
        sdCardInfo.state = FS_UNINITIALIZED;
    }
}


/* Main function for FS module */
void FS_Main(void)
{
    FRESULT fresult = FR_OK;

    /* Proceed only with properly mounted SD card */
    if(FS_INITIALIZED == sdCardInfo.state)
    {   
        fresult = FS_GetSdCardInfo();

        /* Reading info from SD card succeed. Everything is Ok. */
        if(FR_OK == fresult)
        {
            /* Read from from a test file */
            uint8_t retVal = FS_OpenFile(&files.in, "READ.TXT", FA_READ);
            if(RET_OK == retVal)
            {
                while(TRUE == files.in.isMoreLines)
                {
                    retVal |= FS_ReadFile(&files.in, fsBuffer, FS_BUFFSIZE);
                }

                retVal |= FS_CloseFile(&files.in);
            }

            /* Write data to a test file */
            retVal = FS_OpenFile(&files.out, "WRITE.TXT", FA_CREATE_ALWAYS | FA_WRITE);
            if(RET_OK == retVal)
            {
                char str[] = "TEST_FILE";
                memset(fsBuffer, 0u, sizeof(fsBuffer));
                memcpy(fsBuffer, str, strlen(str) + 1u);

                retVal = FS_WriteFile(&files.out, fsBuffer);
                retVal |= FS_CloseFile(&files.out);
            }
        }
        else if(FR_DISK_ERR == fresult)
        {
            /* Set state to uninitialized. FS_Init will be trigger in the next call. */
            sdCardInfo.state = FS_UNINITIALIZED;
        }
        else
        {
            /* To be done */
        }
    }
    else
    {
        FS_ReInit();
    }
}


/* Function called to get information
   about current SD card. */
FRESULT FS_GetSdCardInfo(void)
{
    FRESULT fresult = FR_OK;

    fresult |= FS_GetSDcardCapacity();
    fresult |= FS_ReadDir(&dirInfo.in);
    fresult |= FS_ReadDir(&dirInfo.out);

    return fresult;
}


/* Wrapper used to open a file.
   Mode is defined by the user, its default value
   is FA_READ. */
uint8_t FS_OpenFile(FS_File_T* file, FS_FullPathType path, uint8_t mode)
{
    uint8_t retVal = RET_OK;
    FRESULT fresult = FR_OK;

    /* Check path length */
    uint8_t len = strlen(path) + 1u;
    if(FS_FULLCHARLEN >= len)
    {
        /* Set mode to default FA_READ if no mode was declared before */
        if(0u == mode)
        {
            mode = FA_READ;
        }

        fresult |= f_open(&file->object, (TCHAR*)path, (BYTE)mode);
        if(FR_OK == fresult)
        {
            memcpy(file->name, path, len);
            file->lastLineNumber = 0u;
            file->isMoreLines = TRUE;
            file->isOpen = TRUE;
        }
        else
        {
            /* File can not be opened */
            retVal = RET_NOK;
        }
    }
    else
    {
        /* File path too long */
        retVal = RET_NOK;
    }

    return retVal;
}


/* Wrapper used to clse a file */
uint8_t FS_CloseFile(FS_File_T* file)
{
    uint8_t retVal = RET_OK;
    FRESULT fresult = FR_OK;

    fresult |= f_close(&file->object);
    if(FR_OK == fresult)
    {
        memset(file, 0u, sizeof(*file));
    }
    else
    {
        retVal = RET_NOK;
    }

    return retVal;
}


/* Function called to get one line from the file.
   It checks also whether there is more data to read. */
uint8_t FS_ReadFile(FS_File_T* file, uint8_t *buff, uint16_t len)
{
    memset(buff, 0u, len);
    TCHAR* bytesRead = f_gets((TCHAR*)buff, len, &file->object);

    file->isMoreLines = (bytesRead != NULL) ? TRUE : FALSE;
    file->lastLineNumber++;

    return RET_OK;
}


/* Function called to write data from the buffer 
   to the file.*/
uint8_t FS_WriteFile(FS_File_T* file, uint8_t *buff)
{
    int8_t wroteBytes = f_puts((TCHAR*)buff, &file->object);
    uint8_t retVal = (wroteBytes > 0) ? RET_OK : RET_NOK;

    return retVal;
}


/* Function called to get total size, free space and
   number of free cluster or given SD card */
FRESULT FS_GetSDcardCapacity(void)
{
    FRESULT fresult = f_getfree("", &sdCardInfo.numFreeClusters, &sdCardInfo.ptrFS);

    sdCardInfo.totalSpace = (uint32_t)(((sdCardInfo.ptrFS->n_fatent - 2u) * sdCardInfo.ptrFS->csize) / 2u);
    sdCardInfo.freeSpace = (uint32_t)((sdCardInfo.numFreeClusters * sdCardInfo.ptrFS->csize) / 2u);

    return fresult;
}


/* Function called to read given directory
   on a SD card. */
FRESULT FS_ReadDir(FS_Dir_T* dir)
{
    FRESULT fresult = FR_OK;
    DIR openedDir;
    FILINFO fileInfo;

    FS_FullPathType name[FS_SAVEDFILESNUM] = {0u};
    uint8_t number = 0u;

    fresult |= f_opendir(&openedDir, dir->path);
    if(FR_OK == fresult)
    {
        for(uint8_t i = 0u; i < FS_MAXFILESNUM; i++)
        {
            memset(&fileInfo, 0u, sizeof(fileInfo));
            fresult |= f_readdir(&openedDir, &fileInfo);

            if(0u != fileInfo.fsize)
            {
                /* Due to memory issues, save names only for the first FS_SAVEDFILESNUM files */
                if(i < FS_SAVEDFILESNUM)
                {
                    memcpy(name[i], fileInfo.fname, strlen(fileInfo.fname) + 1u);
                }
                number++;
            }
            else
            {
                /* No file left */
                break;
            }
            
        }
        fresult |= f_closedir(&openedDir);
    }

    /* Replace local variables with the global ones */
    memcpy(dir->fileName, name, sizeof(name));
    dir->filesNum = number;

    return fresult;
}


/* Function called when some fresult is not FR_OK.
   It tries to mount SD card again from higher level. */
FRESULT FS_ReInit(void)
{
    FRESULT fresult = FR_OK;

    fresult |= f_mount(0u, "", (uint8_t)FS_MOUNTNOW);
    memset(&fs, 0u, sizeof(fs));
    memset(&sdCardInfo, 0u, sizeof(sdCardInfo));

    /* Autogenerated functions */
    MX_FATFS_DeInit();
    MX_FATFS_Init();

    /* Local Init */
    FS_Init();

    return fresult;
}
