/******************************************************************************
* @file           : neoGps.c
* @brief          : Source for neoGps.c file.
*                   This file contains the source code of gps device.
******************************************************************************/

#include "neoGps.h"
#include "types.h"
#include "main.h"
#include "dataCollector.h"
#include <stdio.h>
#include <string.h>

/*  Global variables to store all GPS-related data */
GpsUartData_T gpsData = {0u};

/*  Static variable to store new coordinates before assignment to gpsData */
static GpsLocData_T gpsLocData = {0.f};

/*  Function called to initialize GPS variables
   during uC startup phase. */
void Gps_Init(void)
{
    memset(&gpsData, 0u, sizeof(gpsData));
    gpsData.state = GPS_FULL;
    gpsData.modeOne = GPS_MODEONE_INVALID;
    gpsData.modeTwo = GPS_MODETWO_INVALID;
    gpsData.modeIndicator = GPS_MODEINDICATOR_DATANOTVALID;
}


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
    GpsMsgInfo_T messageInfo = {0u};

    /* Read data until read indicator is not equal to write */
    while(gpsData.read != gpsData.write)
    {
        /* Refresh state */
        Gps_PrepareWrite();

        while((gpsData.ringBuff[gpsData.read] != '$') && (gpsData.read != gpsData.write))
        {
            Gps_IncrementReadIndicator();
        }

        /* Only when character was found */
        if(((gpsData.read + GPS_NMEA_OFFSET_FIVE) < GPS_RING_BUFFER_SIZE) &&
            (gpsData.ringBuff[gpsData.read] == '$') && 
            (gpsData.ringBuff[gpsData.read + GPS_NMEA_OFFSET_ONE] == 'G') &&
            (gpsData.ringBuff[gpsData.read + GPS_NMEA_OFFSET_TWO] == 'P'))
        {
            Gps_SelectMsg(&messageInfo);

            /* Go ahead if searching messages fails */
            if(messageInfo.type == GPS_INVALID)
            {
                Gps_IncrementReadIndicator();
            }
            /* Read proper message */
            else
            {
                Gps_ReadMessage(&messageInfo);
                Gps_UpdatePositionData();
            }
        }
        else if(gpsData.read == gpsData.write)
        {
            /* End of the while loop */
            break;
        }
        else
        {
            /* Can not proceed data. Increment read indicator. */
            Gps_IncrementReadIndicator();
        }
        
    }

    ret |= Gps_RetriggerUartGps();

#if GPS_DEBUG == 1
    ret |= Gps_PrepareDebugData();
#endif

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

    messageInfo->type = GPS_INVALID;
    messageInfo->maxElements = 0u;

    for(Gps_msgType msgId=0u; msgId < GPS_NMEA_MSG_NUM; msgId++)
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
             (gpsData.ringBuff[newRead] != ',') && (gpsData.ringBuff[newRead] != '*'));

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
        /* Found ',' or '*' character. */
        else if((gpsData.ringBuff[newRead] == ',') || (gpsData.ringBuff[newRead] == '*'))
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
    1.  A            Position status: A-valid, V-invalid
    2.  4916.45      Latitude 49 deg. 16.45 min
    3.  N            Direction NS
    4.  12311.12     Longitude 123 deg. 11.12 min
    5.  W            Direction WE
    6.  000.5        Speed over ground, Knots
    7.  054.7        Course Made Good, True
    8.  191194       Date of fix: 19 November 1994
    9.  020.3        Magnetic variation 20.3 deg 
   10.  E            Magnetic direction WE 
   11.  A            Mode indicator: A-autonomous, D-differential, E-estimated, M-manual input, N-data not valid
   12.  *68          Checksum
