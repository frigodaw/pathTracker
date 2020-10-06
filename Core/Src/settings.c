/******************************************************************************
* @file           : settings.c
* @brief          : Source for settings.c file.
*                   This file contains defintion of settings structures used
                    by application to restore user data.
******************************************************************************/

#include "settings.h"

/* Variable with all settings data for each element. */
const Settings_SettingsConstData_T settingsConstData[SETTINGS_NUMBER_OF_SETTINGS] =
{
    {
        SETTINGS_NAME_TIMEZONE,
        SETTINGS_ID_TIMEZONE,
        SETTINGS_MIN_TIMEZONE,
        SETTINGS_MAX_TIMEZONE,
        SETTINGS_DEFAULT_TIMEZONE
    },
    {
        SETTINGS_NAME_MESHSIZE,
        SETTINGS_ID_MESH_SIZE,
        SETTINGS_MIN_MESHSIZE,
        SETTINGS_MAX_MESHSIZE,
        SETTINGS_DEFAULT_MESHSIZE
    },
    {
        SETTINGS_NAME_SENSORS,
        SETTINGS_ID_SENSORS,
        SETTINGS_MIN_SENSORS,
        SETTINGS_MAX_SENSORS,
        SETTINGS_DEFAULT_SENSORS
    },
    {
        SETTINGS_NAME_ALTITUDE,
        SETTINGS_ID_ALTITUDE,
        SETTINGS_MIN_ALTITUDE,
        SETTINGS_MAX_ALTITUDE,
        SETTINGS_DEFAULT_ALTITUDE
    },
};


/* Variable which stores current settings values. */
Settings_SettingsData_T settingsData = {0u};



/* Function called only once to assign default values to settings variables. */
void Settings_Init(void)
{
    Settings_SetDefaultValues();
}


/* Function called to set deafult values all settings parameters. */
void Settings_SetDefaultValues(void)
{
    for(uint8_t i = 0u; i < SETTINGS_NUMBER_OF_SETTINGS; i++)
    {
        settingsData.value[i] = settingsConstData[i].defaultVal;
    }
}