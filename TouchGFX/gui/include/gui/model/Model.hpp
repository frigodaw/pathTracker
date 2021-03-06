#ifndef MODEL_HPP
#define MODEL_HPP

/* START OF THE INCLUDE AREA */
#include "stdint.h"
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
#define MODEL_TICKS_PER_SEC     60u
#define MODEL_MS_IN_SEC         1000u
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
/* END OF THE ENUM AREA */


/* START OF THE TYPEDEF AREA */
typedef struct
{
    uint8_t gpsData_latitude;
    uint8_t gpsData_longitude;
    uint8_t gpsData_altitude;
    uint8_t gpsData_date;
    uint8_t gpsData_time;
    uint8_t gpsData_read;
    uint8_t gpsData_write;
    uint8_t gpsData_fixQuality;
    uint8_t gpsData_satellitesNum;
    uint8_t gpsData_lonDir;
    uint8_t gpsData_latDir;
    uint8_t sdCardInfo_totalSpace;
    uint8_t sdCardInfo_freeSpace;
    uint8_t sdCardInfo_state;
    uint8_t dirInfo_in_filesNum;
    uint8_t dirInfo_out_filesNum;
    uint8_t sensorData_altitude;
    uint8_t sensorData_pressure;
    uint8_t sensorData_temperature;
} Model_dataNotifier_T;
/* END OF THE TYPEDEF AREA */

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener);
    void tick(void);

    /* Signal processing */
    void ReadInputSignals(void);
    void NotifyScreens(void);
    void TryCallPresenterMainFunction(void);
    void SignalRequestFromPresenter(void);
    void MainPeriodFromPresenter(uint16_t timePeriod);
    template <typename T> void UpdateElement(T (*getDataPtr)(void), T &currentData, uint8_t &dataNotifier);
    template <typename T> void NotifyElement(void (ModelListener::*notifySignalChangedElement)(T), T currentData, uint8_t &dataNotifier);

protected:
    ModelListener* modelListener;

private:
    /* neoGps */
    float    gpsData_latitude;
    float    gpsData_longitude;
    float    gpsData_altitude;
    uint32_t gpsData_date;
    uint32_t gpsData_time;
    uint16_t gpsData_read;
    uint16_t gpsData_write;
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
    /* envSensors */
    float    sensorData_altitude;
    float    sensorData_pressure;
    float    sensorData_temperature;
    /* internal data */
    Model_dataNotifier_T dataNotifier;
    uint16_t mainTimePeriod;
    uint8_t  timeZoneOffset;
    uint8_t  modelTicks;
};

#endif // MODEL_HPP
