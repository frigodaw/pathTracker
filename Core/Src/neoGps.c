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
            gpsData.state = GPS_OK;
        }
        /* Data fits between start of the buffer and the read pointer */
        else if(gpsData.read >= GPS_MAX_NMEA_SIZE)
        {
            gpsData.write = 0u;
            gpsData.state = GPS_OK;
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
        if((gpsData.write + GPS_MAX_NMEA_SIZE) <= gpsData.read)
        {
            gpsData.state = GPS_OK;
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
    /* Set write pointer to the start of the data */
    while(gpsData.ringBuff[gpsData.read] != '$')
    {
        if(gpsData.read < (GPS_RING_BUFFER_SIZE - 1u))
        {
            gpsData.read++;
        }
        else
        {
            gpsData.read = 0u;
        }
    }

    /* Only when character was found */
    if((gpsData.ringBuff[gpsData.read] == '$') && (gpsData.ringBuff[gpsData.read + 1u] == 'G') && (gpsData.ringBuff[gpsData.read + 2u] == 'P'))
    {
        gpsData.read += 2u;
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
                break;
            default:
                break;
        }
    }
    else
    {
        gpsData.read++;
    }
}


/*  Function called to distinguish which type of data
    is already encountered.
*/
uint8_t Gps_SelectMsg(void)
{
    uint8_t msgType = GPS_ERROR;

    switch(gpsData.ringBuff[gpsData.read + 1u])
    {
        case 'R':
            msgType = GPS_GPRMC;
            break;
        case 'V':
            msgType = GPS_GPVTG;
            break;
        case 'G':
            switch(gpsData.ringBuff[gpsData.read + 2u])
            {
                case 'G':
                    msgType = GPS_GPGGA;
                    break;
                case 'L':
                    msgType = GPS_GPGLL;
                    break;
                case 'S':
                    switch(gpsData.ringBuff[gpsData.read + 3u])
                    {
                        case 'A':
                            msgType = GPS_GPGSA;
                            break;
                        case 'V':
                            msgType = GPS_GPGSV;
                            break;
                        default:
                            msgType = GPS_ERROR;
                            break;
                    }
                    break;
                default:
                    msgType = GPS_ERROR;
                    break;
            }
            break;
        default:
            msgType = GPS_ERROR;
            break;
    }
    gpsData.read += 3u;     //read points on the last letter of the NMEA msg string

    return msgType;
}


/* Function called to get data from GPGGA msg. Example:
   $GPGGA,160915.00,5003.42292,N,01855.72499,E,1,08,1.01,240.5,M,40.7,M,,*5F 
*/
void Gps_ReadGPGGA(void)
{
    uint8_t fieldBuff[GPS_FIELD_BUFFER_SIZE] = {0u};
    uint8_t elementNum = GPS_GPGGA_TIME;

    /* Set the read indicator to first ',' and write value to local variable newRead */
    uint16_t newRead = ++gpsData.read;
    uint8_t len = 0u;
    uint32_t timeLinked = 0u;


    while(elementNum < GPS_GPPGA_ELEMENTS)
    {
        /* Move endRead to the next ',' */
        do
        {
            newRead++;
        }
        while((gpsData.ringBuff[newRead] != ',') && (newRead < GPS_RING_BUFFER_SIZE));

        /* Found ',' character. */
        if(gpsData.ringBuff[newRead] == ',')
        {
            len = newRead - gpsData.read;

            /* Omit empty data */
            if(len > 1u)
            {
                /* Check whether data fits into the ring buffer */
                if((gpsData.read + len <= GPS_RING_BUFFER_SIZE) && (len <= GPS_FIELD_BUFFER_SIZE))
                {
                    memset(fieldBuff, 0u, GPS_FIELD_BUFFER_SIZE);
                    for(uint8_t i=0u; i < len; i++)
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
                            sscanf((char*)fieldBuff, "%lf", &gpsData.latitude);
                            break;
                        case GPS_GPGGA_NS:
                            sscanf((char*)fieldBuff, "%c", &gpsData.latDir);
                            break;
                        case GPS_GPGGA_LONGITUDE:
                            sscanf((char*)fieldBuff, "%lf", &gpsData.longitude);
                            break;
                        case GPS_GPGGA_WE:
                            sscanf((char*)fieldBuff, "%c", &gpsData.lonDir);
                            break;
                        default:
                            break;
                    }
                }
                /* If there is no space for whole field buffer reset gpsData.read to 0u and finish the function. */
                else
                {
                    gpsData.read = 0u;
                    elementNum = GPS_GPPGA_ELEMENTS;
                }
            }

            gpsData.read += len;
            elementNum++;
        }
        /* Cannot find character, do not process the data. */
        else
        {
            elementNum = GPS_GPPGA_ELEMENTS;
        }
    }
}
