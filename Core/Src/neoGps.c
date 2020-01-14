/******************************************************************************
* @file           : neoGps.c
* @brief          : Source for neoGps.c file.
*                   This file contains the source code of gps device.
******************************************************************************/

#include "neoGps.h"
#include <string.h>
#include <stdio.h>

uint8_t gpsBuff[GPS_MSG_MAX_SIZE] = {0};
char lonGpsBuff[GPS_VAR_LEN] = {0};
char latGpsBuff[GPS_VAR_LEN] = {0};
char timeGpsBuff[GPS_VAR_LEN] = {0};

GpsUartData_T gpsData = {0};
volatile uint8_t recvChar = 0;

void Gps_SplitFrame(void)
{
    char* gpsBuffPtr = strstr((char*)gpsBuff, GPS_GPGGA);
    char* tmpBuffer;
    uint8_t sequence = GPS_GPGGA_ID;
    if(gpsBuffPtr)
    {
        while(NULL != (tmpBuffer = strsep(&gpsBuffPtr, ",")))
        {
            switch (sequence)
            {
            case GPS_GPGGA_TIME:
                strncpy(timeGpsBuff, tmpBuffer, 6);
                break;
            case GPS_GPGGA_LATITUDE:
                //strncpy(latGpsBuff, tmpBuffer, 8);
                //sscanf(tmpBuffer, "%lf", latGpsBuff);
                //Gps_FormatConverter(&Gps_mainData.data[*idx].latitude);
                break;
            case GPS_GPGGA_NS:
                //sscanf(tmpBuffer, "%c", &Gps_mainData.data[*idx].NS);
                break;
            case GPS_GPGGA_LONGITUDE:
                //strncpy(lonGpsBuff, tmpBuffer, 8);
                //sscanf(tmpBuffer, "%lf", lonGpsBuff);
                //Gps_FormatConverter(&Gps_mainData.data[*idx].longitude);
                break;
            case GPS_GPGGA_WE:
                //sscanf(tmpBuffer, "%c", &Gps_mainData.data[*idx].WE);
                break;
            case GPS_GPGGA_FIXQUALITY:
                //sscanf(tmpBuffer, "%d", &Gps_mainData.data[*idx].fixQuality);
                break;
            case GPS_GPGGA_SATELITESNUM:
                //sscanf(tmpBuffer, "%d", &Gps_mainData.data[*idx].satelitesNum);
                break;
            case GPS_GPGGA_DILUTION:
                //sscanf(tmpBuffer, "%f", &Gps_mainData.data[*idx].dilution);
                break;
            case GPS_GPGGA_ALTITUDE:
                //sscanf(tmpBuffer, "%f", &Gps_mainData.data[*idx].altitude);
                break;
            default:
                break;
            }
            sequence++;
        }
    }
}

void Gps_Main(void)
{
    Gps_ProcessLine();
}

void Gps_ProcessLine(void)
{
    gpsData.read = 0u;
    Gps_ReadField();

         if(strcmp((char*)gpsData.fieldBuff, "GPRMC") == 0) Gps_ProcessReadout_GPRMC();
    else if(strcmp((char*)gpsData.fieldBuff, "GPVTG") == 0) Gps_ProcessReadout_GPVTG();
    else if(strcmp((char*)gpsData.fieldBuff, "GPGGA") == 0) Gps_ProcessReadout_GPGGA();
    else if(strcmp((char*)gpsData.fieldBuff, "GPGSA") == 0) Gps_ProcessReadout_GPGSA();
    
}

void Gps_ReadField(void)
{
    gpsData.fieldPos = 0u;

    while((gpsData.dataBuff[gpsData.read] != ',')
        && (gpsData.dataBuff[gpsData.read] != '\0')
        && (gpsData.fieldPos < GPS_FIELD_BUFF_SIZE))
    {
        gpsData.fieldBuff[gpsData.fieldPos] = gpsData.dataBuff[gpsData.read];
        gpsData.read++;
        gpsData.fieldPos++;
    }

    gpsData.dataBuff[gpsData.fieldPos] = '\0';
    gpsData.read++;
}

void Gps_ProcessReadout_GPRMC(void) {}
void Gps_ProcessReadout_GPVTG(void) {}
void Gps_ProcessReadout_GPGSA(void) {}
void Gps_ProcessReadout_GPGGA(void) 
{
    Gps_ReadField();
    if(strlen((char*)gpsData.fieldBuff) > 0)
    {
        uint32_t tmp;
        sscanf(gpsData.fieldBuff, "%d", tmp);
        gpsData.timeSec = tmp % 100;
        gpsData.timeMin = (tmp / 100) % 100;
        gpsData.timeHr = (tmp / 10000) % 100;
    }
}
