/******************************************************************************
* @file           : neoGps.c
* @brief          : Source for neoGps.c file.
*                   This file contains the source code of gps device.
******************************************************************************/

#include "neoGps.h"
//#include <string.h>

uint8_t gpsBuff[UART_GPS_MSG_SIZE];
uint8_t lonGpsBuff[GPS_VAR_LEN];
uint8_t latGpsBuff[GPS_VAR_LEN];
uint8_t timeGpsBuff[GPS_VAR_LEN];

void Gps_SplitFrame(uint8_t* gpsBuff, uint8_t* lonGpsBuff, uint8_t* latGpsBuff, uint8_t* timeGpsBuff)
{
	if(gpsBuff[5] == 65)
	{
		for(uint8_t i=7; i<13; i++)
			lonGpsBuff[i-7] = gpsBuff[i];
	}



   /*if(!strncmp((char *)gpsBuff, GPS_GPGGA, GPS_SENTENCELENGTH))
   {
	 char *tmpBuffer;
	 uint8_t sequence = GPS_GPGGA_ID;

	 while(NULL != (tmpBuffer = strsep(&gpsBuff,",")))
	 {
		//printf("tmpBuffer: %s\n", tmpBuffer);
		switch (sequence)
		{
		   case GPS_GPGGA_TIME:
			  strncpy(timeGpsBuff, tmpBuffer, 6);
			  break;
		   case GPS_GPGGA_LATITUDE:
			  strncpy(latGpsBuff, tmpBuffer, 8);
			  //sscanf(tmpBuffer, "%lf", latGpsBuff);
			  //Gps_FormatConverter(&Gps_mainData.data[*idx].latitude);
			  break;
		   case GPS_GPGGA_NS:
			  //sscanf(tmpBuffer, "%c", &Gps_mainData.data[*idx].NS);
			  break;
		   case GPS_GPGGA_LONGITUDE:
			   strncpy(lonGpsBuff, tmpBuffer, 8);
			  //sscanf(tmpBuffer, "%lf", lonGpsBuff);
			  //Gps_FormatConverter(&Gps_mainData.data[*idx].longitude);
			  break;
		   case GPS_GPGGA_WE:
			  sscanf(tmpBuffer, "%c", &Gps_mainData.data[*idx].WE);
			  break;
		   case GPS_GPGGA_FIXQUALITY:
			  sscanf(tmpBuffer, "%d", &Gps_mainData.data[*idx].fixQuality);
			  break;
		   case GPS_GPGGA_SATELITESNUM:
			  sscanf(tmpBuffer, "%d", &Gps_mainData.data[*idx].satelitesNum);
			  break;
		   case GPS_GPGGA_DILUTION:
			  sscanf(tmpBuffer, "%f", &Gps_mainData.data[*idx].dilution);
			  break;
		   case GPS_GPGGA_ALTITUDE:
			  sscanf(tmpBuffer, "%f", &Gps_mainData.data[*idx].altitude);
			  break;
		   default:
			  break;
		}
		sequence++;
	 }
   }*/
}
