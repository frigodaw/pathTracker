/******************************************************************************
* @file   : envSensors.h
* @brief  : Header for envSensors.c file.
*           This file contains the common defines and datatypes for parsing and
            calculating data from sensors.
******************************************************************************/

#ifndef ENVSENSORS_H
#define ENVSENSORS_H

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */



/* START OF THE INCLUDE AREA */
#include "stm32f4xx_hal.h"
#include "bmp280.h"
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
#define ES_P0_SEAPRESSURE                   101325.f
#define ES_R_UNIVERSALGASCONSTANT           8.3144598f
#define ES_U_AIRMOLARMASS                   0.0289644f
#define ES_G_STANDARDGRAVITY                9.80665f
#define ES_CELSIUS_TO_KELVIN(celc)          ((celc) + 273.15f)
#define ES_RUG_COEFF                        ((ES_R_UNIVERSALGASCONSTANT)/((ES_U_AIRMOLARMASS)*(ES_G_STANDARDGRAVITY)))

#define ES_ALTIKALMAN_STDDEV                0.1f
#define ES_ALTIKALMAN_TIMEINTERVAL          0.2f
#define ES_ALTIKALMAN_REG                   0.01f
#define ES_ALTIKALMAN_V                     ((ES_ALTIKALMAN_REG)*(ES_ALTIKALMAN_STDDEV)*(ES_ALTIKALMAN_TIMEINTERVAL))
#define ES_ALTIKALMAN_W                     ((ES_ALTIKALMAN_STDDEV)*(ES_ALTIKALMAN_STDDEV))
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
/* END OF THE ENUM AREA */


/* START OF THE TYPEDEF AREA */
typedef struct
{
    float altitude;
    float pressure;
    float temperature;
}ES_SensorData_T;

typedef struct
{
    uint8_t enabled;
}ES_ActivationFlags_T;

typedef struct x
{
    float xpost;
    float Ppost;
    float rawAlti;
    int16_t offset;
    uint8_t firstCall;
}ES_KalmanData_T;
/* END OF THE TYPEDEF AREA */


/* START OF THE EXTERN VARIABLES AREA */
extern BMP280_HandleTypedef bmp280;
extern ES_SensorData_T sensorData;
extern ES_ActivationFlags_T sensorActivationFlags;
/* END OF THE EXTERN VARIABLES AREA */


/* START OF THE FUNCTIONS PROTOTYPES AREA */
void EnvSensors_Main(void);
float EnvSensors_CalculateAltitude(float temperature, float pressure);
float EnvSensors_FilterAltitude(float calcAltitude);
uint8_t EnvSensors_CheckStartConditions(void);
void Env_Sensors_CalibrateAltitude(int16_t newAlti);
/* END OF THE FUNCTIONS PROTOTYPES AREA */


# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* ENVSENSORS_H */