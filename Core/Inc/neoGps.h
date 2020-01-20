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
#define GPS_RING_BUFFER_SIZE    512u    //size of ring buffer
#define GPS_MAX_NMEA_SIZE       83u     //max size of gps nmea message
#define GPS_FIELD_BUFFER_SIZE   16u     //size of the temporary filed buffer

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
    GPS_OK,
    GPS_FULL
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


//typedef to store all gps data
typedef struct GpsUartData_Tag
{
    double latitude;
    double longitude;
    float altitude;

    uint16_t read;
    uint16_t write;
    uint8_t ringBuff[GPS_RING_BUFFER_SIZE];
    uint8_t state;

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
} GpsUartData_T;

//variables declaration area
extern GpsUartData_T gpsData;

//functions declaration area
void Gps_Main(void);
void Gps_PrepareWrite(void);
uint8_t Gps_SelectMsg(void);
void Gps_ReadGPGGA(void);


# ifdef __cplusplus
}
# endif

#endif
