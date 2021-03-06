/******************************************************************************
* @file           : dataCollector.c
* @brief          : This file contains interfaces to read and write from
                    other global or static variables. Here all getters and
                    setters are defined.
******************************************************************************/

#include "dataCollector.h"
#include "types.h"
#include "main.h"
#include "neoGps.h"
#include "fileSystem.h"
#include "envSensors.h"
#include "settings.h"

/* -------------------- main -------------------- */
/*  File: main.c
    Type: incrementer
    Variable: cnt.c_gpsTask
*/
void DC_inc_main_cnt_c_gpsTask(void)
{
    cnt.c_gpsTask += 1u;
}


/*  File: main.c
    Type: incrementer
    Variable: cnt.c_defaultTask
*/
void DC_inc_main_cnt_c_defaultTask(void)
{
    cnt.c_defaultTask += 1u;
}


/*  File: main.c
    Type: incrementer
    Variable: tim.t_100ms
*/
void DC_inc_main_tim_t_100ms(void)
{
    tim.t_100ms += 1u;
}


/*  File: main.c
    Type: incrementer
    Variable: tim.t_1s
*/
void DC_inc_main_tim_t_1s(void)
{
    tim.t_1s += 1u;
}


/*  File: main.c
    Type: getter
    Variable: tim.t_100ms
*/
uint32_t DC_get_main_tim_t_100ms(void)
{
    return tim.t_100ms;
}


/*  File: main.c
    Type: getter
    Variable: tim.t_1s
*/
uint32_t DC_get_main_tim_t_1s(void)
{
    return tim.t_1s;
}
/* -------------------- main -------------------- */


