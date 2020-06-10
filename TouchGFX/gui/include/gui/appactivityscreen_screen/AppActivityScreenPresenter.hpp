#ifndef APPACTIVITYSCREENPRESENTER_HPP
#define APPACTIVITYSCREENPRESENTER_HPP

/* START OF THE INCLUDE AREA */
#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
/* END OF THE DEFINE AREA */

using namespace touchgfx;

/* START OF THE DEFINE AREA */
#define POW2(x)         ((x)*(x))

#define APP_GPXFILE_SHORTMODE

#define APP_SDCARD_INITIALIZED      1u
#define APP_FILENAMEMAXLEN          32u
#define APP_TIME_COEFF1             100u
#define APP_TIME_COEFF2             10000uL
#define APP_DATE_COEFF1             100u
#define APP_DATE_COEFF2             10000uL
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

#define APP_DISTANCE_MIN_POINTS_NUM     2u
#define APP_DISTANCE_COEFF_PERIMETER    40075.704f
#define APP_DISTANCE_COEFF_ANGLEFULL    360.f
#define APP_DISTANCE_COEFF_ANGLEHALF    180.f
#define APP_DISTANCE_COEFF_M_IN_KM      1000.f
#define APP_DISTANCE_COEFF_PI           3.14159f
#define APP_DISTANCE_COEFF_TORAD        ((APP_DISTANCE_COEFF_PI)/(APP_DISTANCE_COEFF_ANGLEHALF))
#define APP_DISTANCE_COEFF_TONUM        ((APP_DISTANCE_COEFF_PERIMETER)/(APP_DISTANCE_COEFF_ANGLEFULL))
#define APP_DISTANCE_MAXVALUE_M_S       50.f
#define APP_DISTANCE_MAXVALUE_KM_S      ((APP_DISTANCE_MAXVALUE_M_S)/(APP_DISTANCE_COEFF_M_IN_KM))
#define APP_SLOPE_MAXVALUE_PERSEC       60.f
#define APP_DISTNACE_COEFF_POWER_TWO    2.f
#define APP_SPEED_COEFF_PERIOD          1.f

#define APP_MAINPERIOD_MS           100u
#define APP_MS_IN_SEC               1000uL
#define APP_SEC_IN_HR               3600.f
#define APP_MAX_CALL_COUNTER        ((APP_MS_IN_SEC)/(APP_MAINPERIOD_MS))
#define APP_MAX_FILE_COUNTER        60u
#define APP_TIMER_COEFF_TOSEC       APP_MAX_CALL_COUNTER

#define APP_TRACK_WINDOW_WIDTH_PX       240u
#define APP_TRACK_WINDOW_HEIGHT_PX      205u
#define APP_TRACK_WINDOW_MID_X          ((uint8_t)((APP_TRACK_WINDOW_WIDTH_PX)/(2u)))
#define APP_TRACK_WINDOW_MID_Y          ((uint8_t)((APP_TRACK_WINDOW_HEIGHT_PX)/(2u)))
#define APP_TRACK_WINDOW_UPLEFT_X       0u
#define APP_TRACK_WINDOW_UPLEFT_Y       0u
#define APP_TRACK_WINDOW_UPRIGHT_X      (APP_TRACK_WINDOW_WIDTH_PX-1u)
#define APP_TRACK_WINDOW_UPRIGHT_Y      0u
#define APP_TRACK_WINDOW_BOTTOMLEFT_X   0u
#define APP_TRACK_WINDOW_BOTTOMLEFT_Y   (APP_TRACK_WINDOW_HEIGHT_PX-1u)
#define APP_TRACK_WINDOW_BOTTOMRIGHT_X  (APP_TRACK_WINDOW_WIDTH_PX-1u)
#define APP_TRACK_WINDOW_BOTTOMRIGHT_Y  (APP_TRACK_WINDOW_HEIGHT_PX-1u)
#define APP_TRACK_FLOATBUFF_SIZE        32u
#define APP_TRACK_FLOATBUFF_HALF        ((APP_TRACK_FLOATBUFF_SIZE)/(2u))
#define APP_TRACK_MEANTWO_COEEF         2.f
#define APP_TRACK_SCALE_WIDTH_PX        100u
#define APP_TRACK_SCALE_COEFF_M_IN_KM   1000uL

