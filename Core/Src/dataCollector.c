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

/* ---------- main ---------- */
/*  File: main.c
    Type: getter
    Variable: tim.t_1s
*/
uint16_t DC_get_main_tim_t_1s(void)
{
    return tim.t_1s;
}
/* ---------- main ---------- */


/* ---------- neoGps ---------- */
/*  File: neoGps.c
    Type: getter
    Variable: gpsData.write
*/
uint16_t DC_get_neoGps_gpsData_write(void)
{
    return gpsData.write;
}


/*  File: neoGps.c
    Type: setter
    Variable: gpsData.write
*/
void DC_set_neoGps_gpsData_write(uint16_t value)
{
    gpsData.write = value;
}


/*  File: neoGps.c
    Type: add and set
    Variable: gpsData.write
*/
void DC_add_neoGps_gpsData_write(uint16_t value)
{
    gpsData.write += value;
}


/*  File: neoGps.c
    Type: getter
    Variable: gpsData.state
*/
Gps_bufferState DC_get_neoGps_gpsData_state(void)
{
    return gpsData.state;
}
/* ---------- neoGps ---------- */