/******************************************************************************
* @file           : filesystem.c
* @brief          : Source for filesystem.c file.
*                   This file contains the source code for files-related data
******************************************************************************/

#include "filesystem.h"
#include <string.h>
#include <stdio.h>

/* Main file system variable */
static FATFS fs = {0u};

/* Variable to store currently opened read and write file */
static FS_FilesCollection_T files = {0u};

/* Global variable with the main info about current SD card */
FS_SDcardInfo_T sdCardInfo = {0u};

/* Global variable with a list of directories and a files inside them */
FS_DirsCollection_T dirInfo = {0u};

/* Init function for FS module to mount SD card and
   initialize varaibles */
void FS_Init(void)
{
    FRESULT fresult = FR_OK;
    sdCardInfo.blocked = FALSE;

    /* Set path to 'in' and 'out' directories */
    memcpy(&dirInfo.in.path, FS_INPUTPATH, strlen(FS_INPUTPATH) + 1u);
    memcpy(&dirInfo.out.path, FS_OUTPUTPATH, strlen(FS_OUTPUTPATH) + 1u);
    memcpy(&dirInfo.reports.path, FS_REPORTSPATH, strlen(FS_REPORTSPATH) + 1u);

    /* Mount SD Card */
    fresult = f_mount(&fs, "", (uint8_t)FS_MOUNTNOW);

    if(FR_OK == fresult)
    {
        sdCardInfo.state = FS_INITIALIZED;
    }
    else
    {
        FS_ClearSdCardData();
    }
}


