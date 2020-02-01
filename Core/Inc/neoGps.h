/******************************************************************************
* @file   : neoGps.h
* @brief             : Header for neoGps.c file.
*                         This file contains the common defines of the neoGps.
******************************************************************************/

#ifndef NEOGPS_H
#define NEOGPS_H

# ifdef __cplusplus
extern "C" {
# endif


//include area
#include "stm32f4xx_hal.h"


//define area
#define GPS_RING_BUFFER_SIZE    1024u   //size of ring buffer
#define GPS_MAX_NMEA_SIZE       83u     //max size of gps nmea message
#define GPS_FIELD_BUFFER_SIZE   16u     //size of the temporary filed buffer
#define GPS_DEBUG_BUFF_SIZE     128u

#define GPS_NMEA_WORD_LEN       5u      //length of 'GPxxx' word
#define GPS_NMEA_MSG_NUM        6u      //number of NMEA messages

#define GPS_GPPGA_ELEMENTS      14u
#define GPS_GPPGA_TIME_LEN      6u
#define GPS_GPPGA_TIME_COEFF1   100u
#define GPS_GPPGA_TIME_COEFF2   10000u

//enums area
enum Gps_gpggaDataSequence
{
    GPS_GPGGA_TIME,
    GPS_GPGGA_LATITUDE,
    GPS_GPGGA_NS,
    GPS_GPGGA_LONGITUDE,
    GPS_GPGGA_WE,
    GPS_GPGGA_FIXQUALITY,
    GPS_GPGGA_SATELITESNUM,
    GPS_GPGGA_DILUTION,
    GPS_GPGGA_ALTITUDE
};

enum Gps_bufferState
{
    GPS_OK_AHEAD,
    GPS_OK_BEHIND,
    GPS_FULL = 255
};

enum Gps_msgType
{
    GPS_GPRMC,
    GPS_GPVTG,
    GPS_GPGGA,
    GPS_GPGSA,
    GPS_GPGSV,
    GPS_GPGLL,
    GPS_ERROR = 255u
};

enum Gps_nmeaOffset
{
    GPS_NMEA_OFFSET_ONE = 1u,
    GPS_NMEA_OFFSET_TWO,
    GPS_NMEA_OFFSET_THREE,
    GPS_NMEA_OFFSET_FOUR,
    GPS_NMEA_OFFSET_FIVE,
};

//typedef to store all gps data
typedef struct GpsUartData_Tag
{
    float latitude;
    float longitude;
    float dilution;
    float altitude;

    uint16_t read;
    volatile uint16_t write;
    volatile uint8_t ringBuff[GPS_RING_BUFFER_SIZE];
    volatile enum Gps_bufferState state;

    uint8_t dateDay;
    uint8_t dateMon;
    uint8_t dateYear;
    uint8_t timeHr;
    uint8_t timeMin;
    uint8_t timeSec;

    char latDir;
    char lonDir;

    uint8_t fixQuality;
    uint8_t satelitesNum;
    uint8_t dummy;
} GpsUartData_T;

//typedef to send gps debug data
typedef struct GpsDebugData_Tag
{
    uint8_t buffer[GPS_DEBUG_BUFF_SIZE];
    uint16_t size;
} GpsDebugData_T;

//variables declaration area
extern GpsUartData_T gpsData;
extern GpsDebugData_T gpsDebug;

//functions declaration area
void Gps_Main(void);
void Gps_PrepareWrite(void);
uint8_t Gps_SelectMsg(void);
void Gps_ReadGPGGA(void);
void Gps_PrepareDebugData(void);


# ifdef __cplusplus
}
# endif

#endif
