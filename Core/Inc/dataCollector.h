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
uint16_t DC_get_neoGps_gpsData_write(void);
void DC_set_neoGps_gpsData_write(uint16_t value);
void DC_add_neoGps_gpsData_write(uint16_t value);
Gps_bufferState DC_get_neoGps_gpsData_state(void);
/* END OF THE FUNCTIONS PROTOTYPES AREA */

# ifdef __cplusplus
}
# endif /* __cplusplus */
#endif  /* DATACOLLECTOR_H */