*/
uint8_t Gps_ReadMessage_GPRMC(uint8_t currentElement, uint8_t* fieldBuff)
{
    switch (currentElement)
    {
        case GPS_GPRMC_TIME:
            Gps_ReadMessageElement_Time(fieldBuff);
            break;
        case GPS_GPRMC_POSITIONSTATUS:
            break;
        case GPS_GPRMC_LATITUDE:
            Gps_ReadMessageElement_Latitude(fieldBuff);
            break;
        case GPS_GPRMC_NS:
            Gps_ReadMessageElement_LatDir(fieldBuff);
            break;
        case GPS_GPRMC_LONGITUDE:
            Gps_ReadMessageElement_Longitude(fieldBuff);
            break;
        case GPS_GPRMC_WE:
            Gps_ReadMessageElement_LonDir(fieldBuff);
            break;
        case GPS_GPRMC_SPEEDGROUNDKNOTS:
            break;
        case GPS_GPRMC_TRACKMADEGOODDEGREES:
            break;
        case GPS_GPRMC_DATE:
            Gps_ReadMessageElement_Date(fieldBuff);
            break;
        case GPS_GPRMC_MAGNETICVARIATION:
            break;
        case GPS_GPRMC_MAGNETICVARIATIONDIR:
            break;
        case GPS_GPRMC_MODEINDICATOR:
            Gps_ReadMessageElement_ModeIndicator(fieldBuff);
            break;
        case GPS_GPRMC_CHECKSUM:
            break;
        default:
            break;
    }

    return RET_OK;
}


/* Function called to get data from GPVTG msg. Example:
   $GPVTG,360.0,T,348.7,M,000.0,N,000.0,K,A*43
    0.  360.0        Track made good (degrees true)
    1.  T            Fixed text 'T' indicates that track made good is relative to true north
    2.  348.7        Track made good (degrees magnetic)
    3.  M            M: track made good is relative to magnetic north
    4.  000.0        Speed over ground in knots
    5.  N            Fixed text 'N' indicates that speed over ground in in knots
    6.  000.0        Speed over ground in kilometers/hour
    7.  K            Fixed text 'K' indicates that speed over ground is in kilometers/hour
    8.  A            Mode indicator: A-autonomous, D-differential, E-estimated, M-manual input, N-data not valid
    9.  *43          Checksum
*/
uint8_t Gps_ReadMessage_GPVTG(uint8_t currentElement, uint8_t* fieldBuff)
{
    switch (currentElement)
    {
        case GPS_GPVTG_TRACKMADEGOODDEGREES:
            break;
        case GPS_GPVTG_RELATIVENORTH:
            break;
        case GPS_GPVTG_TRACKMADEGOODDEGREESMAGNETIC:
            break;
        case GPS_GPVTG_RELATIVEMAGNETICNORTH:
            break;
        case GPS_GPVTG_GROUNDSPEEDKNOTS:
            break;
        case GPS_GPVTG_SPEEDUNITKNOTS:
            break;
        case GPS_GPVTG_GROUNDSPEEDKMH:
            Gps_ReadMessageElement_GroundSpeedKmh(fieldBuff);
            break;
        case GPS_GPVTG_SPEEDUNITKMH:
            break;
        case GPS_GPVTG_MODEINDICATOR:
            Gps_ReadMessageElement_ModeIndicator(fieldBuff);
            break;
        default:
            break;
    }

    return RET_OK;
}


