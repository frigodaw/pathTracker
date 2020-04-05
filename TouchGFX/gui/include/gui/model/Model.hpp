#ifndef MODEL_HPP
#define MODEL_HPP

#include "stdint.h"

#define MODEL_TICKS_PER_SECOND          60u
#define MODEL_GPSDATA_TIME_INTERVAL     15u     //15:60 = 0.25s = 250ms

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener);
    void tick(void);

    void ReadInputSignals(void);
    void NotifySignalsChanged(void);
    void RefreshScreens(void);

protected:
    ModelListener* modelListener;

private:
    /* neoGps */
    float    gpsData_latitude;
    float    gpsData_longitude;
    float    gpsData_altitude;
    uint16_t gpsData_read;
    uint16_t gpsData_write;
    uint8_t  gpsData_timeHr;
    uint8_t  gpsData_timeMin;
    uint8_t  gpsData_timeSec;
    uint8_t  gpsData_dateDay;
    uint8_t  gpsData_dateMon;
    uint8_t  gpsData_dateYear;
    uint8_t  gpsData_fixQuality;
    uint8_t  gpsData_satellitesNum;
    char     gpsData_lonDir;
    char     gpsData_latDir;
    /* fileSystem */
    uint32_t sdCardInfo_totalSpace;
    uint32_t sdCardInfo_freeSpace;
    uint8_t  sdCardInfo_state;
    uint8_t  dirInfo_in_filesNum;
    uint8_t  dirInfo_out_filesNum;
    /* internal data */
    uint8_t modelTicks;
};

#endif // MODEL_HPP
