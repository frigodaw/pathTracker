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
#define GPS_MSG_MAX_SIZE        85u   //max size of gps uart message
#define GPS_FIELD_BUFF_SIZE     16u   //size of field buffer 

#define GPS_VAR_LEN             8u    //buff size for storing gps variables

# define GPS_SENTENCELENGTH     6u
# define GPS_GPGGA              (const char*)("$GPGGA")
# define GPS_GPRMC                    (const char*)("$GPRMC")

# define GPS_GPPGA_DIV_VALUE    100.f
# define GPS_GPPGA_MUL_VALUE    (double)(100.f/60.f)

//enums area
enum Gps_gpggaDataSequence
{
    GPS_GPGGA_ID,
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

//typedef to store all gps data
typedef struct GpsUartData_Tag
{
    uint8_t read;
    uint8_t write;
    uint8_t fieldBuff[GPS_FIELD_BUFF_SIZE];
    uint8_t fieldPos;

    uint8_t dataBuff[GPS_MSG_MAX_SIZE];
    uint8_t dateDay;
    uint8_t dateMon;
    uint8_t dateYear;
    uint8_t timeHr;
    uint8_t timeMin;
    uint8_t timeSec;

    float latitude;
    char latDir;
    float longitude;
    char lonDir;
    float altitude;

    uint8_t fixQuality;
    uint8_t satelitesNum;
} GpsUartData_T;

//variables declaration area
extern uint8_t gpsBuff[GPS_MSG_MAX_SIZE];
extern char  lonGpsBuff[GPS_VAR_LEN];
extern char  latGpsBuff[GPS_VAR_LEN];
extern char  timeGpsBuff[GPS_VAR_LEN];
extern GpsUartData_T gpsData;
extern volatile uint8_t recvChar;


//functions declaration area
void Gps_SplitFrame(void);
void Gps_Main(void);
void Gps_ProcessLine(void);
void Gps_ReadField(void);
void Gps_ProcessReadout_GPRMC(void);
void Gps_ProcessReadout_GPVTG(void);
void Gps_ProcessReadout_GPGGA(void);
void Gps_ProcessReadout_GPGSA(void);


# ifdef __cplusplus
}
# endif

#endif
