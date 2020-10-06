/******************************************************************************
* @file   : neoGps.h
* @brief  : Header for neoGps.c file.
*           This file contains the common defines of the neoGps.
******************************************************************************/

#ifndef NEOGPS_H
#define NEOGPS_H

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */


/* START OF THE INCLUDE AREA */
#include "stm32f4xx_hal.h"
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
#define GPS_RING_BUFFER_SIZE        512u   //size of ring buffer
#define GPS_MAX_NMEA_SIZE           83u     //max size of gps nmea message
#define GPS_FIELD_BUFFER_SIZE       16u     //size of the temporary filed buffer
#define GPS_DEBUG_BUFF_SIZE         128u

#define GPS_NMEA_WORD_LEN           5u      //length of 'GPxxx' word
#define GPS_NMEA_MSG_NUM            6u      //number of NMEA messages

#define GPS_TIME_LEN                6u
#define GPS_TIME_COEFF1             100u
#define GPS_TIME_COEFF2             10000u
#define GPS_DATE_LEN                6u
#define GPS_DATE_COEFF1             100u
#define GPS_DATE_COEFF2             10000u

#define GPS_SIZE_GPRMC              13u     //13    ok
#define GPS_SIZE_GPVTG              10u     //10    ok
#define GPS_SIZE_GPPGA              15u     //15    ok
#define GPS_SIZE_GPGSA              18u     //18    ok
#define GPS_SIZE_GPGSV              19u     //7     none
#define GPS_SIZE_GPGLL              8u      //8     ok

#define GPS_RETRIGGER_TIMEOUT       5u      //time [s] to retrigger gps uart

#define GPS_DEBUG                   0u      //disable debug messages

#define GPS_LIMIT_ALTITUDE_LOWER            0.f
#define GPS_LIMIT_ALTITUDE_UPPER            10000.f
#define GPS_LIMIT_LATITUDE_LOWER            0.f
#define GPS_LIMIT_LATITUDE_UPPER            9000.f
#define GPS_LIMIT_LONGITUDE_LOWER           0.f
#define GPS_LIMIT_LONGITUDE_UPPER           18000.f
#define GPS_LIMIT_DATE_YEAR_LOWER           0u
#define GPS_LIMIT_DATE_YEAR_UPPER           99u
#define GPS_LIMIT_DATE_MON_LOWER            1u
#define GPS_LIMIT_DATE_MON_UPPER            12u
#define GPS_LIMIT_DATE_DAY_LOWER            1u
#define GPS_LIMIT_DATE_DAY_UPPER            31u
#define GPS_LIMIT_TIME_SEC_LOWER            0u
#define GPS_LIMIT_TIME_SEC_UPPER            59u
#define GPS_LIMIT_TIME_MIN_LOWER            0u
#define GPS_LIMIT_TIME_MIN_UPPER            59u
#define GPS_LIMIT_TIME_HR_LOWER             0u
#define GPS_LIMIT_TIME_HR_UPPER             23u
#define GPS_LIMIT_FIXQUALITY_LOWER          0u
#define GPS_LIMIT_FIXQUALITY_UPPER          2u
#define GPS_LIMIT_HDOP_LOWER                0.f
#define GPS_LIMIT_HDOP_UPPER                100.f
#define GPS_LIMIT_VDOP_LOWER                0.f
#define GPS_LIMIT_VDOP_UPPER                100.f
#define GPS_LIMIT_MODETWO_LOWER             1u
#define GPS_LIMIT_MODETWO_UPPER             3u
#define GPS_LIMIT_SATELLITESNUM_LOWER       0u
#define GPS_LIMIT_SATELLITESNUM_UPPER       33u     //max number of satellites
#define GPS_LIMIT_GROUNDSPEEDKMH_LOWER      0.f
#define GPS_LIMIT_GROUNDSPEEDKMH_UPPER      300.f
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
typedef enum
{
    GPS_GPRMC_TIME,
    GPS_GPRMC_POSITIONSTATUS,
    GPS_GPRMC_LATITUDE,
    GPS_GPRMC_NS,
    GPS_GPRMC_LONGITUDE,
    GPS_GPRMC_WE,
    GPS_GPRMC_SPEEDGROUNDKNOTS,
    GPS_GPRMC_TRACKMADEGOODDEGREES,
    GPS_GPRMC_DATE,
    GPS_GPRMC_MAGNETICVARIATION,
    GPS_GPRMC_MAGNETICVARIATIONDIR,
    GPS_GPRMC_MODEINDICATOR,
    GPS_GPRMC_CHECKSUM
}Gps_gprmcDataSequence;