#define APP_TRACK_SCALE_50              50u
#define APP_TRACK_SCALE_100             100u
#define APP_TRACK_SCALE_500             500u
#define APP_TRACK_SCALE_1000            1000uL
#define APP_TRACK_SCALE_5000            5000uL
#define APP_TRACK_SCALE_FULL_COEFFVIEW  1.75f
#define APP_TRACK_SCALE_FULL_COEFFROUND 1000uL
#define APP_TRACK_SCALE_ERROR           0u

#define APP_TRACK_SKIPPED_COORDS_50     0u
#define APP_TRACK_SKIPPED_COORDS_100    0u
#define APP_TRACK_SKIPPED_COORDS_500    1u
#define APP_TRACK_SKIPPED_COORDS_1000   2u
#define APP_TRACK_SKIPPED_COORDS_5000   5u
#define APP_TRACK_SKIP_DRAWN_POINTS_BOTTOM_LIMIT    120u
#define APP_TRACK_SKIP_DRAWN_POINTS_UPPER_LIMIT     190u
#define APP_TRACK_SKIP_MAX_VALUE        255u

#define APP_TRACK_FILE_HEADEROFFSET     180u
#define APP_TRACK_FILE_DATAOFFSET       101u
#define APP_TRACK_FILE_READLINES        32u

#define APP_TRACK_MAP_ELEMENTS          200u
#define APP_TRACK_COMMONARRAY_LENGTH    8192uL
#define APP_TRACK_TRACK_FIRST_ELEMENT   0u
#define APP_TRACK_MAP_FIRST_ELEMENT     (APP_TRACK_COMMONARRAY_LENGTH - 1uL)
#define APP_TRACK_SKIP_BOTTOM_LIMIT     1u
#define APP_TRACK_MAP_OFFSET_FOR_TRACK  1u

#define APP_COORDS_FILTER_ARR_SIZE      3u
#define APP_COORDS_FILTER_IDX_INIT      255u

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

typedef enum
{
    APP_TRACK_SCALE50,
    APP_TRACK_SCALE100,
    APP_TRACK_SCALE500,
    APP_TRACK_SCALE1000,
    APP_TRACK_SCALE5000,
    APP_TRACK_SCALEFULL,
    APP_TRACK_MAX_SCALE
}AppActivity_trackScale_T;

typedef enum
{
    APP_TRACK_LATSTART,
    APP_TRACK_LATEND,
    APP_TRACK_LONSTART,
    APP_TRACK_LONEND,
    APP_TRACK_FOUNDALL
}AppActivity_trackCoordsFromBuffer_T;
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
    uint8_t state;
}AppActivity_sdCardData_T;
typedef struct
{
    char name[APP_FILENAMEMAXLEN];
    uint16_t points;
    uint8_t errorStatus;
    AppActivity_fileStatus_T fileStatus;
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
    uint8_t  fileCounter;
    AppActivity_activeScreen_T screen;
    AppActivity_activityState_T state;
}AppActivity_appInternalData_T;

typedef struct
{
    uint32_t frac;
    int16_t  sint;
}AppActivity_floatToInt_T;

typedef struct
{
    float lat;
    float lon;
}AppActivity_coordinatesGPS_T;

typedef struct
{
    uint8_t X;
    uint8_t Y;
}AppActivity_coordinatesXY_T;

typedef struct
{
    AppActivity_coordinatesGPS_T center;
    AppActivity_coordinatesGPS_T upLeft;
    AppActivity_coordinatesGPS_T upRight;
    AppActivity_coordinatesGPS_T bottomLeft;
    AppActivity_coordinatesGPS_T bottomRight;
}AppActivity_mapCoordsGPS_T;

