/******************************************************************************
* @file           : neoGps.c
* @brief          : Source for neoGps.c file.
*                   This file contains the source code of gps device.
******************************************************************************/

#include "neoGps.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/*  Global variables to store all GPS-related data */
GpsUartData_T gpsData = {0u};
GpsDebugData_T gpsDebug = {0u};


/*  Function called to set write pointer to proper value
    every time before HAL_UART_Receive_DMA(). 
*/
void Gps_PrepareWrite(void)
{
    if(gpsData.write >= gpsData.read)
    {
        /* Data fits between the write pointer and the end of the buffer */
        if((gpsData.write + GPS_MAX_NMEA_SIZE) <= GPS_RING_BUFFER_SIZE)
        {
            gpsData.state = GPS_OK_AHEAD;
        }
        /* Data fits between start of the buffer and the read pointer */
        else if(gpsData.read > GPS_MAX_NMEA_SIZE)
        {
            gpsData.write = 0u;
            gpsData.state = GPS_OK_BEHIND;
        }
        /* There is no space for data */
        else
        {
            gpsData.state = GPS_FULL;
        }
    }
    else    /* read > write */
    {
        /* Data fits between write and read pointer */
        if((gpsData.write + GPS_MAX_NMEA_SIZE) < gpsData.read)
        {
            gpsData.state = GPS_OK_BEHIND;
        }
        /* There is no space for data */
        else
        {
            gpsData.state = GPS_FULL;
        }
    }
}


/*  Main function for GPS module. It reads the data from buffer
    and divides them into separates variables.
*/
void Gps_Main(void)
{
    /* Set read pointer to the start of the data */
    while((gpsData.ringBuff[gpsData.read] != '$') && (gpsData.read != gpsData.write))
    {
        if((gpsData.state == GPS_OK_AHEAD) && (gpsData.read < gpsData.write))
        {
            gpsData.read++;
        }
        else if((gpsData.state == GPS_OK_BEHIND) && (gpsData.read > gpsData.write))
        {
            gpsData.read = ((gpsData.read + 1u) < GPS_RING_BUFFER_SIZE) ? (gpsData.read + 1u) : 0u;
        }
        else
        {
            /* Should not get here */
            gpsData.dummy++;
        }
    }
    
    if(gpsData.read != gpsData.write)
    {
        /* Only when character was found */
        if(((gpsData.read + GPS_NMEA_OFFSET_FIVE) < GPS_RING_BUFFER_SIZE) &&
            (gpsData.ringBuff[gpsData.read] == '$') && 
            (gpsData.ringBuff[gpsData.read + GPS_NMEA_OFFSET_ONE] == 'G') && 
            (gpsData.ringBuff[gpsData.read + GPS_NMEA_OFFSET_TWO] == 'P'))
        {
            switch(Gps_SelectMsg())
            {
                case GPS_GPRMC:
                    break;
                case GPS_GPVTG:
                    break;
                case GPS_GPGGA:
                    Gps_ReadGPGGA();
                    break;
                case GPS_GPGSA:
                    break;
                case GPS_GPGSV:
                    break;
                case GPS_GPGLL:
                    break;
                case GPS_ERROR:
                default:
                    gpsData.read++;
                    break;
            }
        }
        else
        {
            gpsData.read = ((gpsData.read + 1u) < GPS_RING_BUFFER_SIZE) ? (gpsData.read + 1u) : 0u;
        }
    }
    else
    {
        /* Wait for new data. */
    }
    
    Gps_PrepareDebugData();
}


/*  Function called to distinguish which type of data
    is already encountered.
*/
uint8_t Gps_SelectMsg(void)
{
    uint8_t msgType = GPS_ERROR;
    const char *msgArray[GPS_NMEA_MSG_NUM] = {"GPRMC", "GPVTG", "GPGGA", "GPGSA", "GPGSV", "GPGLL"};

    for(enum Gps_msgType i=0u; i < GPS_NMEA_MSG_NUM; i++)
    {
        if(strncmp((char*)&gpsData.ringBuff[gpsData.read + GPS_NMEA_OFFSET_ONE], msgArray[i], GPS_NMEA_WORD_LEN) == 0)
        {
            msgType = i;
            gpsData.read += GPS_NMEA_OFFSET_FIVE;   //read points on the last letter of the NMEA msg string
            break;
        }
    }
    return msgType;
}


