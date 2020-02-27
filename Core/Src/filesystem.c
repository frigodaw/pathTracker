/******************************************************************************
* @file           : filesystem.c
* @brief          : Source for filesystem.c file.
*                   This file contains the source code for files-related data
******************************************************************************/

#include "filesystem.h"
#include "types.h"
#include <string.h>

/* Global fatFS variable */
FATFS fs;           //file system
FIL fil;

char buffer[256];   //to store data

//UINT br, bw;        //file read/write count

/* capacity related variables */
FS_SDcardCapacity_T sdcardCapacity;


/* Init function for FS module to mount SD card and
   initialize varaibles */
void FS_Init(void)
{
    FRESULT fresult;

    /* Mount SD Card */
    fresult = f_mount(&fs, "", 0u);

    if(FR_OK == fresult)
    {
        /* Card capacity details. */
        fresult = FS_GetSDcardCapacity();
    }

}

/* Function called to get total size, free space and
   number of free cluster or given SD card */
FRESULT FS_GetSDcardCapacity(void)
{
    FRESULT fresult = f_getfree("", &sdcardCapacity.numFreeClusters, &sdcardCapacity.ptrFS);

    sdcardCapacity.totalSpace = (uint32_t)((sdcardCapacity.ptrFS->n_fatent - 2u) * sdcardCapacity.ptrFS->csize * 0.5);
    sdcardCapacity.freeSpace = (uint32_t)(sdcardCapacity.numFreeClusters * sdcardCapacity.ptrFS->csize * 0.5);

    return fresult;
}

/* Main function for FS module */
void FS_Main(void)
{
    //FRESULT fresult;
    //fresult = FS_GetSDcardCapacity();
    FS_ReadFile();

}


/* Function called to read test file */
void FS_ReadFile(void)
{
    FRESULT fresult;
    UINT read;

    /* Open file to read */
    fresult = f_open(&fil, "TEST.TXT", FA_READ);

    if(FR_OK == fresult)
    {
        /* Read string from the file */
        //f_gets(buffer, f_size(&fil), &fil);
        f_read (&fil, buffer, f_size(&fil), &read);

        /* Close file */
        f_close(&fil);
    }

  /* Close file */
  //f_close(&fil);

}


#if 0
  /************* The following operation is using PUTS and GETS *********************/


  /* Open file to write/ create a file if it doesn't exist */
  fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

  /* Writing text */
  fresult = f_puts("This data is from the First FILE\n\n", &fil);

  /* Close file */
  fresult = f_close(&fil);

  /* Open file to read */
  //fresult = f_open(&fil, "file1.txt", FA_READ);

  /* Read string from the file */
  //f_gets(buffer, fil.fsize, &fil);

  /* Close file */
  //f_close(&fil);

  /**************** The following operation is using f_write and f_read **************************/

  /* Create second file with read write access and open it */
  fresult = f_open(&fil, "file2.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

  /* Writing text */
  strcpy (buffer, "This is File 2 and it says Hello from controllerstech\n");

  fresult = f_write(&fil, buffer, sizeof(buffer), &bw);

  //send_uart ("File2.txt created and data is written\n");

  /* Close file */
  f_close(&fil);

  /* Open second file to read */
  //fresult = f_open(&fil, "file2.txt", FA_READ);

  /* Read data from the file
    * Please see the function details for the arguments */
  //f_read (&fil, buffer, fil.fsize, &br);

  /* Close file */
  //f_close(&fil);

  /*********************UPDATING an existing file ***************************/

  /* Open the file with write access */
  fresult = f_open(&fil, "file2.txt", FA_OPEN_ALWAYS | FA_WRITE);

  /* Move to offset to the end of the file */
  fresult = f_lseek(&fil, f_size(&fil));

  /* write the string to the file */
  fresult = f_puts("This is updated data and it should be in the end \n", &fil);

  f_close (&fil);

  /* Open to read the file */
  //fresult = f_open (&fil, "file2.txt", FA_READ);

  /* Read string from the file */
  //f_read (&fil, buffer, fil.fsize, &br);

  /* Close file */
  //f_close(&fil);

  /*************************REMOVING FILES FROM THE DIRECTORY ****************************/

  //fresult = f_unlink("/file1.txt");

  //fresult = f_unlink("/file2.txt");

  /* Unmount SDCARD */
  fresult = f_mount(NULL, "", 1);
#endif
