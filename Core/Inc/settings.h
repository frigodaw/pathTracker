/******************************************************************************
* @file           : settings.h
* @brief          : Header file for settings.c
                    This file contains settings structure with all
                    irremovable data needs by application.
******************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */


/* START OF THE INCLUDE AREA */
#include "stm32f4xx_hal.h"
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
#define SETTINGS_NAME_LEN                               16u
#define SETTINGS_NUMBER_OF_SETTINGS                     ((uint8_t)(SETTINGS_ID_AMOUT_OF_ELEMENTS))
#define SETTINGS_NO_DEFAULT                             255u

#define SETTINGS_NAME_TIMEZONE                          "Time zone"
#define SETTINGS_MIN_TIMEZONE                           -12
#define SETTINGS_MAX_TIMEZONE                           12
#define SETTINGS_DEFAULT_TIMEZONE                       2

#define SETTINGS_NAME_MESHSIZE                          "Mesh size"
#define SETTINGS_MIN_MESHSIZE                           1
#define SETTINGS_MAX_MESHSIZE                           10
#define SETTINGS_DEFAULT_MESHSIZE                       3

#define SETTINGS_NAME_SENSORS                           "Sensors"
#define SETTINGS_MIN_SENSORS                            0
#define SETTINGS_MAX_SENSORS                            1
#define SETTINGS_DEFAULT_SENSORS                        1

#define SETTINGS_NAME_ALTITUDE                          "Altitude"
#define SETTINGS_MIN_ALTITUDE                           -16000
#define SETTINGS_MAX_ALTITUDE                           16000
#define SETTINGS_DEFAULT_ALTITUDE                       SETTINGS_NO_DEFAULT
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
typedef enum
{
    SETTINGS_ID_TIMEZONE,
    SETTINGS_ID_MESH_SIZE,
    SETTINGS_ID_SENSORS,
    SETTINGS_ID_ALTITUDE,
    SETTINGS_ID_AMOUT_OF_ELEMENTS
}Settings_Elements_ID_T;
/* END OF THE ENUM AREA */


/* START OF THE STRUCT AREA */
typedef struct
{
    char name[SETTINGS_NAME_LEN];
    Settings_Elements_ID_T ID;
    int16_t min;
    int16_t max;
    int16_t defaultVal;
}Settings_SettingsConstData_T;

typedef struct
{
    int16_t value[SETTINGS_NUMBER_OF_SETTINGS];
    Settings_Elements_ID_T ID;
    const Settings_SettingsConstData_T *ptr;
}Settings_SettingsData_T;
/* END OF THE STRUCT AREA */


/* START OF THE EXTERN VARIABLES AREA */
extern Settings_SettingsData_T settingsData;
extern const Settings_SettingsConstData_T settingsConstData[SETTINGS_NUMBER_OF_SETTINGS];
/* END OF THE EXTERN VARIABLES AREA */


/* START OF THE FUNCTIONS PROTOTYPES AREA */
void Settings_Init(void);
void Settings_SetDefaultValues(void);
/* END OF THE FUNCTIONS PROTOTYPES AREA */

# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* SETTINGS_H */