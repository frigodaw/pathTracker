/******************************************************************************
* @file           : neoGps.c
* @brief          : Source for neoGps.c file.
*                   This file contains the source code of gps device.
******************************************************************************/

#include "neoGps.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

/*  Global variables to store all GPS-related data */
GpsUartData_T gpsData = {0u};
GpsDebugData_T gpsDebug = {0u};


/*  Function called to set write pointer to proper value
    every time before HAL_UART_Receive_DMA(). 
*/
uint8_t Gps_PrepareWrite(void)
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

    return RET_OK;
}


/*  Main function for GPS module. It reads the data from buffer
    and divides them into separates variables.
*/
uint8_t Gps_Main(void)
{
    uint8_t ret = RET_OK;
    GpsMsgInfo_T messageInfo;

    /* Set read pointer to the start of the data */
    while((ret == RET_OK) && (gpsData.ringBuff[gpsData.read] != '$') && (gpsData.read != gpsData.write))
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
            gpsData.errorCnt++;
            gpsData.read = gpsData.write;
            ret = RET_NOK;
        }
    }
    
    if((ret == RET_OK) && (gpsData.read != gpsData.write))
    {
        /* Only when character was found */
        if(((gpsData.read + GPS_NMEA_OFFSET_FIVE) < GPS_RING_BUFFER_SIZE) &&
            (gpsData.ringBuff[gpsData.read] == '$') && 
            (gpsData.ringBuff[gpsData.read + GPS_NMEA_OFFSET_ONE] == 'G') && 
            (gpsData.ringBuff[gpsData.read + GPS_NMEA_OFFSET_TWO] == 'P'))
        {   
            Gps_SelectMsg(&messageInfo);
        }
        
        /* Go ahead if searching messages fails */
        if(messageInfo.type == GPS_ERROR)
        {
            gpsData.read = ((gpsData.read + 1u) < GPS_RING_BUFFER_SIZE) ? (gpsData.read + 1u) : 0u;
        }
        /* Read proper message */
        else
        {
            Gps_ReadMessage(&messageInfo);
        }
        
    }
    else
    {
        /* Wait for new data. */
    }
    
    Gps_PrepareDebugData();

    return ret;
}


/*  Function called to distinguish which type of data
    is already encountered.
*/
uint8_t Gps_SelectMsg(GpsMsgInfo_T* messageInfo)
{
    uint8_t ret = RET_NOK;
    const char *msgTypeArray[GPS_NMEA_MSG_NUM] = {"GPRMC", "GPVTG", "GPGGA", "GPGSA", "GPGSV", "GPGLL"};
    const uint8_t msgSizeArray[GPS_NMEA_MSG_NUM] = {GPS_SIZE_GPRMC, GPS_SIZE_GPVTG, GPS_SIZE_GPPGA, GPS_SIZE_GPGSA, GPS_SIZE_GPGSV, GPS_SIZE_GPGLL};

    messageInfo->type = GPS_ERROR;
    messageInfo->maxElements = 0u;

    for(enum Gps_msgType msgId=0u; msgId < GPS_NMEA_MSG_NUM; msgId++)
    {
        if(strncmp((char*)&gpsData.ringBuff[gpsData.read + GPS_NMEA_OFFSET_ONE], msgTypeArray[msgId], GPS_NMEA_WORD_LEN) == 0)
        {
            messageInfo->type = msgId;
            messageInfo->maxElements = msgSizeArray[msgId];
            ret = RET_OK;
            gpsData.read += GPS_NMEA_OFFSET_FIVE;   //read points on the last letter of the NMEA msg string
            break;
        }
    }
    return ret;
}

