/******************************************************************************
* @file           : envSensors.c
* @brief          : Source for envSensors.c file.
*                   This file contains the source code for BMP280 and MPU9255 sensors.
                    It is not a driver, only a layer between hardware and whole system.
******************************************************************************/

#include "envSensors.h"
#include "math.h"
#include "types.h"


/* Variable to store transformed signals read from sensors. */
ES_SensorData_T sensorData;


/* Main function for EnvSensors component. It is called from default task. */
void EnvSensors_Main(void)
{
    bmp280_read_float(&bmp280, &sensorData.temperature, &sensorData.pressure, NULL);
    float calcAltitude = EnvSensors_CalculateAltitude(sensorData.temperature, sensorData.pressure);

    sensorData.altitude = EnvSensors_FilterAltitude(calcAltitude);
}


/* Function called to convert pressure to Altitude. */
float EnvSensors_CalculateAltitude(float temperature, float pressure)
{
    float calcAltitude = -logf(pressure/ES_P0_SEAPRESSURE)*ES_CELSIUS_TO_KELVIN(temperature)*ES_RUG_COEFF;
    return calcAltitude;
}


/* Function called to filter altitude with kalman filter. */
float EnvSensors_FilterAltitude(float calcAltitude)
{
    static float xpost = 0.f;
    static float Ppost = 0.f;
    static uint8_t firstCall = TRUE;

    static float xpri, Ppri, eps;
    static float S, K;

    if(TRUE == firstCall)
    {
        /* Local init */
        xpost = calcAltitude;
        Ppost = calcAltitude;
        firstCall = FALSE;
    }

    xpri = xpost;
    Ppri = Ppost + ES_ALTIKALMAN_V;
    eps = calcAltitude - xpri;
    S = Ppri + ES_ALTIKALMAN_W;
    K = Ppri/S;
    xpost = xpri + K*eps;
    Ppost = Ppri - K*S*K;

    return xpost;
}
