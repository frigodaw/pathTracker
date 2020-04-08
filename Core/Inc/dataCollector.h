/******************************************************************************
* @file           : dataCollector.h
* @brief          : Header file for dataCollector.c
                    This file contains interfaces to read and write from
                    other global or static variables. Here all getters and
                    setters are defined.
******************************************************************************/

#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */


/* START OF THE INCLUDE AREA */
#include "stm32f4xx_hal.h"
#include "neoGps.h"
#include "fileSystem.h"
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
/* END OF THE ENUM AREA */


/* START OF THE TYPEDEF AREA */
/* END OF THE TYPEDEF AREA */


/* START OF THE EXTERN VARIABLES AREA */
/* END OF THE EXTERN VARIABLES AREA */


/* START OF THE FUNCTIONS PROTOTYPES AREA */
/* ---------- main ---------- */
uint16_t DC_get_main_tim_t_1s(void);

/* ---------- neoGps ---------- */
float    DC_get_neoGps_gpsData_longitude(void);
float    DC_get_neoGps_gpsData_latitude(void);
float    DC_get_neoGps_gpsData_altitude(void);
uint16_t DC_get_neoGps_gpsData_read(void);
uint16_t DC_get_neoGps_gpsData_write(void);
uint8_t  DC_get_neoGps_gpsData_state(void);
uint32_t DC_get_neoGps_gpsData_time(void);
uint8_t  DC_get_neoGps_gpsData_timeHr(void);
uint8_t  DC_get_neoGps_gpsData_timeMin(void);
uint8_t  DC_get_neoGps_gpsData_timeSec(void);
uint32_t DC_get_neoGps_gpsData_date(void);
uint8_t  DC_get_neoGps_gpsData_dateDay(void);
uint8_t  DC_get_neoGps_gpsData_dateMon(void);
uint8_t  DC_get_neoGps_gpsData_dateYear(void);
uint8_t  DC_get_neoGps_gpsData_fixQuality(void);
uint8_t  DC_get_neoGps_gpsData_satellitesNum(void);
char     DC_get_neoGps_gpsData_lonDir(void);
char     DC_get_neoGps_gpsData_latDir(void);
void     DC_set_neoGps_gpsData_write(uint16_t value);

/* ---------- fileSystem ---------- */
uint32_t DC_get_fileSystem_sdCardInfo_totalSpace(void);
uint32_t DC_get_fileSystem_sdCardInfo_freeSpace(void);
uint8_t  DC_get_fileSystem_sdCardInfo_state(void);
uint8_t  DC_get_fileSystem_dirInfo_in_filesNum(void);
uint8_t  DC_get_fileSystem_dirInfo_out_filesNum(void);
/* END OF THE FUNCTIONS PROTOTYPES AREA */

# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* DATACOLLECTOR_H */