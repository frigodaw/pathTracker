#ifndef APPACTIVITYSCREENPRESENTER_HPP
#define APPACTIVITYSCREENPRESENTER_HPP

/* START OF THE INCLUDE AREA */
#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
/* END OF THE DEFINE AREA */

using namespace touchgfx;

/* START OF THE DEFINE AREA */
#define APP_FILENAMEMAXLEN          32u
#define APP_TIME_COEFF1             100u
#define APP_TIME_COEFF2             10000u
#define APP_DATE_COEFF1             100u
#define APP_DATE_COEFF2             10000u
#define APP_MAXFILEBUFFERSIZE       127u
#define APP_LATLON_PRECISION        6u
#define APP_ALTI_PRECISION          1u
#define APP_ALTI_FIRSTCALL          255u
#define APP_ALTI_INTERVAL           10u
#define APP_ALTI_EVEN               2u
#define APP_SLOPE_100PERCENT        100.f
#define APP_SLOPE_MTOKM             1000.f
#define APP_LOCATION_COEFF_DIV      100.f
#define APP_LOCATION_COEFF_MUL      (100.f/60.f)

#define APP_DISTANCE_COEFF_PERIMETER    40075.704f
#define APP_DISTANCE_COEFF_ANGLEFULL    360.f
#define APP_DISTANCE_COEFF_ANGLEHALF    180.f
#define APP_DISTANCE_COEFF_KMTOM        1000.f
#define APP_DISTANCE_COEFF_PI           3.14159f
#define APP_DISTANCE_MAXVALUE_PERSEC    0.03f
#define APP_SLOPE_MAXVALUE_PERSEC       60.f
#define APP_DISTNACE_COEFF_POWER_TWO    2.f
#define APP_SPEED_COEFF_PERIOD          1.f


#define APP_MAINPERIOD_MS           100u
#define APP_MS_IN_SEC               1000u
#define APP_SEC_IN_HR               3600.f
#define APP_MAX_CALL_COUNTER        ((APP_MS_IN_SEC)/(APP_MAINPERIOD_MS))
#define APP_TIMER_COEFF_TOSEC       APP_MAX_CALL_COUNTER
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
typedef enum
{
    APP_STATE_INIT,
    APP_STATE_READY,
    APP_STATE_RUNNING,
    APP_STATE_PAUSED,
    APP_STATE_FINISHED
}AppActivity_activityState_T;

typedef enum
{
    APP_FILE_NOFILE,
    APP_FILE_CREATED,
    APP_FILE_PENDING,
    APP_FILE_CLOSED,
    APP_FILE_ERROR = 0xFF
}AppActivity_fileStatus_T;

typedef enum
{
    APP_SCREEN_MAIN,
    APP_SCREEN_ALTI,
    //APP_SCREEN_ALTICHART,
    APP_SCREEN_MAP,
    //APP_SCREEN_SETTINGS
    APP_MAX_SCREENS
}AppActivity_activeScreen_T;
/* END OF THE ENUM AREA */


/* START OF THE STRUCT AREA */
typedef struct
{
    float latitude;
    float longitude;
    float altitude;
    uint8_t timeSec;
    uint8_t timeMin;
    uint8_t timeHr;
    uint8_t dateYear;
    uint8_t dateMon;
    uint8_t dateDay;
    uint8_t fixQuality;
    uint8_t satellitesNum;
    char latDir;
    char lonDir;
}AppActivity_gpsSignals_T;

typedef struct
{
    float altitude;
    float pressure;
    float temperature;
}AppActivity_sensorData_T;

typedef struct
{
    char name[APP_FILENAMEMAXLEN];
    uint16_t lines;
    AppActivity_fileStatus_T fileStatus;
    uint8_t errorStatus;
    void* filePtr;
}AppActivity_fileInfo_T;

typedef struct
{
    float speed;
    float avgSpeed;
    float maxSpeed;
    float distance;
    uint32_t timer;
    uint32_t time;

    float slope;
    float slopeMax;
    float altitude;
    int32_t altiUp;
    int32_t altiDown;
    int32_t altiMax;
}AppActivity_activityData_T;

typedef struct
{
    float    maxDistancePerSecond;
    float    maxSlopePerSecond;
    uint32_t lastTime;
    uint16_t mainTimePeriod;
    uint8_t  callCounter;
    AppActivity_activeScreen_T screen;
    AppActivity_activityState_T state;
}AppActivity_appInternalData_T;

typedef struct
{
    uint32_t frac;
    int16_t  sint;
}AppActivity_floatToInt_T;
/* END OF THE STRUCT AREA */


class AppActivityScreenView;

class AppActivityScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    AppActivityScreenPresenter(AppActivityScreenView& v);
    ~AppActivityScreenPresenter() {};
    void activate();
    void deactivate();

    void Main(void);
    void StartStopActivity(void);
    void FinishActivity(void);
    void ChangeActivityDataCC(void);
    void SetBitmapButton(const uint16_t bitmapId);
    void InitActivity(void);
    void InsertDataIntoFile(void);
    void ConvertFloatToInt(float data, AppActivity_floatToInt_T &intData, uint8_t precision);
    void ConvertLatLon(float data, float &newData);
    bool IsFix(void);
    void UpdateSignalFixStatus(void);
    void CalculateSpeedAndDistance(void);
    void CalculateAltitude(void);
    void IncrementTimer(void);
    void UpdateTime(void);
    void UpdateAltitude(void);
    template <typename T> T MedianFromArray(T* array, const uint8_t size);
    float MeanFromArray(float* array, const uint8_t size);


    void NotifySignalChanged_gpsData_latitude(float newLatitude);
    void NotifySignalChanged_gpsData_longitude(float newLongitude);
    void NotifySignalChanged_gpsData_altitude(float newAltitude);
    void NotifySignalChanged_gpsData_time(uint32_t newTime);
    void NotifySignalChanged_gpsData_date(uint32_t newDate);
    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum);
    void NotifySignalChanged_gpsData_lonDir(char newLonDir);
    void NotifySignalChanged_gpsData_latDir(char newLatDir);
    void NotifySignalChanged_sensorData_altitude(float newAltitude);
    void NotifySignalChanged_sensorData_pressure(float newPressure);
    void NotifySignalChanged_sensorData_temperature(float newTemperature);

private:
    AppActivityScreenPresenter();
    AppActivityScreenView& view;

    AppActivity_gpsSignals_T        gpsSignals;
    AppActivity_sensorData_T        sensorData;
    AppActivity_fileInfo_T          fileInfo;
    AppActivity_activityData_T      activityData;
    AppActivity_appInternalData_T   appInternalData;
};

#endif // APPACTIVITYSCREENPRESENTER_HPP