/* -------------------- neoGps -------------------- */
/*  File: neoGps.c
    Type: getter
    Variable: gpsData.latitude
*/
float DC_get_neoGps_gpsData_latitude(void)
{
    return gpsData.latitude;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.longitude
*/
float DC_get_neoGps_gpsData_longitude(void)
{
    return gpsData.longitude;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.altitude
*/
float DC_get_neoGps_gpsData_altitude(void)
{
    return gpsData.altitude;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.read
*/
uint16_t DC_get_neoGps_gpsData_read(void)
{
    return gpsData.read;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.write
*/
uint16_t DC_get_neoGps_gpsData_write(void)
{
    return gpsData.write;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.state
*/
uint8_t DC_get_neoGps_gpsData_state(void)
{
    return (uint8_t)gpsData.state;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.time
*/
uint32_t DC_get_neoGps_gpsData_time(void)
{
    return gpsData.time;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.timeHr
*/
uint8_t DC_get_neoGps_gpsData_timeHr(void)
{
    return gpsData.timeHr;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.timeMin
*/
uint8_t DC_get_neoGps_gpsData_timeMin(void)
{
    return gpsData.timeMin;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.timeSec
*/
uint8_t DC_get_neoGps_gpsData_timeSec(void)
{
    return gpsData.timeSec;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.date
*/
uint32_t DC_get_neoGps_gpsData_date(void)
{
    return gpsData.date;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.dateDay
*/
uint8_t DC_get_neoGps_gpsData_dateDay(void)
{
    return gpsData.dateDay;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.dateMon
*/
uint8_t DC_get_neoGps_gpsData_dateMon(void)
{
    return gpsData.dateMon;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.dateYear
*/
uint8_t DC_get_neoGps_gpsData_dateYear(void)
{
    return gpsData.dateYear;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.fixQuality
*/
uint8_t DC_get_neoGps_gpsData_fixQuality(void)
{
    return gpsData.fixQuality;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.satellitesNum
*/
uint8_t DC_get_neoGps_gpsData_satellitesNum(void)
{
    return gpsData.satellitesNum;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.lonDir
*/
char DC_get_neoGps_gpsData_lonDir(void)
{
    return gpsData.lonDir;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.latDir
*/
char DC_get_neoGps_gpsData_latDir(void)
{
    return gpsData.latDir;
}


/*  File: neoGps.c
    Type: setter
    Variable: gpsData.write
*/
void DC_set_neoGps_gpsData_write(uint16_t value)
{
    gpsData.write = value;
}
/* -------------------- neoGps -------------------- */


/* -------------------- fileSystem -------------------- */
/*  File: fileSystem.c
    Type: getter
    Variable: sdCardInfo.totalSpace
*/
uint32_t DC_get_fileSystem_sdCardInfo_totalSpace(void)
{
    return sdCardInfo.totalSpace;
}


/*  File: fileSystem.c
    Type: getter
    Variable: sdCardInfo.freeSpace
*/
uint32_t DC_get_fileSystem_sdCardInfo_freeSpace(void)
{
    return sdCardInfo.freeSpace;
}


/*  File: fileSystem.c
    Type: getter
    Variable: sdCardInfo.state
*/
uint8_t DC_get_fileSystem_sdCardInfo_state(void)
{
    return (uint8_t)sdCardInfo.state;
}


/*  File: fileSystem.c
    Type: getter
    Variable: sdCardInfo.blocked
*/
uint8_t DC_get_fileSystem_sdCardInfo_blocked(void)
{
    return (uint8_t)sdCardInfo.blocked;
}


/*  File: fileSystem.c
    Type: getter
    Variable: dirInfo.in.filesNum
*/
uint8_t DC_get_fileSystem_dirInfo_in_filesNum(void)
{
    return dirInfo.in.filesNum;
}


/*  File: fileSystem.c
    Type: getter
    Variable: dirInfo.out.filesNum
*/
uint8_t DC_get_fileSystem_dirInfo_out_filesNum(void)
{
    return dirInfo.out.filesNum;
}


/*  File: fileSystem.c
    Type: setter
    Variable: sdCardInfo.blocked
*/
void DC_set_fileSystem_sdCardInfo_blocked(uint8_t value)
{
    sdCardInfo.blocked = value;
}
/* -------------------- fileSystem -------------------- */



/* -------------------- envSensors -------------------- */
/*  File: envSensors.c
    Type: getter
    Variable: sensorData.altitude
*/
float DC_get_sensorData_altitude(void)
{
    return sensorData.altitude;
}


/*  File: envSensors.c
    Type: getter
    Variable: sensorData.pressure
*/
float DC_get_sensorData_pressure(void)
{
    return sensorData.pressure;
}


/*  File: envSensors.c
    Type: getter
    Variable: sensorData.temperature
*/
float DC_get_sensorData_temperature(void)
{
    return sensorData.temperature;
}


/*  File: envSensors.c
    Type: getter
    Variable: sensorActivationFlags.enabled
*/
uint8_t DC_get_sensorActivationFlags_enabled(void)
{
    return sensorActivationFlags.enabled;
}


/*  File: envSensors.c
    Type: setter
    Variable: sensorActivationFlags.enabled
*/
void DC_set_sensorActivationFlags_enabled(uint8_t value)
{
    sensorActivationFlags.enabled = value;
}


/*  File: envSensors.c
    Type: wrapper
    Function: Env_Sensors_CalibrateAltitude()
*/
void DC_call_CalibrateAltitude(int16_t newAlti)
{
    Env_Sensors_CalibrateAltitude(newAlti);
}
/* -------------------- envSensors -------------------- */



/* -------------------- settings -------------------- */
/*  File: AppSettingsScreenPresenter.c
    Type: getter
    Variable: settingsData.value[SETTINGS_ID_TIMEZONE]
*/
uint8_t DC_get_appSettings_settingsData_value_timezone(void)
{
    return settingsData.value[SETTINGS_ID_TIMEZONE];
}


/*  File: AppSettingsScreenPresenter.c
    Type: getter
    Variable: settingsData.value[SETTINGS_ID_MESH_SIZE]
*/
uint8_t DC_get_appSettings_settingsData_value_meshsize(void)
{
    return settingsData.value[SETTINGS_ID_MESH_SIZE];
}


/*  File: AppSettingsScreenPresenter.c
    Type: getter
    Variable: settingsData.value[SETTINGS_ID_SENSORS]
*/
uint8_t DC_get_appSettings_settingsData_value_sensors(void)
{
    return settingsData.value[SETTINGS_ID_SENSORS];
}
/* -------------------- settings -------------------- */