/* Function called to get data from GPGGA msg. Example:
   $GPGGA,160915.00,5003.42292,N,01855.72499,E,1,08,1.01,240.5,M,40.7,M,,*5F 
    0.  160915.00       Time of fix 16:09:15 UTC
    1.  5003.42292      Latitude
    2.  N               Direction NS
    3.  01855.72499     Longitude
    4.  E               Direction WE
    5.  1               GPS quality indicator
    6.  08              Number of satellites
    7.  1.01            Horizontal dilution of precision
    8.  240.5           Altitude
    9.  M               Units, meters
   10.  40.7            Geoidal separation
   11.  M               Units, meters 
   12.                  [--]
   13.                  [--]
   14.  *5F             Checksum
*/
uint8_t Gps_ReadMessage_GPGGA(uint8_t currentElement, uint8_t* fieldBuff)
{
    switch(currentElement)
    {
        case GPS_GPGGA_TIME:
            Gps_ReadMessageElement_Time(fieldBuff);
            break;
        case GPS_GPGGA_LATITUDE:
            Gps_ReadMessageElement_Latitude(fieldBuff);
            break;
        case GPS_GPGGA_NS:
            Gps_ReadMessageElement_LatDir(fieldBuff);
            break;
        case GPS_GPGGA_LONGITUDE:
            Gps_ReadMessageElement_Longitude(fieldBuff);
            break;
        case GPS_GPGGA_WE:
            Gps_ReadMessageElement_LonDir(fieldBuff);
            break;
        case GPS_GPGGA_FIXQUALITY:
            Gps_ReadMessageElement_FixQuality(fieldBuff);
            break;
        case GPS_GPGGA_SATELLITESNUM:
            Gps_ReadMessageElement_SatellitesNum(fieldBuff);
            break;
        case GPS_GPGGA_HDOP:
            Gps_ReadMessageElement_Hdop(fieldBuff);
            break;
        case GPS_GPGGA_ALTITUDE:
            Gps_ReadMessageElement_Altitude(fieldBuff);
            break;
        case GPS_GPGGA_ALTITUDEUNITSMETERS:
            break;
        case GPS_GPGGA_GEOIDALSEPARATION:
            break;
        case GPS_GPGGA_GEOIDALSEPARATIONUNITMETERS:
            break;
        case GPS_GPGGA_EMPTYSLOTONE:
            break;
        case GPS_GPGGA_EMPTYSLOTTWO:
            break;
        case GPS_GPGGA_CHECKSUM:
            break;
        default:
            break;
    }

    return RET_OK;
}