/* Function called to get data from GPS messages.
*/
uint8_t Gps_ReadMessage(GpsMsgInfo_T* messageInfo)
{
    uint8_t ret = RET_OK;
    uint16_t len = 0u;
    uint8_t fieldBuff[GPS_FIELD_BUFFER_SIZE] = {0u};

    /* Set the read indicator to first ',' and write value to local variable newRead */
    uint16_t newRead = ++gpsData.read;
    messageInfo->currentElement = 0u;

    while(messageInfo->currentElement < messageInfo->maxElements)
    {
        /* Increment to the next ',' */
        do
        {
            newRead++;
        }
        while((newRead < GPS_RING_BUFFER_SIZE) &&
             (gpsData.ringBuff[newRead] != '$') && (gpsData.ringBuff[newRead] != '\n') &&
             (gpsData.ringBuff[newRead] != ','));

        /* Calculate field length */
        len = newRead - gpsData.read;

        /* Check whether the read is not too high */
        if(newRead >= GPS_RING_BUFFER_SIZE)
        {
            gpsData.read = 0u;
            messageInfo->currentElement = messageInfo->maxElements;
        }
        /* Check whether newRead is not higher than write during AHEAD state */
        else if((newRead > gpsData.write) && (gpsData.state == GPS_OK_AHEAD))
        {
            gpsData.read = gpsData.write;
            messageInfo->currentElement = messageInfo->maxElements;
        }
        /* Check for the end of current message */
        else if((gpsData.ringBuff[newRead] == '$') || (gpsData.ringBuff[newRead] == '\n'))
        {
            gpsData.read += len;
            messageInfo->currentElement = messageInfo->maxElements;
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

                    switch (messageInfo->type)
                    {
                        case GPS_GPRMC:
                            ret |= Gps_ReadMessage_GPRMC(messageInfo->currentElement, fieldBuff);
                            break;
                        case GPS_GPVTG:
                            ret |= Gps_ReadMessage_GPVTG(messageInfo->currentElement, fieldBuff);
                            break;
                        case GPS_GPGGA:
                            ret |= Gps_ReadMessage_GPGGA(messageInfo->currentElement, fieldBuff);
                            break;
                        case GPS_GPGSA:
                            ret |= Gps_ReadMessage_GPGSA(messageInfo->currentElement, fieldBuff);
                            break;
                        case GPS_GPGSV:
                            ret |= Gps_ReadMessage_GPGSV(messageInfo->currentElement, fieldBuff);
                            break;
                        case GPS_GPGLL:
                            ret |= Gps_ReadMessage_GPGLL(messageInfo->currentElement, fieldBuff);
                            break;
                        default:
                            break;
                    }

                    gpsData.read = newRead;
                    messageInfo->currentElement++;
                }
                /* Buffer overflow. End the function. */
                else
                {
                    gpsData.read = (newRead < GPS_RING_BUFFER_SIZE) ? newRead : 0u;
                    messageInfo->currentElement = messageInfo->maxElements;
                }
            }
            /* If there is no data in new msg, go to next step. */
            else
            {
                gpsData.read = newRead;
                messageInfo->currentElement++;
            }

        }
        /* Cannot find character, do not process the data. End the function. */
        else
        {
            gpsData.read = newRead;
            messageInfo->currentElement = messageInfo->maxElements;
        }
    }

    return ret;
}

