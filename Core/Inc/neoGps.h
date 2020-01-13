/******************************************************************************
* @file           : neoGps.h
* @brief          : Header for neoGps.c file.
*                   This file contains the common defines of the neoGps.
******************************************************************************/

#ifndef NEOGPS_H
#define NEOGPS_H

#ifdef __cplusplus
 extern "C" {
#endif


//include area
#include "stm32f4xx_hal.h"


//define area
#define UART_GPS_MSG_SIZE        128u     //max size of gps uart message
#define GPS_VAR_LEN              8u      //buff size for storing gps variables

# define GPS_SENTENCELENGTH      6u
# define GPS_GPGGA               (const char*)("$GPGGA")
# define GPS_GPRMC               (const char*)("$GPRMC")

# define GPS_GPPGA_DIV_VALUE     100.f
# define GPS_GPPGA_MUL_VALUE     (double)(100.f/60.f)

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


//variables declaration area
extern uint8_t gpsBuff[UART_GPS_MSG_SIZE];
extern char  lonGpsBuff[GPS_VAR_LEN];
extern char  latGpsBuff[GPS_VAR_LEN];
extern char  timeGpsBuff[GPS_VAR_LEN];


//functions declaration area
//void Gps_SplitFrame(uint8_t* gpsBuff, uint8_t* lonGpsBuff, uint8_t* latGpsBuff, uint8_t* timeGpsBuff);
void Gps_SplitFrame(void);


#ifdef __cplusplus
 }
#endif

#endif