typedef struct
{
    AppActivity_coordinatesGPS_T center;
    AppActivity_coordinatesGPS_T N;
    AppActivity_coordinatesGPS_T E;
    AppActivity_coordinatesGPS_T W;
    AppActivity_coordinatesGPS_T S;
}AppActivity_maxCoordsGPS_T;

typedef struct
{
    AppActivity_coordinatesXY_T center;
    AppActivity_coordinatesXY_T upLeft;
    AppActivity_coordinatesXY_T upRight;
    AppActivity_coordinatesXY_T bottomLeft;
    AppActivity_coordinatesXY_T bottomRight;
}AppActivity_mapCoordsXY_T;

typedef struct
{
    AppActivity_mapCoordsGPS_T mapCoordsGPS;
    AppActivity_maxCoordsGPS_T maxCoordsGPS;
    AppActivity_mapCoordsXY_T  *mapCoordsXY;
    AppActivity_trackScale_T   scale;
    uint8_t                    addedPoints;
    uint8_t                    skip;
}AppActivity_trackData_T;

typedef struct
{
    AppActivity_coordinatesGPS_T *coords;
    uint16_t idxTrack;
    uint16_t idxMap;
    bool     overflow;
}AppActivity_trackPointsData_T;
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
    void ZoomIn(void);
    void ZoomOut(void);
    void ChangeActivityDataCC(void);
    void SetBitmapButton(const uint16_t bitmapId);
    void InitActivity(void);
    void InsertDataIntoFile(void);
    void ConvertFloatToInt(float data, AppActivity_floatToInt_T &intData, uint8_t precision);
    void ConvertLatLon(float data, float &newData);
    bool IsFix(void);
    bool IsSdCard(void);
    void UpdateSignalFixStatus(void);
    void UpdateSignalSdCard(void);
    void CalculateSpeedAndDistance(void);
    float CalculateDistance(float lat, float lon, float lastLat, float lastLon);
    void CalculateAltitude(void);
    void IncrementTimer(void);
    void UpdateTime(void);
    void UpdateAltitude(void);
    template <typename T> T MedianFromArray(T* array, const uint8_t size);
    float MeanFromArray(float* array, const uint8_t size);
    void DrawTrack(void);
    void MapCenterCoordinates(void);
    uint32_t GetScaleValue(void);
    uint32_t CalculateFullScale(void);
    float MapScaleToDistance(void);
    void CalculateSkippedCoords(void);
    AppActivity_coordinatesGPS_T MapXYCoordsToGPS(AppActivity_coordinatesXY_T coords, float scaleCoeff);
    AppActivity_coordinatesGPS_T GetCoordsGPSFromBuffer(uint8_t* buffer, uint8_t size);
    bool CoordsInView(AppActivity_coordinatesGPS_T coords);
    AppActivity_coordinatesXY_T MapGPSCoordsToXY(AppActivity_coordinatesGPS_T coords);
    float MapPointToLinearFunction(float x1, float y1, float x2, float y2, float X);
    uint32_t CalculateFileOffset(uint16_t points);
    void SaveCoordsInCcmRam(float lat, float lon);
    void FindMaxCoords(float lat, float lon);
    void SaveFile(void);
    void FilterCoords(void);


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
    void NotifySignalChanged_sdCardInfo_state(uint8_t state);

private:
    AppActivityScreenPresenter();
    AppActivityScreenView& view;

    AppActivity_gpsSignals_T        gpsSignals;
    AppActivity_sensorData_T        sensorData;
    AppActivity_sdCardData_T        sdCardData;
    AppActivity_fileInfo_T          fileInfo;
    AppActivity_activityData_T      activityData;
    AppActivity_appInternalData_T   appInternalData;
    AppActivity_trackData_T         trackData;
    AppActivity_trackPointsData_T   trackPointsData;
};

#endif // APPACTIVITYSCREENPRESENTER_HPP