typedef enum
{
    GPS_GPVTG_TRACKMADEGOODDEGREES,
    GPS_GPVTG_RELATIVENORTH,
    GPS_GPVTG_TRACKMADEGOODDEGREESMAGNETIC,
    GPS_GPVTG_RELATIVEMAGNETICNORTH,
    GPS_GPVTG_GROUNDSPEEDKNOTS,
    GPS_GPVTG_SPEEDUNITKNOTS,
    GPS_GPVTG_GROUNDSPEEDKMH,
    GPS_GPVTG_SPEEDUNITKMH,
    GPS_GPVTG_MODEINDICATOR,
    GPS_GPVTG_CHECKSUM
}Gps_gpvtgDataSequence;

typedef enum
{
    GPS_GPGGA_TIME,
    GPS_GPGGA_LATITUDE,
    GPS_GPGGA_NS,
    GPS_GPGGA_LONGITUDE,
    GPS_GPGGA_WE,
    GPS_GPGGA_FIXQUALITY,
    GPS_GPGGA_SATELLITESNUM,
    GPS_GPGGA_HDOP,
    GPS_GPGGA_ALTITUDE,
    GPS_GPGGA_ALTITUDEUNITSMETERS,
    GPS_GPGGA_GEOIDALSEPARATION,
    GPS_GPGGA_GEOIDALSEPARATIONUNITMETERS,
    GPS_GPGGA_EMPTYSLOTONE,
    GPS_GPGGA_EMPTYSLOTTWO,
    GPS_GPGGA_CHECKSUM
}Gps_gpggaDataSequence;

typedef enum
{
    GPS_GPGSA_MODEONE,
    GPS_GPGSA_MODETWO,
    GPS_GPGSA_SATELLITEIDONE,
    GPS_GPGSA_SATELLITEIDTWO,
    GPS_GPGSA_SATELLITEIDTHREE,
    GPS_GPGSA_SATELLITEIDFOUR,
    GPS_GPGSA_SATELLITEIDFIVE,
    GPS_GPGSA_SATELLITEIDSIX,
    GPS_GPGSA_SATELLITEIDSEVEN,
    GPS_GPGSA_SATELLITEIDEIGTH,
    GPS_GPGSA_SATELLITEIDNINE,
    GPS_GPGSA_SATELLITEIDTEN,
    GPS_GPGSA_SATELLITEIDELEVEN,
    GPS_GPGSA_SATELLITEIDTWELVE,
    GPS_GPGSA_PDOP,
    GPS_GPGSA_HDOP,
    GPS_GPGSA_VDOP,
    GPS_GPGSA_CHECKSUM
}Gps_gpgsaDataSequence;

typedef enum
{
    GPS_GPGLL_LATITUDE,
    GPS_GPGLL_NS,
    GPS_GPGLL_LONGITUDE,
    GPS_GPGLL_WE,
    GPS_GPGLL_TIME,
    GPS_GPGLL_POSITIONSTATUS,
    GPS_GPGLL_MODEINDICATOR,
    GPS_GPGLL_CHECKSUM
}Gps_gpgllDataSequence;

typedef enum
{
    GPS_OK_AHEAD,
    GPS_OK_BEHIND,
    GPS_FULL = 255u
}Gps_bufferState;

typedef enum
{
    GPS_GPRMC,
    GPS_GPVTG,
    GPS_GPGGA,
    GPS_GPGSA,
    GPS_GPGSV,
    GPS_GPGLL,
    GPS_INVALID = 255u
}Gps_msgType;

typedef enum
{
    GPS_NMEA_OFFSET_ONE = 1u,
    GPS_NMEA_OFFSET_TWO,
    GPS_NMEA_OFFSET_THREE,
    GPS_NMEA_OFFSET_FOUR,
    GPS_NMEA_OFFSET_FIVE,
}Gps_nmeaOffset;

typedef enum
{
    GPS_MODEONE_INVALID,
    GPS_MODEONE_AUTOMATIC,
    GPS_MODEONE_MANUAL
}Gps_modeOne;

typedef enum
{
    GPS_MODETWO_INVALID,
    GPS_MODETWO_FIXUNAVAILABLE,     //have to be 1u
    GPS_MODETWO_2D,
    GPS_MODETWO_3D
}Gps_modeTwo;

typedef enum
{
    GPS_MODEINDICATOR_DATANOTVALID,
    GPS_MODEINDICATOR_AUTONOMOUS,
    GPS_MODEINDICATOR_DIFFERENTIAL,
    GPS_MODEINDICATOR_ESTIMATED,
    GPS_MODEINDICATOR_MANUALINPUT
}Gps_modeIndicator;
/* END OF THE ENUM AREA */


