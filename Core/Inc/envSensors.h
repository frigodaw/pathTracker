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
/* END OF THE TYPEDEF AREA */


/* START OF THE EXTERN VARIABLES AREA */
extern BMP280_HandleTypedef bmp280;
extern ES_SensorData_T sensorData;
/* END OF THE EXTERN VARIABLES AREA */


/* START OF THE FUNCTIONS PROTOTYPES AREA */
void EnvSensors_Main(void);
void EnvSensors_CalculateAltitude(float temperature, float pressure);
/* END OF THE FUNCTIONS PROTOTYPES AREA */


# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* ENVSENSORS_H */