/* Main function for FS module */
void FS_Main(void)
{
    FRESULT fresult = FR_OK;

    /* Proceed only with properly mounted SD card. */
    if(FS_INITIALIZED == sdCardInfo.state)
    {   
        fresult = FS_GetSdCardInfo();

        /* Reading info from SD card succeed. Everything is Ok. */
        if(FR_OK == fresult)
        {
            /* OK */
        }
        else if(FR_DISK_ERR == fresult)
        {
            /* Set state to uninitialized. FS_Init will be trigger in the next call. */
            FS_ClearSdCardData();
        }
        else
        {
            static uint8_t errorCnt = 0u;
            errorCnt++;

            if(errorCnt >= FS_MAXFAILSNUM)
            {
                errorCnt = 0u;
                FS_ClearSdCardData();
            }
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

    if(FALSE == sdCardInfo.blocked)
    {
        fresult |= FS_GetSDcardCapacity();
        fresult |= FS_ReadDir(&dirInfo.in);
        fresult |= FS_ReadDir(&dirInfo.out);
        fresult |= FS_ReadDir(&dirInfo.reports);
    }

    return fresult;
}


/* Wrapper used to open a file.
   Mode is defined by the user. */
uint8_t FS_OpenFile(FS_File_T** file, FS_FullPathType path, FS_fileMode mode)
{
    uint8_t retVal = RET_OK;
    FRESULT fresult = FR_OK;
    uint8_t fileMode = 255u;

    switch (mode)
    {
        case FS_MODEREAD:
            fileMode = FA_READ;
            *file = &(files.in);
            break;
        case FS_MODEWRITE:
            fileMode = FA_READ | FA_WRITE | FA_CREATE_ALWAYS;
            *file = &(files.out);
            break;
        case FS_MODEAPPEND:
            fileMode = FA_READ | FA_WRITE | FA_OPEN_APPEND;
            *file = &(files.out);
            break;
        
        default:
            retVal = RET_NOK;
            fresult = FR_INVALID_PARAMETER;
            break;
    }

    if(RET_OK == retVal)
    {
        /* Check path length */
        uint8_t len = strlen(path) + 1u;
        if(FS_FULLCHARLEN >= len)
        {
            if(FALSE != (*file)->isOpen)
            {
                FS_CloseFile((FS_File_T**)file);
            }

            /* Open file */
            fresult |= f_open((FIL*)&(*file)->object, (TCHAR*)path, (BYTE)fileMode);
            if(FR_OK == fresult)
            {
                memcpy((*file)->name, path, len);
                (*file)->lastLineNumber = 0u;
                (*file)->isMoreLines = TRUE;
                (*file)->isOpen = TRUE;
                (*file)->mode = mode;
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
    }
    else
    {
        /* Wrong mode parameter */
        retVal = RET_NOK;
    }

    return retVal;
}


/* Wrapper used to close a file */
uint8_t FS_CloseFile(FS_File_T** file)
{
    uint8_t retVal = RET_OK;
    FRESULT fresult = FR_OK;

    fresult |= f_close((FIL*)&(*file)->object);
    memset(*file, 0u, sizeof(**file));

    if(FR_OK != fresult)
    {
        retVal = RET_NOK;
    }

    return retVal;
}


/* Function called to rename existing file in a runtime. */
uint8_t FS_RenameFile(FS_File_T** file, FS_FullPathType newPath)
{
    uint8_t retVal = RET_NOK;
    FRESULT fresult = FR_OK;

    uint8_t len = strlen(newPath);

    if(len > 0u)
    {
        boolean reopen = FALSE;
        FS_fileMode mode = FS_NOMODE;

        /* Opened file need to be closes before renaming */
        if(TRUE == (*file)->isOpen)
        {
            reopen = TRUE;
            mode = (*file)->mode;
            FS_FullPathType oldPath;
            memcpy(&oldPath, (*file)->name, sizeof(FS_FullPathType));
            retVal = FS_CloseFile(file);
        }

        fresult = f_rename((const TCHAR*)(*file)->name, (const TCHAR*)newPath);

        /* Reopen file again if was opened before */
        if(TRUE == reopen)
        {
            retVal = FS_OpenFile(file, newPath, mode);
        }

        if(FR_OK == fresult)
        {
            retVal = RET_OK;
        }
    }

    return retVal;
}


/* Function called to move file pointer. */
uint8_t FS_Lseek(FS_File_T** file, uint32_t offset)
{
    uint8_t retVal = RET_OK;
    FRESULT fresult = FR_OK;

    if(offset > 0)
    {
        fresult |= f_lseek((FIL*)&(*file)->object, (FSIZE_t)offset);
    }

    if(FR_OK != fresult)
    {
        retVal = RET_NOK;
    }

    return retVal;
}


/* Function called to move file pointer to the very end of file. */
uint8_t FS_LseekEnd(FS_File_T** file)
{
    uint8_t retVal = RET_OK;
    FRESULT fresult = FR_OK;
    FSIZE_t offset = (*file)->object.obj.objsize;

    if(offset > 0)
    {
        fresult |= f_lseek((FIL*)&(*file)->object, (FSIZE_t)offset);
    }

    if(FR_OK != fresult)
    {
        retVal = RET_NOK;
    }

    return retVal;
}


/* Function called to get one line from the file.
   It checks also whether there is more data to read. */
uint8_t FS_ReadFile(FS_File_T* file, uint8_t *buff, uint16_t len, boolean *isMoreLines)
{
    memset(buff, 0u, len);
    TCHAR* bytesRead = f_gets((TCHAR*)buff, (int)len, &file->object);
    file->lastLineNumber++;

    if(bytesRead != NULL)
    {
        file->isMoreLines = TRUE;
        *isMoreLines = TRUE;
    }
    else
    {
        file->isMoreLines = FALSE;
        *isMoreLines = FALSE;
    }

    return RET_OK;
}


/* Function called to find file with index 'idx'
   and returns its name. */
uint8_t FS_FindInputFile(uint8_t idx, FS_FullPathType filePath, uint8_t *fileDisplayPath, uint8_t maxLen)
{
    uint8_t retVal = RET_OK;
    FRESULT fresult = FR_OK;
    DIR openedDir = {0u};
    FILINFO fileInfo = {0u};

    memset(filePath, 0u, sizeof(FS_FullPathType));
    memset(fileDisplayPath, 0u, sizeof(*fileDisplayPath)*maxLen);

    fresult = f_findfirst(&openedDir, &fileInfo, (const char*)dirInfo.in.path, "*.gpx");

    if(idx > FS_INPUTFILE_FIRST_ITEM)
    {
        for(uint8_t i = FS_INPUTFILE_FIRST_ITEM; i < idx; i++)
        {
            memset(fileInfo.fname, 0u, sizeof(FS_FullPathType));
            fresult = f_findnext(&openedDir, &fileInfo);
        }
    }

    if((FR_OK == fresult) && (0u != fileInfo.fname[0u]))
    {
        /* Save full path */
        uint8_t num = strlen((const char*)dirInfo.in.path);
        strncpy(filePath, (const char*)dirInfo.in.path, num);
        strncat(filePath, "/", FS_SEPARATOR_LEN);
        strncat(filePath, fileInfo.fname, sizeof(FS_FullPathType) - num - FS_SEPARATOR_LEN);

        /* Remove file extension to display path */
        uint8_t len = strlen(fileInfo.fname);
        memset(&fileInfo.fname[len - FS_FILEEXT_DELETE], 0u, FS_FILEEXT_DELETE);
        len -= FS_FILEEXT_DELETE;

        len = (len > maxLen) ? maxLen : len;

        strncpy((char*)fileDisplayPath, fileInfo.fname, len);
        retVal = RET_OK;
    }
    else
    {
        retVal = RET_NOK;
    }

    fresult = f_closedir(&openedDir);

    return retVal;
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
    DIR openedDir = {0u};
    FILINFO fileInfo = {0u};

    //FS_FullPathType name[FS_SAVEDFILESNUM] = {0u};
    uint8_t number = 0u;

    fresult |= f_opendir(&openedDir, dir->path);
    if(FR_OK == fresult)
    {
        for(uint8_t i = 0u; i < FS_MAXFILESNUM; i++)
        {
            memset(&fileInfo, 0u, sizeof(fileInfo));
            fresult |= f_readdir(&openedDir, &fileInfo);

            if((0u != fileInfo.fsize) || (0u != fileInfo.fdate) || (0u != fileInfo.ftime) || (0u != fileInfo.fattrib))
            {
                /* Due to memory issues, save names only for the first FS_SAVEDFILESNUM files */
                //if(i < FS_SAVEDFILESNUM)
                //{
                //    memcpy(name[i], fileInfo.fname, strlen(fileInfo.fname) + 1u);
                //}
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
    //memcpy(dir->fileName, name, sizeof(name));
    dir->filesNum = number;
    //dir->state = fresult;

    return fresult;
}


/* Function called when some fresult is not FR_OK.
   It tries to mount SD card again from higher level. */
FRESULT FS_ReInit(void)
{
    FRESULT fresult = FR_OK;

    fresult |= f_mount(0u, "", (uint8_t)FS_MOUNTNOW);
    memset(&fs, 0u, sizeof(fs));
    FS_ClearSdCardData();

    /* Auto-generated functions */
    MX_FATFS_DeInit();
    MX_FATFS_Init();

    /* Local Init */
    FS_Init();

    /* Restore SD card data */
    if(FS_INITIALIZED == sdCardInfo.state)
    {
        fresult |= FS_GetSdCardInfo();
    }

    return fresult;
}

/* Function called to clear all data shown on a screen
   when sd card is not initialized. */
FRESULT FS_ClearSdCardData(void)
{
    memset(&sdCardInfo, 0u, sizeof(sdCardInfo));
    memset(&dirInfo, 0u, sizeof(dirInfo));
    sdCardInfo.state = FS_UNINITIALIZED;

    return FR_OK;
}