/* START OF THE TYPEDEF AREA */
//typedef to store all gps data
typedef struct GpsUartData_Tag
{
    volatile uint8_t ringBuff[GPS_RING_BUFFER_SIZE];
    uint16_t read;
    volatile uint16_t write;

    float latitude;
    float longitude;
    float hdop;
    float vdop;
    float altitude;
    float groundSpeedKmh;

    uint32_t time;
    uint32_t date;

    volatile Gps_bufferState state;

    uint8_t dateDay;
    uint8_t dateMon;
    uint8_t dateYear;
    uint8_t timeHr;
    uint8_t timeMin;
    uint8_t timeSec;

    char latDir;
    char lonDir;

    Gps_modeOne modeOne;
    Gps_modeTwo modeTwo;
    Gps_modeIndicator modeIndicator;

    uint8_t fixQuality;
    uint8_t satellitesNum;

} GpsUartData_T;

//typedef to send gps debug data
typedef struct GpsDebugData_Tag
{
    uint8_t buffer[GPS_DEBUG_BUFF_SIZE];
    uint16_t size;
} GpsDebugData_T;

//typedef to store information about actual read message
typedef struct GpsMsgInfo_Tag
{
    Gps_msgType type;
    uint8_t maxElements;
    uint8_t currentElement;
}GpsMsgInfo_T;

//typedef to store read location data before assignment to destination structure
typedef struct GpsLocData_Tag
{
    float latitude;
    float longitude;
}GpsLocData_T;
/* END OF THE TYPEDEF AREA */


/* START OF THE EXTERN VARIABLES AREA */
extern GpsUartData_T gpsData;
extern GpsDebugData_T gpsDebug;
/* END OF THE EXTERN VARIABLES AREA */


/* START OF THE FUNCTIONS PROTOTYPES AREA */
void Gps_Init(void);
uint8_t Gps_Main(void);
uint8_t Gps_PrepareWrite(void);
uint8_t Gps_SelectMsg(GpsMsgInfo_T* messageInfo);
uint8_t Gps_ReadMessage(GpsMsgInfo_T* messageInfo);
uint8_t Gps_ReadMessage_GPRMC(uint8_t currentElement, uint8_t* fieldBuff);
uint8_t Gps_ReadMessage_GPVTG(uint8_t currentElement, uint8_t* fieldBuff);
uint8_t Gps_ReadMessage_GPGGA(uint8_t currentElement, uint8_t* fieldBuff);
uint8_t Gps_ReadMessage_GPGSA(uint8_t currentElement, uint8_t* fieldBuff);
uint8_t Gps_ReadMessage_GPGSV(uint8_t currentElement, uint8_t* fieldBuff);
uint8_t Gps_ReadMessage_GPGLL(uint8_t currentElement, uint8_t* fieldBuff);
uint8_t Gps_PrepareDebugData(void);
uint8_t Gps_RetriggerUartGps(void);
uint8_t Gps_IncrementReadIndicator(void);
void Gps_ReadMessageElement_Altitude(uint8_t* fieldBuff);
void Gps_ReadMessageElement_Latitude(uint8_t* fieldBuff);
void Gps_ReadMessageElement_Longitude(uint8_t* fieldBuff);
void Gps_ReadMessageElement_Date(uint8_t* fieldBuff);
void Gps_ReadMessageElement_Time(uint8_t* fieldBuff);
void Gps_ReadMessageElement_FixQuality(uint8_t* fieldBuff);
void Gps_ReadMessageElement_Hdop(uint8_t* fieldBuff);
void Gps_ReadMessageElement_Vdop(uint8_t* fieldBuff);
void Gps_ReadMessageElement_LatDir(uint8_t* fieldBuff);
void Gps_ReadMessageElement_LonDir(uint8_t* fieldBuff);
void Gps_ReadMessageElement_ModeOne(uint8_t* fieldBuff);
void Gps_ReadMessageElement_ModeTwo(uint8_t* fieldBuff);
void Gps_ReadMessageElement_ModeIndicator(uint8_t* fieldBuff);
void Gps_ReadMessageElement_SatellitesNum(uint8_t* fieldBuff);
void Gps_ReadMessageElement_GroundSpeedKmh(uint8_t* fieldBuff);
void Gps_UpdatePositionData(void);
/* END OF THE FUNCTIONS PROTOTYPES AREA */

# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* NEOGPS_H */