/* Function called to get data from GPGGA msg. Example:
   $GPGGA,160915.00,5003.42292,N,01855.72499,E,1,08,1.01,240.5,M,40.7,M,,*5F 
*/
void Gps_ReadGPGGA(void)
{
    uint8_t fieldBuff[GPS_FIELD_BUFFER_SIZE] = {0u};
    enum Gps_gpggaDataSequence elementNum = GPS_GPGGA_TIME;

    /* Set the read indicator to first ',' and write value to local variable newRead */
    uint16_t newRead = ++gpsData.read;
    uint16_t len = 0u;
    uint32_t timeLinked = 0u;

    while(elementNum < GPS_GPPGA_ELEMENTS)
    {
        /* Increment to the next ',' */
        do
        {
            newRead++;
        }
        while((newRead < GPS_RING_BUFFER_SIZE) &&
             (gpsData.ringBuff[newRead] != '$') && (gpsData.ringBuff[newRead] != '*') &&
             (gpsData.ringBuff[newRead] != ','));

        /* Calculate field length */
        len = newRead - gpsData.read;

        /* Check whether the read is not too high */
        if(newRead >= GPS_RING_BUFFER_SIZE)
        {
            gpsData.read = 0u;
            elementNum = GPS_GPPGA_ELEMENTS;
        }
        /* Check whether newRead is not higher then write during AHEAD state */
        else if((newRead > gpsData.write) && (gpsData.state == GPS_OK_AHEAD))
        {
            gpsData.read = gpsData.write;
            elementNum = GPS_GPPGA_ELEMENTS;
        }
        /* Check for the end of current message */
        else if((gpsData.ringBuff[newRead] == '$') || (gpsData.ringBuff[newRead] == '*'))
        {
            gpsData.read += len;
            elementNum = GPS_GPPGA_ELEMENTS;
        }
        /* Found ',' character. */
        else if(gpsData.ringBuff[newRead] == ',')
        {
            /* Omit empty data */
            if(len > 1u)
            {
                /* Check whether data fits into the ring buffer */
                if((newRead <= GPS_RING_BUFFER_SIZE) && (len <= GPS_FIELD_BUFFER_SIZE))
                {
                    memset(fieldBuff, 0u, GPS_FIELD_BUFFER_SIZE);
                    for(uint8_t i=0u; i < (len - 1u); i++)
                    {
                        fieldBuff[i] = gpsData.ringBuff[gpsData.read + 1u + i];
                    }

                    switch(elementNum)
                    {
                        case GPS_GPGGA_TIME:
                            memset(&fieldBuff[GPS_GPPGA_TIME_LEN], 0u, GPS_FIELD_BUFFER_SIZE - GPS_GPPGA_TIME_LEN);
                            sscanf((char*)fieldBuff, "%ld", &timeLinked);
                            gpsData.timeSec  = timeLinked % GPS_GPPGA_TIME_COEFF1;
                            gpsData.timeMin = (timeLinked / GPS_GPPGA_TIME_COEFF1) % GPS_GPPGA_TIME_COEFF1;
                            gpsData.timeHr = (timeLinked / GPS_GPPGA_TIME_COEFF2) % GPS_GPPGA_TIME_COEFF1;
                            break;
                        case GPS_GPGGA_LATITUDE:
                            sscanf((char*)fieldBuff, "%f", &gpsData.latitude);
                            break;
                        case GPS_GPGGA_NS:
                            sscanf((char*)fieldBuff, "%c", &gpsData.latDir);
                            break;
                        case GPS_GPGGA_LONGITUDE:
                            sscanf((char*)fieldBuff, "%f", &gpsData.longitude);
                            break;
                        case GPS_GPGGA_WE:
                            sscanf((char*)fieldBuff, "%c", &gpsData.lonDir);
                            break;
                        case GPS_GPGGA_FIXQUALITY:
                            sscanf((char*)fieldBuff, "%hhu", &gpsData.fixQuality);
                            break;
                        case GPS_GPGGA_SATELITESNUM:
                            sscanf((char*)fieldBuff, "%hhu", &gpsData.satelitesNum);
                            break;
                        case GPS_GPGGA_DILUTION:
                            sscanf((char*)fieldBuff, "%f", &gpsData.dilution);
                            break;
                        case GPS_GPGGA_ALTITUDE:
                            sscanf((char*)fieldBuff, "%f", &gpsData.altitude);
                            break;
                        default:
                            break;
                    }

                    gpsData.read = newRead;
                    elementNum++;
                }
                /* Buffer overflow. End the function. */
                else
                {
                    gpsData.read = (newRead < GPS_RING_BUFFER_SIZE) ? newRead : 0u;
                    elementNum = GPS_GPPGA_ELEMENTS;
                }
            }
            /* If there is no data in new msg, go to next step. */
            else
            {
                gpsData.read = newRead;
                elementNum++;
            }

        }
        /* Cannot find character, do not process the data. End the function. */
        else
        {
            gpsData.read = newRead;
            elementNum = GPS_GPPGA_ELEMENTS;
        }
    }
}


/* Function called to prepare buffer which will be send 
   via USB.
*/
void Gps_PrepareDebugData(void)
{
    memset(gpsDebug.buffer, 0u, GPS_DEBUG_BUFF_SIZE);
    gpsDebug.size = snprintf((char*)gpsDebug.buffer, GPS_DEBUG_BUFF_SIZE, 
        "Read: %d,  Write: %d,  Time: %d:%d:%d,  Lat: NONE,  Dir: %c,  Lon: NONE,  Dir: %c,  Alti: NONE,  fixQual: %d\n",
        gpsData.read, gpsData.write, gpsData.timeHr, gpsData.timeMin, gpsData.timeSec,
        gpsData.latDir, gpsData.lonDir, gpsData.fixQuality);
}
