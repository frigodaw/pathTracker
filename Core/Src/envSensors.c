/******************************************************************************
* @file           : envSensors.c
* @brief          : Source for envSensors.c file.
*                   This file contains the source code for BMP280 and MPU9255 sensors.
                    It is not a driver, only a layer between hardware and whole system.
******************************************************************************/

#include "envSensors.h"
#include "math.h"


/* Variable to store transformed signals read from sensors. */
ES_SensorData_T sensorData;


/* Main function for EnvSensors component. It is called from default task. */
void EnvSensors_Main(void)
{
    bmp280_read_float(&bmp280, &sensorData.temperature, &sensorData.pressure, NULL);
    EnvSensors_CalculateAltitude(sensorData.temperature, sensorData.pressure);
}


/* Function called to convert pressure to altitude. */
void EnvSensors_CalculateAltitude(float temperature, float pressure)
{
    sensorData.altitude = -logf(pressure/ES_P0_SEAPRESSURE)*ES_CELSIUS_TO_KELVIN(temperature)*ES_RUG_COEFF;
}