/* Function called to get data from GPGSA msg. Example:
   $GPGSA,A,3,26,31,05,16,29,,,,,,,,3.6,2.1,2.2*3C 
    0.  A               Mode: M=Manual, forced to operate in 2D or 3D, A=Automatic, 3D/2D
    1.  3               Mode: 1-Fix no available, 2=2D, 3=3D
 2-13.  ...             IDs of SVs used in position fix (null for unused fields)
   14.  3.6             PDOP: position accuracy factor
   15.  2.1             HDOP: position accuracy factor horizontal
   16.  2.2             VDOP: position accuracy factor vertical
   17.  *3C             Checksum
*/
uint8_t Gps_ReadMessage_GPGSA(uint8_t currentElement, uint8_t* fieldBuff)
{
    switch(currentElement)
    {
        case GPS_GPGSA_MODEONE:
            Gps_ReadMessageElement_ModeOne(fieldBuff);
            break;
        case GPS_GPGSA_MODETWO:
            Gps_ReadMessageElement_ModeTwo(fieldBuff);
            break;
        case GPS_GPGSA_SATELLITEIDONE:
            break;
        case GPS_GPGSA_SATELLITEIDTWO:
            break;
        case GPS_GPGSA_SATELLITEIDTHREE:
            break;
        case GPS_GPGSA_SATELLITEIDFOUR:
            break;
        case GPS_GPGSA_SATELLITEIDFIVE:
            break;
        case GPS_GPGSA_SATELLITEIDSIX:
            break;
        case GPS_GPGSA_SATELLITEIDSEVEN:
            break;
        case GPS_GPGSA_SATELLITEIDEIGTH:
            break;
        case GPS_GPGSA_SATELLITEIDNINE:
            break;
        case GPS_GPGSA_SATELLITEIDTEN:
            break;
        case GPS_GPGSA_SATELLITEIDELEVEN:
            break;
        case GPS_GPGSA_SATELLITEIDTWELVE:
            break;
        case GPS_GPGSA_PDOP:
            break;
        case GPS_GPGSA_HDOP:
            Gps_ReadMessageElement_Hdop(fieldBuff);
            break;
        case GPS_GPGSA_VDOP:
            Gps_ReadMessageElement_Vdop(fieldBuff);
            break;
        case GPS_GPGSA_CHECKSUM:
            break;
        default:
            break;
    }

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


/* Function called to get data from GPGLL msg. Example:
   $GPGLL,3751.65,S,14507.36,E,225444,A,A*6E
    0.  3751.65         Latitude
    1.  S               Direction NS
    2.  14507.36        Longitude
    3.  E               Direction WE
    4.  225444          Time of fix 22:54:44 UTC
    5.  A               Position status: A-valid, V-invalid
    6.  A               Mode indicator: A-autonomous, D-differential, E-estimated, M-manual input, N-data not valid
    7.  *6E             Checksum
*/
uint8_t Gps_ReadMessage_GPGLL(uint8_t currentElement, uint8_t* fieldBuff)
{
    switch (currentElement)
    {
        case GPS_GPGLL_LATITUDE:
            Gps_ReadMessageElement_Latitude(fieldBuff);
            break;
        case GPS_GPGLL_NS:
            Gps_ReadMessageElement_LatDir(fieldBuff);
            break;
        case GPS_GPGLL_LONGITUDE:
            Gps_ReadMessageElement_Longitude(fieldBuff);
            break;
        case GPS_GPGLL_WE:
            Gps_ReadMessageElement_LonDir(fieldBuff);
            break;
        case GPS_GPGLL_TIME:
            Gps_ReadMessageElement_Time(fieldBuff);
            break;
        case GPS_GPGLL_POSITIONSTATUS:
            break;
        case GPS_GPGLL_MODEINDICATOR:
            Gps_ReadMessageElement_ModeIndicator(fieldBuff);
            break;
        case GPS_GPGLL_CHECKSUM:
            break;
        default:
            break;
    }
    return RET_OK;
}


#if GPS_DEBUG == 1
GpsDebugData_T gpsDebug = {0u};

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
#endif


/* Functon called to retrigger gps uart
   when it is stacked after overflow or lack of data.
   UART will be retrigger only when a pause was longer
   than GPS_RETRIGGER_TIMEOUT (5s).
*/
uint8_t Gps_RetriggerUartGps(void)
{
    static uint32_t timeFirstFault = 0u;
    static uint16_t lastWrite = 0u;

    uint32_t timeNow = DC_get_main_tim_t_1s();
    uint32_t timeDiff = 0u;

    if((gpsData.state == GPS_FULL) || (lastWrite == gpsData.write))
    {
        /* Wait until timeout elapses */
        if(timeFirstFault == 0u)
        {
            timeFirstFault = timeNow;
        }
        else
        {
            timeDiff = timeNow - timeFirstFault;

            if(timeDiff >= GPS_RETRIGGER_TIMEOUT)
            {
                /* Do the retrigger and refresh the state. */
                Gps_PrepareWrite();
                HAL_UART_Receive_DMA(&huart5, (uint8_t*)&gpsData.ringBuff[gpsData.write], GPS_MAX_NMEA_SIZE);

                timeFirstFault = 0u;
            }
            else
            {
                /* Timer has not elapsed yet. Wait. */
            }
        }
    }
    else
    {
        timeFirstFault = 0u;
    }

    lastWrite = gpsData.write;

    return RET_OK;
}


/* Function called to read altitude from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_Altitude(uint8_t* fieldBuff)
{
    float altitude = 0.f;
    sscanf((char*)fieldBuff, "%f", &altitude);

    if( (GPS_LIMIT_ALTITUDE_LOWER <= altitude) && (GPS_LIMIT_ALTITUDE_UPPER >= altitude) )
    {
        gpsData.altitude = altitude;
    }
}


/* Function called to read latitude from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_Latitude(uint8_t* fieldBuff)
{
    float latitude = 0.f;
    sscanf((char*)fieldBuff, "%f", &latitude);

    if( (GPS_LIMIT_LATITUDE_LOWER <= latitude) && (GPS_LIMIT_LATITUDE_UPPER >= latitude) )
    {
        //gpsData.latitude = latitude;
        gpsLocData.latitude = latitude;
    }
}


/* Function called to read longitude from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_Longitude(uint8_t* fieldBuff)
{
    float longitude = 0.f;
    sscanf((char*)fieldBuff, "%f", &longitude);

    if( (GPS_LIMIT_LONGITUDE_LOWER <= longitude) && (GPS_LIMIT_LONGITUDE_UPPER >= longitude) )
    {
        //gpsData.longitude = longitude;
        gpsLocData.longitude = longitude;
    }
}


/* Function called to read date from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_Date(uint8_t* fieldBuff)
{
    uint32_t dateLinked = 0u;
    uint8_t dateYear = 0u;
    uint8_t dateMon  = 0u;
    uint8_t dateDay  = 0u;

    memset(&fieldBuff[GPS_DATE_LEN], 0u, GPS_FIELD_BUFFER_SIZE - GPS_DATE_LEN);
    sscanf((char*)fieldBuff, "%ld", &dateLinked);

    dateYear = dateLinked % GPS_DATE_COEFF1;
    dateMon  = (dateLinked / GPS_DATE_COEFF1) % GPS_DATE_COEFF1;
    dateDay  = ((dateLinked / GPS_DATE_COEFF2) % GPS_DATE_COEFF1);

    if( ((GPS_LIMIT_DATE_YEAR_LOWER <= dateYear) && (GPS_LIMIT_DATE_YEAR_UPPER >= dateYear)) &&
        ((GPS_LIMIT_DATE_MON_LOWER  <= dateMon ) && (GPS_LIMIT_DATE_MON_UPPER  >= dateMon)) &&
        ((GPS_LIMIT_DATE_DAY_LOWER  <= dateDay ) && (GPS_LIMIT_DATE_DAY_UPPER  >= dateDay)) )
    {
        gpsData.date     = dateLinked;
        gpsData.dateYear = dateYear;
        gpsData.dateMon  = dateMon;
        gpsData.dateDay  = dateDay;
    }
}


/* Function called to read time from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_Time(uint8_t* fieldBuff)
{
    uint32_t timeLinked = 0u;
    uint8_t timeSec = 0u;
    uint8_t timeMin = 0u;
    uint8_t timeHr  = 0u;

    memset(&fieldBuff[GPS_TIME_LEN], 0u, GPS_FIELD_BUFFER_SIZE - GPS_TIME_LEN);
    sscanf((char*)fieldBuff, "%ld", &timeLinked);

    timeSec  = timeLinked % GPS_TIME_COEFF1;
    timeMin = (timeLinked / GPS_TIME_COEFF1) % GPS_TIME_COEFF1;
    timeHr = ((timeLinked / GPS_TIME_COEFF2) % GPS_TIME_COEFF1);

    if( ((GPS_LIMIT_TIME_SEC_LOWER <= timeSec) && (GPS_LIMIT_TIME_SEC_UPPER >= timeSec)) &&
        ((GPS_LIMIT_TIME_MIN_LOWER <= timeMin) && (GPS_LIMIT_TIME_MIN_UPPER >= timeMin)) &&
        ((GPS_LIMIT_TIME_HR_LOWER  <= timeHr ) && (GPS_LIMIT_TIME_HR_UPPER  >= timeHr)) )
    {
        gpsData.time    = timeLinked;
        gpsData.timeSec = timeSec;
        gpsData.timeMin = timeMin;
        gpsData.timeHr  = timeHr;
    }
}


/* Function called to read fixQuality from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_FixQuality(uint8_t* fieldBuff)
{
    uint16_t fixQuality = 0u;
    sscanf((char*)fieldBuff, "%hu", &fixQuality);

    if( (GPS_LIMIT_FIXQUALITY_LOWER <= fixQuality) && (GPS_LIMIT_FIXQUALITY_UPPER >= fixQuality) )
    {
        gpsData.fixQuality = (uint8_t)fixQuality;
    }
}


/* Function called to read hdop from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_Hdop(uint8_t* fieldBuff)
{
    float hdop = 0.f;
    sscanf((char*)fieldBuff, "%f", &hdop);

    if( (GPS_LIMIT_HDOP_LOWER <= hdop) && (GPS_LIMIT_HDOP_UPPER >= hdop) )
    {
        gpsData.hdop = hdop;
    }
}


/* Function called to read vdop from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_Vdop(uint8_t* fieldBuff)
{
    float vdop = 0.f;
    sscanf((char*)fieldBuff, "%f", &vdop);

    if( (GPS_LIMIT_VDOP_LOWER <= vdop) && (GPS_LIMIT_VDOP_UPPER >= vdop) )
    {
        gpsData.vdop = vdop;
    }
}


/* Function called to read latDir from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_LatDir(uint8_t* fieldBuff)
{
    char latDir = '0';
    sscanf((char*)fieldBuff, "%c", &latDir);

    if( ('N' == latDir ) || ('S' == latDir ) )
    {
        gpsData.latDir = latDir;
    }
}


/* Function called to read lonDir from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_LonDir(uint8_t* fieldBuff)
{
    char lonDir = '0';
    sscanf((char*)fieldBuff, "%c", &lonDir);

    if( ('W' == lonDir ) || ('E' == lonDir ) )
    {
        gpsData.lonDir = lonDir;
    }
}


/* Function called to read modeOne from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_ModeOne(uint8_t* fieldBuff)
{
    char modeOne = '0';
    sscanf((char*)fieldBuff, "%c", &modeOne);

    switch(modeOne)
    {
        case 'A':
            gpsData.modeOne = GPS_MODEONE_AUTOMATIC;
            break;
        case 'M':
            gpsData.modeOne = GPS_MODEONE_MANUAL;
            break;
        default:
            gpsData.modeOne = GPS_MODEONE_INVALID;
            break;
    }
}


/* Function called to read modeTwo from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_ModeTwo(uint8_t* fieldBuff)
{
    uint16_t modeTwo = 0u;
    sscanf((char*)fieldBuff, "%hu", &modeTwo);


    if( (GPS_LIMIT_MODETWO_LOWER <= modeTwo) && (GPS_LIMIT_MODETWO_UPPER >= modeTwo) )
    {
        gpsData.modeTwo = (uint8_t)modeTwo;
    }
}


/* Function called to read modeIndicator given in different
   gps frames.
*/
void Gps_ReadMessageElement_ModeIndicator(uint8_t* fieldBuff)
{
    char readModeIndicator = '0';
    sscanf((char*)fieldBuff, "%c", &readModeIndicator);

    switch(readModeIndicator)
    {
        case 'A':
            gpsData.modeIndicator = GPS_MODEINDICATOR_AUTONOMOUS;
            break;
        case 'D':
            gpsData.modeIndicator = GPS_MODEINDICATOR_DIFFERENTIAL;
            break;
        case 'E':
            gpsData.modeIndicator = GPS_MODEINDICATOR_ESTIMATED;
            break;
        case 'M':
            gpsData.modeIndicator = GPS_MODEINDICATOR_MANUALINPUT;
            break;
        case 'N':
        default:
            gpsData.modeIndicator = GPS_MODEINDICATOR_DATANOTVALID;
            break;
    }
}


/* Function called to read satellitesNum from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_SatellitesNum(uint8_t* fieldBuff)
{
    uint16_t satellitesNum = 0u;
    sscanf((char*)fieldBuff, "%hu", &satellitesNum);

    if( (GPS_LIMIT_SATELLITESNUM_LOWER <= satellitesNum) && (GPS_LIMIT_SATELLITESNUM_UPPER >= satellitesNum) )
    {
        gpsData.satellitesNum = (uint8_t)satellitesNum;
    }
}


/* Function called to read groundSpeedKmh from
   field buffer and if it is correct save it
   to gpsData structure */
void Gps_ReadMessageElement_GroundSpeedKmh(uint8_t* fieldBuff)
{
    float groundSpeedKmh = 0u;
    sscanf((char*)fieldBuff, "%f", &groundSpeedKmh);

    if( (GPS_LIMIT_GROUNDSPEEDKMH_LOWER <= groundSpeedKmh) && (GPS_LIMIT_GROUNDSPEEDKMH_UPPER >= groundSpeedKmh) )
    {
        gpsData.groundSpeedKmh = (uint8_t)groundSpeedKmh;
    }
}


/* Function called to increment read indicator
   or set it to zero when it is bigger than
   max size.
*/
uint8_t Gps_IncrementReadIndicator(void)
{
    if((gpsData.read + 1u) < GPS_RING_BUFFER_SIZE)
    {
        gpsData.read += 1u;
    }
    else
    {
        gpsData.read = 0u;
        gpsData.state = GPS_OK_AHEAD;
    }

    return RET_OK;
}


/* Function called set gpsData coordinates to new
   values stored in gpsLocData structure. This method makes
   assignment more atomic and it can be safely read from
   other tasks.
*/
void Gps_UpdatePositionData(void)
{
    gpsData.latitude = gpsLocData.latitude;
    gpsData.longitude = gpsLocData.longitude;
}