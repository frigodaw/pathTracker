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
#define APP_ALT_PRECISION           1u
#define APP_LOCATION_COEFF_DIV      100.f
#define APP_LOCATION_COEFF_MUL      (100.f/60.f)

#define APP_MAINPERIOD_MS           100u
#define APP_MS_IN_SEC               1000u
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
typedef enum
{
    APP_READY,
    APP_RUNNING,
    APP_PAUSED,
    APP_FINISHED
}AppActivity_activityState_T;

typedef enum
{
    APP_NOFILE,
    APP_FILECREATED,
    APP_FILECLOSED,
    APP_FILEERROR
}AppActivity_fileStatus;
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
    char name[APP_FILENAMEMAXLEN];
    uint16_t lines;
    AppActivity_fileStatus fileStatus;
    uint8_t errorStatus;
    void* filePtr;
}AppActivity_fileInfo_T;

typedef struct
{
    float speed;
    float avgSpeed;
    float distance;
    uint32_t timer;
    AppActivity_activityState_T state;
}AppActivity_activityData_T;

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
    void SetBitmapButton(const uint16_t bitmapId);
    void InitActivity(void);
    void InsertDataIntoFile(void);
    void ConvertFloatToInt(float data, AppActivity_floatToInt_T &intData, uint8_t precision);
    void ConvertLatLon(float data, float &newData);

    void NotifySignalChanged_gpsData_latitude(float newLatitude);
    void NotifySignalChanged_gpsData_longitude(float newLongitude);
    void NotifySignalChanged_gpsData_altitude(float newAltitude);
    void NotifySignalChanged_gpsData_time(uint32_t newTime);
    void NotifySignalChanged_gpsData_date(uint32_t newDate);
    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum);
    void NotifySignalChanged_gpsData_lonDir(char newLonDir);
    void NotifySignalChanged_gpsData_latDir(char newLatDir);

private:
    AppActivityScreenPresenter();
    AppActivityScreenView& view;


    AppActivity_gpsSignals_T        gpsSignals;
    AppActivity_fileInfo_T          fileInfo;
    AppActivity_activityData_T      activityData;
    uint16_t                        mainTimePeriod;
};

#endif // APPACTIVITYSCREENPRESENTER_HPP