/* Function called to get data from GPRMC msg. Example:
   $GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68
    0.  225446       Time of fix 22:54:46 UTC
    1.  A            Navigation receiver warning A = OK, V = warning
    2.  4916.45      Latitude 49 deg. 16.45 min
    3.  N            Direction NS
    4.  12311.12     Longitude 123 deg. 11.12 min
    5.  W            Direction WE
    6.  000.5        Speed over ground, Knots
    7.  054.7        Course Made Good, True
    8.  191194       Date of fix  19 November 1994
    9.  020.3        Magnetic variation 20.3 deg 
   10.  E            Magnetic direction WE + checksum
*/
uint8_t Gps_ReadMessage_GPRMC(uint8_t currentElement, uint8_t* fieldBuff)
{
    uint32_t timeLinked = 0u;
    uint32_t dateLinked = 0u;

    switch (currentElement)
    {
        case GPS_GPRMC_TIME:
            memset(&fieldBuff[GPS_TIME_LEN], 0u, GPS_FIELD_BUFFER_SIZE - GPS_TIME_LEN);
            sscanf((char*)fieldBuff, "%ld", &timeLinked);
            gpsData.timeSec  = timeLinked % GPS_TIME_COEFF1;
            gpsData.timeMin = (timeLinked / GPS_TIME_COEFF1) % GPS_TIME_COEFF1;
            gpsData.timeHr = ((timeLinked / GPS_TIME_COEFF2) % GPS_TIME_COEFF1) + GPS_TIMEZONE_OFFSET;
            break;
        case GPS_GPRMC_WARNING:
            break;
        case GPS_GPRMC_LATITUDE:
            sscanf((char*)fieldBuff, "%f", &gpsData.latitude);
            break;
        case GPS_GPRMC_NS:
            sscanf((char*)fieldBuff, "%c", &gpsData.latDir);
            break;
        case GPS_GPRMC_LONGITUDE:
            sscanf((char*)fieldBuff, "%f", &gpsData.longitude);
            break;
        case GPS_GPRMC_WE:
            sscanf((char*)fieldBuff, "%c", &gpsData.lonDir);
            break;
        case GPS_GPRMC_SPEEDGROUNDKNOTS:
            break;
        case GPS_GPRMC_COURSEMADEGOOD:
            break;
        case GPS_GPRMC_DATE:
            sscanf((char*)fieldBuff, "%ld", &dateLinked);
            gpsData.dateYear = dateLinked % GPS_DATE_COEFF1;
            gpsData.dateMon = (dateLinked / GPS_DATE_COEFF1) % GPS_DATE_COEFF1;
            gpsData.dateDay = ((dateLinked / GPS_DATE_COEFF2) % GPS_DATE_COEFF1);
            break;
        case GPS_GPRMC_MAGNETICVAR:
            break;
        case GPS_GPRMC_MAGNETICDIR:
            break;
        default:
            break;
    }

    return RET_OK;
}


/* Function called to get data from GPRMC msg. Example:
   $GPVTG,360.0,T,348.7,M,000.0,N,000.0,K*43
    0.  360.0        Track made good (degrees true)
    1.  T            Fixed text 'T' indicates that track made good is relative to true north
    2.  348.7        Track made good (degrees magnetic)
    3.  M            M: track made good is relative to magnetic north
    4.  000.0        Speed over ground in knots
    5.  N            Fixed text 'N' indicates that speed over ground in in knots
    6.  000.0        Speed over ground in kilometers/hour
    7.  K*43         Fixed text 'K' indicates that speed over ground is in kilometers/hour + checksum
*/
uint8_t Gps_ReadMessage_GPVTG(uint8_t currentElement, uint8_t* fieldBuff)
{
    return RET_OK;  //length mismatch
}


