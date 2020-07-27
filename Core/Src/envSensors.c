/******************************************************************************
* @file           : envSensors.c
* @brief          : Source for envSensors.c file.
*                   This file contains the source code for BMP280 and MPU9255 sensors.
                    It is not a driver, only a layer between hardware and whole system.
******************************************************************************/

#include "envSensors.h"
#include "math.h"
#include "types.h"
#include "settings.h"


/* Variable to store transformed signals read from sensors. */
ES_SensorData_T sensorData = {0.f};

/* Variable responsible for activation or deactivation of a sensor. */
ES_ActivationFlags_T sensorActivationFlags = {(uint8_t)SETTINGS_DEFAULT_SENSORS};

/* Variable which stores all kalman filter related data. */
ES_KalmanData_T kalmanData = {0.f, 0.f, 0.f, TRUE};

/* Main function for EnvSensors component. It is called from default task. */
void EnvSensors_Main(void)
{
    uint8_t enableFlag = EnvSensors_CheckStartConditions();
    if(TRUE == enableFlag)
    {
        bmp280_read_float(&bmp280, &sensorData.temperature, &sensorData.pressure, NULL);
        float calcAltitude = EnvSensors_CalculateAltitude(sensorData.temperature, sensorData.pressure);

        sensorData.altitude = EnvSensors_FilterAltitude(calcAltitude);
    }
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
    float xpri, Ppri, eps;
    float S, K;

    calcAltitude += kalmanData.offset;

    if(TRUE == kalmanData.firstCall)
    {
        /* Local init */
        kalmanData.xpost = calcAltitude;
        kalmanData.Ppost = 0.f;
        kalmanData.firstCall = FALSE;
    }

    xpri = kalmanData.xpost;
    Ppri = kalmanData.Ppost + ES_ALTIKALMAN_V;
    eps = calcAltitude - xpri;
    S = Ppri + ES_ALTIKALMAN_W;
    K = Ppri/S;
    kalmanData.xpost = xpri + K*eps;
    kalmanData.Ppost = Ppri - K*S*K;

    return kalmanData.xpost;
}


/* Function called whether it is allowed to enable
   sensors depending on current settings value. */
uint8_t EnvSensors_CheckStartConditions(void)
{
    uint8_t enableFlag = (TRUE == sensorActivationFlags.enabled) ? TRUE : FALSE;
    return enableFlag;
}


/* Function called to set the offset and to modify altitude
   with new user defined value. */
void Env_Sensors_CalibrateAltitude(float newAlti)
{
    kalmanData.offset = newAlti;
    kalmanData.firstCall = TRUE;
}