/* Function called to get data from GPGGA msg. Example:
   $GPGGA,160915.00,5003.42292,N,01855.72499,E,1,08,1.01,240.5,M,40.7,M,,*5F 
    0.  160915.00       Time of fix 16:09:15 UTC
    1.  5003.42292      Latitude
    2.  N               Direction NS
    3.  01855.72499     Longitude
    4.  E               Direction WE
    5.  1               GPS quality indicator
    6.  08              Number of satelites
    7.  1.01            Horizontal dilution of horizon
    8.  240.5           Altitude
    9.  M               Units, meters
   10.  40.7            Geoidal separation
   11.  M               Units, meters 
   12.                  [--]
   13.  *5F             Checksum
*/
uint8_t Gps_ReadMessage_GPGGA(uint8_t currentElement, uint8_t* fieldBuff)
{
    uint32_t timeLinked = 0u;

    switch(currentElement)
    {
        case GPS_GPGGA_TIME:
            memset(&fieldBuff[GPS_TIME_LEN], 0u, GPS_FIELD_BUFFER_SIZE - GPS_TIME_LEN);
            sscanf((char*)fieldBuff, "%ld", &timeLinked);
            gpsData.timeSec  = timeLinked % GPS_TIME_COEFF1;
            gpsData.timeMin = (timeLinked / GPS_TIME_COEFF1) % GPS_TIME_COEFF1;
            gpsData.timeHr = ((timeLinked / GPS_TIME_COEFF2) % GPS_TIME_COEFF1) + GPS_TIMEZONE_OFFSET;
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

    return RET_OK;
}


/* Function called to get data from GPGSA msg. Example:
   $GPGSA,A,3,,,,,,16,18,,22,24,,,3.6,2.1,2.2*3C 
    0.  A               Mode: M=Manual, forced to operate in 2D or 3D, A=Automatic, 3D/2D
    1.  3               Mode: 1-Fix no available, 2=2D, 3=3D
 2-13.  ...             IDs of SVs used in position fix (null for unused fields)
   14.  3.6             PDOP
   15.  2.1             HDOP
   16.  2.2*3C          VDOP + checksum
*/
uint8_t Gps_ReadMessage_GPGSA(uint8_t currentElement, uint8_t* fieldBuff)
{
    return RET_OK;
}


/* Function called to get data from GPGSV msg. Example:
   $GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
    0.  3               Total number of messages of this type in this cycle
    1.  1               Message number
    2.  11              Total number of SVs in view
    3.  03              SV PRN number
    4.  03              Elevation in degrees, 90 maximum
    5.  111             Azimuth, degrees from true north, 000 to 359
    6.  00              SNR, 00-99 dB (null when not tracking)
 7-10.  ...             Information about second SV, same as field 3-6
11-14.  ...             Information about third SV, same as field 3-6
15-18.  ...             Information about fourth SV, same as field 3-6 + checksum
*/
uint8_t Gps_ReadMessage_GPGSV(uint8_t currentElement, uint8_t* fieldBuff)
{
    return RET_OK;  //length mismatch
}


/* Function called to get data from GPGSA msg. Example:
   $GPGLL,3751.65,S,14507.36,E,225444,A,A*6E
    0.  3751.65         Latitude
    1.  S               Direction NS
    2.  14507.36        Longitude
    3.  E               Direction WE
    4.  225444          Time of fix 22:54:44 UTC
    5.  A               Data status A-valid, V-invalid
    6.  A*6E            Positioning system mode indicator + checksum
*/
uint8_t Gps_ReadMessage_GPGLL(uint8_t currentElement, uint8_t* fieldBuff)
{
    uint32_t timeLinked = 0u;

    switch (currentElement)
    {
        case GPS_GPGLL_LATITUDE:
            sscanf((char*)fieldBuff, "%f", &gpsData.latitude);
            break;
        case GPS_GPGLL_NS:
            sscanf((char*)fieldBuff, "%c", &gpsData.latDir);
            break;
        case GPS_GPGLL_LONGITUDE:
            sscanf((char*)fieldBuff, "%f", &gpsData.longitude);
            break;
        case GPS_GPGLL_WE:
            sscanf((char*)fieldBuff, "%c", &gpsData.lonDir);
            break;
        case GPS_GPGLL_TIME:
            memset(&fieldBuff[GPS_TIME_LEN], 0u, GPS_FIELD_BUFFER_SIZE - GPS_TIME_LEN);
            sscanf((char*)fieldBuff, "%ld", &timeLinked);
            gpsData.timeSec  = timeLinked % GPS_TIME_COEFF1;
            gpsData.timeMin = (timeLinked / GPS_TIME_COEFF1) % GPS_TIME_COEFF1;
            gpsData.timeHr = ((timeLinked / GPS_TIME_COEFF2) % GPS_TIME_COEFF1) + GPS_TIMEZONE_OFFSET;
            break;
        case GPS_GPGLL_WARNING:
            break;
        case GPS_GPGLL_MODEINDICATOR:
            break;
        default:
            break;
    }
    return RET_OK;
}


/* Function called to prepare buffer which will be send 
   via USB.
*/
uint8_t Gps_PrepareDebugData(void)
{
    memset(gpsDebug.buffer, 0u, GPS_DEBUG_BUFF_SIZE);
    gpsDebug.size = snprintf((char*)gpsDebug.buffer, GPS_DEBUG_BUFF_SIZE, 
        "Read: %d,  Write: %d,  Time: %d:%d:%d,  Lat: NONE,  Dir: %c,  Lon: NONE,  Dir: %c,  Alti: NONE,  fixQual: %d\n",
        gpsData.read, gpsData.write, gpsData.timeHr, gpsData.timeMin, gpsData.timeSec,
        gpsData.latDir, gpsData.lonDir, gpsData.fixQuality);

    return RET_OK;
}
