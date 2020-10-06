#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "BitmapDatabase.hpp"
#include "filesystem.h"
#include "dataCollector.h"


/* Const array containg number of days per each month. */
static const uint8_t daysInMon[APP_TIMEZONE_NUMOFMONTHS] = { APP_TIMEZONE_DAYS_JAN, APP_TIMEZONE_DAYS_FEB, APP_TIMEZONE_DAYS_MAR,
                                                     APP_TIMEZONE_DAYS_APR, APP_TIMEZONE_DAYS_MAY, APP_TIMEZONE_DAYS_JUN,
                                                     APP_TIMEZONE_DAYS_JUL, APP_TIMEZONE_DAYS_AUG, APP_TIMEZONE_DAYS_SEP,
                                                     APP_TIMEZONE_DAYS_OCT, APP_TIMEZONE_DAYS_NOV, APP_TIMEZONE_DAYS_DEC };

/* Common array to store lat and lon coordinates recorded during activity 
   and from preloaded .gpx file. First part is for tracked coords, second one 
   for points to navigate. */
AppActivity_coordinatesGPS_T routePoints[APP_ROUTE_COMMONARRAY_LENGTH] __attribute__ ((section (".ccm"))) = {0.f};

static const AppActivity_mapCoordsXY_T mapCoordsXY = {{APP_ROUTE_WINDOW_MID_X, APP_ROUTE_WINDOW_MID_Y},
                                                      {APP_ROUTE_WINDOW_UPLEFT_X, APP_ROUTE_WINDOW_UPLEFT_Y},
                                                      {APP_ROUTE_WINDOW_UPRIGHT_X, APP_ROUTE_WINDOW_UPRIGHT_Y},
                                                      {APP_ROUTE_WINDOW_BOTTOMLEFT_X, APP_ROUTE_WINDOW_BOTTOMLEFT_Y},
                                                      {APP_ROUTE_WINDOW_BOTTOMRIGHT_X, APP_ROUTE_WINDOW_BOTTOMRIGHT_Y}};

AppActivityScreenPresenter::AppActivityScreenPresenter(AppActivityScreenView& v)
    : view(v)
{
    memset(&activityData, 0u, sizeof(activityData));
    memset(&appInternalData, 0u, sizeof(appInternalData));
    memset(&mapFileInfo, 0u, sizeof(mapFileInfo));
    memset(&trackFileInfo, 0u, sizeof(trackFileInfo));
    memset(&gpsSignals, 0u, sizeof(gpsSignals));
    memset(&sensorData, 0u, sizeof(sensorData));
    memset(&routeData, 0u, sizeof(routeData));
    memset(&altitudeInfo, 0u, sizeof(altitudeInfo));
    memset(&calendar, 0u, sizeof(calendar));
    memset(&mapIndex, 0u, sizeof(mapIndex));

    appInternalData.state = APP_STATE_INIT;
    appInternalData.screen = APP_SCREEN_NONE;
    appInternalData.maxDistancePerSecond = APP_DISTANCE_MAXVALUE_KM_S;
    appInternalData.maxSlopePerSecond = APP_SLOPE_MAXVALUE_PERSEC;
    appInternalData.mainTimePeriod = APP_MAINPERIOD_MS;
    appInternalData.callCounter = APP_MAX_CALL_COUNTER;
    appInternalData.sensorEnabled = false;
    appInternalData.timezone = DC_get_appSettings_settingsData_value_timezone();

    mapFileInfo.fileStatus = APP_FILE_NOFILE;
    mapFileInfo.errorStatus = APP_FILEERROR_NOERROR;
    trackFileInfo.fileStatus = APP_FILE_NOFILE;
    trackFileInfo.errorStatus = APP_FILEERROR_NOERROR;

    routeData.scale = APP_ROUTE_SCALEFULL;
    routeData.mapCoordsXY = (AppActivity_mapCoordsXY_T*)&mapCoordsXY;

    routePointsData.coords = routePoints;
    routePointsData.idxTrack = APP_ROUTE_TRACK_FIRST_ELEMENT;
    routePointsData.idxMap = APP_ROUTE_MAP_FIRST_ELEMENT;

    mapIndex.max = DC_get_fileSystem_dirInfo_in_filesNum();

    SelectAltitudeSource();
    SetBitmapButton(BITMAP_BLUE_ICONS_PLAY_32_ID);
}

void AppActivityScreenPresenter::activate()
{
    uint32_t scaleVal = GetScaleValue();
    view.SetRouteScale(scaleVal);

    appInternalData.screen = APP_SCREEN_SELECTMAP;
    view.SetActivityDataScreen(appInternalData.screen);

    model->SignalRequestFromPresenter();
    model->MainPeriodFromPresenter(appInternalData.mainTimePeriod);

    DC_set_fileSystem_sdCardInfo_blocked((uint8_t)true);

    GetMapInfo();

    appInternalData.state = APP_STATE_READY;
}

void AppActivityScreenPresenter::deactivate()
{
    DC_set_fileSystem_sdCardInfo_blocked((uint8_t)false);
}


/* Method called after pause/start button
   is pressed. Its point is to change appInternalData.state
   to proper state. */
void AppActivityScreenPresenter::StartStopActivity(void)
{
    if(APP_STATE_READY == appInternalData.state)
    {
        InitActivity();
    }
    else if(APP_STATE_PAUSED == appInternalData.state)
    {
        appInternalData.state = APP_STATE_RUNNING;
        SetBitmapButton(BITMAP_BLUE_ICONS_PAUSE_32_ID);
    }
    else if(APP_STATE_RUNNING == appInternalData.state)
    {
        appInternalData.state = APP_STATE_PAUSED;
        SetBitmapButton(BITMAP_BLUE_ICONS_PLAY_32_ID);
    }
}


/* Method called when starting an activity to
   prepare output file. */
void AppActivityScreenPresenter::InitActivity(void)
{
    if(true == IsFix())
    {
        appInternalData.lastTime = DC_get_main_tim_t_100ms();

        snprintf(trackFileInfo.name, APP_FILENAMEMAXLEN, "%s/20%.2d%.2d%.2d_%.2d%.2d%.2d.gpx",
                FS_OUTPUTPATH, calendar.date.year, calendar.date.mon, calendar.date.day,
                calendar.time.hr, calendar.time.min, calendar.time.sec);

        view.SetArrowVisibilityStatus(true);

        uint8_t retVal = FS_OpenFile((FS_File_T**)&trackFileInfo.filePtr, trackFileInfo.name, FS_MODEWRITE);
        if(RET_OK == retVal)
        {
            trackFileInfo.errorStatus &= ~APP_FILEERROR_OPENFILE;
            trackFileInfo.fileStatus = APP_FILE_CREATED;
            appInternalData.state = APP_STATE_RUNNING;
            SetBitmapButton(BITMAP_BLUE_ICONS_PAUSE_32_ID);

            const char* initBufferOne = 
                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<gpx version=\"1.1\" creator=\"Path Tracker\">\n"
                "\t<metadata>\n";
            const char* initBufferTwo = 
                "\t\t<name>Activity</name>\n"
                "\t</metadata>\n"
                "\t<trk>\n"
                "\t\t<name>Cycling</name>\n"
                "\t\t<trkseg>\n";
            retVal |= FS_WriteFile((FS_File_T*)trackFileInfo.filePtr, (uint8_t*)initBufferOne);
            retVal |= FS_WriteFile((FS_File_T*)trackFileInfo.filePtr, (uint8_t*)initBufferTwo);

            if(RET_OK == retVal)
            {
                trackFileInfo.errorStatus &= ~APP_FILEERROR_WRITEDATA;
            }
            else
            {
                trackFileInfo.errorStatus |= APP_FILEERROR_WRITEDATA;
            }
        }
        else
        {
            /* File can not be created */
            trackFileInfo.fileStatus = APP_FILE_ERROR;
            trackFileInfo.errorStatus |= APP_FILEERROR_OPENFILE;
        }
    }
    else
    {
        /* No fix, file will not be created */
        trackFileInfo.fileStatus = APP_FILE_PENDING;
    }
}


/* Method called to finish activity, close file and
   to change appInternalData.state. */
void AppActivityScreenPresenter::FinishActivity(void)
{
    if(APP_FILE_NOFILE != trackFileInfo.fileStatus)
    {
        const char* finishBuffer = 
            "\t\t</trkseg>\n"
            "\t</trk>\n"
            "</gpx>";
        uint8_t retVal = RET_OK;

        retVal = FS_WriteFile((FS_File_T*)trackFileInfo.filePtr, (uint8_t*)finishBuffer);
        if(RET_OK == retVal)
        {
            trackFileInfo.errorStatus &= ~APP_FILEERROR_WRITEDATA;
        }
        else
        {
            trackFileInfo.errorStatus |= APP_FILEERROR_WRITEDATA;
        }

        retVal = FS_CloseFile((FS_File_T**)&trackFileInfo.filePtr);
        if(RET_OK == retVal)
        {
            trackFileInfo.fileStatus = APP_FILE_CLOSED;
            trackFileInfo.errorStatus &= ~APP_FILEERROR_FINISHACTIVITY;
        }
        else
        {
            trackFileInfo.fileStatus = APP_FILE_ERROR;
            trackFileInfo.errorStatus |= APP_FILEERROR_FINISHACTIVITY;
        }
        trackFileInfo.filePtr = NULL;
        appInternalData.state = APP_STATE_FINISHED;
    }
}


/* Main method, triggered periodically by model. */
void AppActivityScreenPresenter::Main(void)
{
    if(APP_FILE_CREATED == trackFileInfo.fileStatus)
    {
        if(APP_STATE_RUNNING == appInternalData.state)
        {
            IncrementTimer();

            appInternalData.callCounter++;

            /* This parts of code executes every 1 second */
            if(appInternalData.callCounter >= APP_MAX_CALL_COUNTER)
            {
                appInternalData.callCounter = 0u;
                appInternalData.fileCounter++;

                 /* Gps related method can execute only if signal is fixed */
                if(true == IsFix())
                {
                    FilterCoords();

                    InsertDataIntoFile();

                    CalculateSpeedAndDistance();
                }

                CalculateAltitude();

                /* TODO: Move this calls to IsFix section */
                DrawRoute(APP_DRAWROUTE_MAP);
                DrawRoute(APP_DRAWROUTE_TRACK);
            }

            if(appInternalData.fileCounter >= APP_MAX_FILE_COUNTER)
            {
                appInternalData.fileCounter= 0u;

                SaveFile();
            }
            
        }
    }
    else if(APP_FILE_PENDING == trackFileInfo.fileStatus)
    {
        InitActivity();
    }
}


/* Method called after "NEXT" button is clicked to change
   visible data widget to another one. */
void AppActivityScreenPresenter::ChangeActivityDataCC(void)
{
    appInternalData.screen = ((appInternalData.screen + 1u) >= APP_MAX_SCREENS) ? APP_SCREEN_MAIN : (AppActivity_activeScreen_T)(appInternalData.screen + 1u);
    view.SetActivityDataScreen(appInternalData.screen);
}


/* Method called to set proper bitmap on
   the button */
void AppActivityScreenPresenter::SetBitmapButton(const uint16_t bitmapId)
{
    view.SetBitmapButton(bitmapId);
}


/* Method called to insert gps-related data
   info gpx file. */
void AppActivityScreenPresenter::InsertDataIntoFile(void)
{
    uint8_t retVal = RET_OK;
    uint8_t buffer[APP_MAXFILEBUFFERSIZE] = {0u};

    AppActivity_floatToInt_T lat = {0u};
    AppActivity_floatToInt_T lon = {0u};
    AppActivity_floatToInt_T alt = {0u};

    SaveCoordsInCcmRam(gpsSignals.latitude, gpsSignals.longitude);
    FindMaxCoords(gpsSignals.latitude, gpsSignals.longitude);

    ConvertFloatToInt(gpsSignals.latitude,  lat, APP_LATLON_PRECISION);
    ConvertFloatToInt(gpsSignals.longitude, lon, APP_LATLON_PRECISION);
    ConvertFloatToInt(*altitudeInfo.ptrValue,  alt, APP_ALTI_PRECISION);

#ifdef APP_GPXFILE_SHORTMODE
    snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE,
        "\t\t\t<trkpt lat=\"%d.%.*lu\" lon=\"%d.%.*lu\"><ele>%d.%.*lu</ele><time>20%.2d-%.2d-%.2dT%.2d:%.2d:%.2d.000Z</time></trkpt>\n",
        lat.sint, APP_LATLON_PRECISION, lat.frac, lon.sint, APP_LATLON_PRECISION, lon.frac, alt.sint, APP_ALTI_PRECISION, alt.frac,
        calendar.date.year, calendar.date.mon, calendar.date.day, calendar.time.hr, calendar.time.min, calendar.time.sec);
    retVal = FS_WriteFile((FS_File_T*)trackFileInfo.filePtr, (uint8_t*)buffer);

    if(RET_OK == retVal)
    {
        trackFileInfo.errorStatus &= ~APP_FILEERROR_WRITEDATA;
    }
    else
    {
        trackFileInfo.errorStatus |= APP_FILEERROR_WRITEDATA;
    }

#else   /* Default saving mode */
    snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE,
        "\t\t\t<trkpt lat=\"%d.%.*lu\" lon=\"%d.%.*lu\">\n"
        "\t\t\t\t<ele>%d.%.*lu</ele>\n",
        lat.sint, APP_LATLON_PRECISION, lat.frac, lon.sint, APP_LATLON_PRECISION, lon.frac, alt.sint, APP_ALTI_PRECISION, alt.frac);
    trackFileInfo.errorStatus = FS_WriteFile((FS_File_T*)trackFileInfo.filePtr, (uint8_t*)buffer);

    memset(buffer, 0u, sizeof(buffer));
    snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE,
        "\t\t\t\t<time>20%.2d-%.2d-%.2dT%.2d:%.2d:%.2d.000Z</time>\n"
        "\t\t\t</trkpt>\n",
        calendar.date.year, calendar.date.mon, calendar.date.day, calendar.time.hr, calendar.time.min, calendar.time.sec);
    trackFileInfo.errorStatus = FS_WriteFile((FS_File_T*)trackFileInfo.filePtr, (uint8_t*)buffer);
#endif

    UpdateSignalSdCard();
    trackFileInfo.points++;
}


/* Method called to convert float to interger without loosing precision. */
void AppActivityScreenPresenter::ConvertFloatToInt(float data, AppActivity_floatToInt_T &intData, uint8_t precision)
{
    uint32_t precCoeff = 10u;
    for(uint8_t i = 1u; i < precision; i++)
    {
        precCoeff *= 10u;
    }

    intData.sint = (int16_t)data;
    intData.frac = (uint32_t)((data - (float)intData.sint) * (float)precCoeff);
}


/* Method called to convert float gps latitude and longitude signals
   to demanded format. */
void AppActivityScreenPresenter::ConvertLatLon(float data, float  &newData)
{
    float convData = (uint16_t)((uint16_t)data / APP_LOCATION_COEFF_DIV);
    float convDataFrac = (data / APP_LOCATION_COEFF_DIV) - convData;
    convData += convDataFrac * APP_LOCATION_COEFF_MUL;

    newData = convData;
}


/* Method called to tell check whether gps signal
   is fixed or not. */
bool AppActivityScreenPresenter::IsFix(void)
{
    bool retVal = false;

    if(0u != gpsSignals.fixQuality)
    {
        retVal = true;
        appInternalData.wasFix = true;
    }
    else
    {
        retVal = false;
    }

    return retVal;
}


/* Method called to update fix icon on the screen. */
void AppActivityScreenPresenter::UpdateSignalFixStatus(void)
{
    bool isFix = IsFix();

    view.ShowFixImage(isFix);
}


/* Method called to tell check whether sd card is
   inserted correctly or not. */
bool AppActivityScreenPresenter::IsSdCard(void)
{
    bool retVal = false;

    if((APP_SDCARD_INITIALIZED == sdCardData.state) && (APP_FILE_ERROR != trackFileInfo.fileStatus) && (APP_FILEERROR_NOERROR == trackFileInfo.errorStatus))
    {
        retVal = true;
    }
    else
    {
        retVal = false;
    }

    return retVal;
}


/* Method called to update sd card icon on the screen. */
void AppActivityScreenPresenter::UpdateSignalSdCard(void)
{
    bool isSdCard = IsSdCard();

    view.ShowSdCard(isSdCard);
}


/* Method called to calculate distance between two points. */
float AppActivityScreenPresenter::CalculateDistance(float lat, float lon, float lastLat, float lastLon)
{
    float distance = sqrtf( POW2( (lat - lastLat) ) + 
                            POW2( (cosf(lastLat * APP_DISTANCE_COEFF_TORAD) * (lon - lastLon) ) ) 
                          ) * APP_DISTANCE_COEFF_TONUM;

    return distance;
}


/* Method called to calculate speed based on travelled distance. */
float AppActivityScreenPresenter::CalculateSpeed(float newDistance)
{
    static float lastDist[APP_SPEED_INTERVAL] = {0.f};
    static uint8_t idx = 0u;

    if(false == appInternalData.initFunc.Initialized_CalculateSpeed)
    {
        /* Local init */
        for(uint8_t i=0u; i < APP_SPEED_INTERVAL; i++)
        {
            lastDist[i] = 0.f;
        }
        idx = 0u;
        appInternalData.initFunc.Initialized_CalculateSpeed = true;
    }

    lastDist[idx] = newDistance;
    idx = (idx >= (APP_SPEED_INTERVAL - 1u)) ? 0u : (idx + 1u);

    float speed = MeanFromArray(lastDist, APP_SPEED_INTERVAL);
    speed *= APP_SEC_IN_HR;

    return speed;
}


/* Method called to calculate total distance and speed. */
void AppActivityScreenPresenter::CalculateSpeedAndDistance(void)
{
    static float lastLat = 0.f;
    static float lastLon = 0.f;

    if(APP_DISTANCE_MIN_POINTS_NUM > trackFileInfo.points)
    {
        /* Do not proceed on first entry when there is no enough points */
    }
    else
    {
        float distance = CalculateDistance(gpsSignals.latitude, gpsSignals.longitude, lastLat, lastLon);
        distance = (distance < appInternalData.maxDistancePerSecond) ? distance : appInternalData.maxDistancePerSecond;
        float speed = CalculateSpeed(distance);

        activityData.distance += distance;
        activityData.speed = speed;
        activityData.avgSpeed = (float)(activityData.distance / (activityData.timer / APP_SEC_IN_HR / APP_TIMER_COEFF_TOSEC));
        activityData.maxSpeed = (activityData.speed > activityData.maxSpeed) ? activityData.speed : activityData.maxSpeed;
    }

    if(APP_SCREEN_MAIN == appInternalData.screen)
    {
        view.NotifySignalChanged_activityData_distance(activityData.distance);
        view.NotifySignalChanged_activityData_speed(activityData.speed);
        view.NotifySignalChanged_activityData_avgSpeed(activityData.avgSpeed);
        view.NotifySignalChanged_activityData_maxSpeed(activityData.maxSpeed);
    }

    lastLat = gpsSignals.latitude;
    lastLon = gpsSignals.longitude;
}


/* Method called to calculate all parameters related to altitude. */
void AppActivityScreenPresenter::CalculateAltitude(void)
{
    static uint8_t idx = 0u;
    static float   lastDist[APP_ALTI_INTERVAL] = {0.f};
    static float   lastAlti[APP_ALTI_INTERVAL] = {0.f};
    static float   lastDistance = activityData.distance;
    static float   lastAltitude = *altitudeInfo.ptrValue;

    if(false == appInternalData.initFunc.Initialized_CalculateAltitude)
    {
        /* Local init */
        for(uint8_t i=0u; i < APP_ALTI_INTERVAL; i++)
        {
            lastDist[i] = 0.f;
            lastAlti[i] = *altitudeInfo.ptrValue;
        }
        lastDistance = activityData.distance;
        lastAltitude = *altitudeInfo.ptrValue;
        idx = 0u;
        appInternalData.initFunc.Initialized_CalculateAltitude = true;
    }

    /* Add new data to cyclic buffer */
    lastDist[idx] = activityData.distance;
    lastAlti[idx] = *altitudeInfo.ptrValue;

    //float currDistance = MedianFromArray<float>(lastDist, APP_ALTI_INTERVAL);
    //float currAltitude = MedianFromArray<float>(lastAlti, APP_ALTI_INTERVAL);

    float currDistance = MeanFromArray(lastDist, APP_ALTI_INTERVAL);
    float currAltitude = MeanFromArray(lastAlti, APP_ALTI_INTERVAL);

    float distDiff = currDistance - lastDistance;
    float altiDiff = currAltitude - lastAltitude;

    if(altiDiff >= 0)
    {
        activityData.altiUp += altiDiff;
    }
    else
    {
        /* Subtraction of negative value gives positive number */
        activityData.altiDown -= altiDiff;
    }

    if( (distDiff > 0.f) && (distDiff <= APP_ALTI_DIST_THRESHOLD) )
    {
        float slope = ((float)altiDiff / APP_SLOPE_MTOKM) / distDiff * APP_SLOPE_100PERCENT;
        if(appInternalData.maxSlopePerSecond > slope)
        {
            activityData.slope = slope;
        }
    }

    activityData.altitude = currAltitude;
    activityData.altiMax = (activityData.altitude > activityData.altiMax) ? activityData.altitude : activityData.altiMax;
    activityData.slopeMax = (activityData.slope > activityData.slopeMax) ? activityData.slope : activityData.slopeMax;

    if(APP_SCREEN_ALTI == appInternalData.screen)
    {
        view.NotifySignalChanged_activityData_altitude(activityData.altitude);
        view.NotifySignalChanged_activityData_slope(activityData.slope);
        view.NotifySignalChanged_activityData_altiUp((int32_t)activityData.altiUp);
        view.NotifySignalChanged_activityData_altiDown((int32_t)activityData.altiDown);
        view.NotifySignalChanged_activityData_altiMax(activityData.altiMax);
        view.NotifySignalChanged_activityData_slopeMax(activityData.slopeMax);
    }

    lastDistance = currDistance;
    lastAltitude = currAltitude;
    idx = (idx >= (APP_ALTI_INTERVAL - 1u)) ? 0u : (idx + 1u);
}


/* Method called to sort array and return median value. */
template <typename T>
T AppActivityScreenPresenter::MedianFromArray(T* array, const uint8_t size)
{
    T median = 0;
    T cpyArr[size] = {0};
    
    for(uint8_t i = 0u; i < size; i++)
    {
        cpyArr[i] = array[i];
    }

    /* Sort array */
    for(uint8_t i = 0u; i < (size - 1u); i++)
    {
        for(uint8_t j = 0u; j < (size - i- 1u); j++)
        {
            if(cpyArr[j] > cpyArr[j + 1u])
            {
                T tmp = cpyArr[j];
                cpyArr[j] = cpyArr[j + 1u];
                cpyArr[j + 1u] = tmp;
            }
        }
    }

    if((size % APP_ALTI_EVEN) == 0u)
    {
        uint8_t halfUp = size / APP_ALTI_EVEN;
        uint8_t halfDown = halfUp - 1u;
        median = (cpyArr[halfUp] + cpyArr[halfDown]) / APP_ALTI_EVEN;
    }
    else
    {
        uint8_t half = (uint8_t)(size / APP_ALTI_EVEN);
        median = cpyArr[half];
    }

    return median;
}


/* Method called to sort array and return median value. */
float AppActivityScreenPresenter::MeanFromArray(float* array, const uint8_t size)
{
    float mean = 0.f;

    for(uint8_t i=0u; i < size; i++)
    {
        mean += array[i];
    }
    mean /= size;

    return mean;
}


/* Method called to increment activity timer. Signal is synchronized
   with global interrupt timer tim_t_100ms. */
void AppActivityScreenPresenter::IncrementTimer(void)
{
    uint32_t newTime = DC_get_main_tim_t_100ms();

    activityData.timer += newTime - appInternalData.lastTime;
    appInternalData.lastTime = newTime;

    if(APP_SCREEN_MAIN == appInternalData.screen)
    {
        view.NotifySignalChanged_activityData_timer(activityData.timer);
    }
}


/* Method called to consider time zone during time and date
   calculations. */
void AppActivityScreenPresenter::ApplyTimezone(void)
{
    AppActivity_timezoneDateChange_T dateChange = APP_TIMEZONE_NOCHANGE_DAY;
    int8_t newTimeHr = (int8_t)gpsSignals.timeHr + appInternalData.timezone;

    if((APP_TIMEZONE_HR_MIN <= newTimeHr) && (newTimeHr <= APP_TIMEZONE_HR_MAX))
    {
        /* Nothing to change, time is between 0 and 23 */
        dateChange = APP_TIMEZONE_NOCHANGE_DAY;
    }
    else if(APP_TIMEZONE_HR_MIN > newTimeHr)
    {
        newTimeHr += APP_TIMEZONE_HR_FULLDAY;
        dateChange = APP_TIMEZONE_DECREMENT_DAY;
    }
    else /* newTimeHr > APP_TIMEZONE_HR_MAX */
    {
        newTimeHr -= APP_TIMEZONE_HR_FULLDAY;
        dateChange = APP_TIMEZONE_INCREMENT_DAY;
    }

    calendar.time.hr = (uint8_t)newTimeHr;
    calendar.time.min = gpsSignals.timeMin;
    calendar.time.sec = gpsSignals.timeSec;

    ApplyDate(dateChange);
}


/* Method called to set new date according to current timezone
   and date from gps. */
void AppActivityScreenPresenter::ApplyDate(AppActivity_timezoneDateChange_T dateChange)
{
    uint8_t day = gpsSignals.dateDay;
    uint8_t mon = gpsSignals.dateMon;
    uint8_t year = gpsSignals.dateYear;
    bool isLeapYear = IsLeapYear(year);
    uint8_t leapOffset = 0u;

    switch (dateChange)
    {
        case APP_TIMEZONE_NOCHANGE_DAY:
            break;

        case APP_TIMEZONE_DECREMENT_DAY:
            if(gpsSignals.dateDay <= APP_TIMEZONE_FIRST_DAY)            /* Day is the first day of month, month needs to be decremented too */
            {
                if(gpsSignals.dateMon <= APP_TIMEZONE_MONTH_JANUARY)    /* Month is the first month of year, year needs to be decremented too */
                {
                    mon = APP_TIMEZONE_MONTH_DECEMBER;
                    year = (year != APP_TIMEZONE_INIT_YEAR) ? (year - 1u) : APP_TIMEZONE_INIT_YEAR;
                    isLeapYear = IsLeapYear(year);
                }
                else    /* Month is not the first month in year */
                {
                    mon--;
                }
                leapOffset = ((true == isLeapYear) && (APP_TIMEZONE_MONTH_FEBRUARY == mon)) ? APP_TIMEZONE_LEAPYEAR_FEB_ADD : 0u;
                day = daysInMon[mon-1u] + leapOffset;
            }
            else    /* Day is not the first day */
            {
                day--;
            }
            break;

        case APP_TIMEZONE_INCREMENT_DAY:
            leapOffset = ((true == isLeapYear) && (APP_TIMEZONE_MONTH_FEBRUARY == mon)) ? 1u : 0u;
            if(gpsSignals.dateDay >= (daysInMon[mon-1u] + leapOffset))     /* Day is the last day of month, month needs to be incremented too */
            {
                day = APP_TIMEZONE_FIRST_DAY;
                if(gpsSignals.dateMon >= APP_TIMEZONE_MONTH_DECEMBER)   /* Month is the last month of year, year needs to be incremented too */
                {
                    mon = APP_TIMEZONE_MONTH_JANUARY;
                    year = (year != APP_TIMEZONE_MAX_YEAR) ? (year + 1u) : APP_TIMEZONE_MAX_YEAR;
                }
                else    /* Month is not the last month in year */
                {
                    mon++;
                }
            }
            else    /* Day is not the last day */
            {
                day++;
            }
            break;

        default:
            break;
    }

    calendar.date.year = year;
    calendar.date.mon = mon;
    calendar.date.day = day;
}


/* Method called to update activity time (clock). */
void AppActivityScreenPresenter::UpdateTime(void)
{
    activityData.time = calendar.time.hr * APP_TIME_COEFF2 + calendar.time.min * APP_TIME_COEFF1 + calendar.time.sec;

    if(APP_SCREEN_MAIN == appInternalData.screen)
    {
        view.NotifySignalChanged_activityData_time(activityData.time);
    }
}


/* Method called to update altitude when activity is not running. */
void AppActivityScreenPresenter::UpdateAltitude(void)
{
    if(APP_STATE_RUNNING != appInternalData.state)
    {
        if((APP_SCREEN_ALTI == appInternalData.screen) || (APP_STATE_INIT == appInternalData.state))
        {
            view.NotifySignalChanged_sensorData_altitude(*altitudeInfo.ptrValue);
        }
    }
}


/* Method called to draw route: map or track depending
   on input arguments. */
void AppActivityScreenPresenter::DrawRoute(AppActivity_drawRoute_T route)
{
    if((APP_SCREEN_SELECTMAP == appInternalData.screen) || (APP_SCREEN_MAP == appInternalData.screen))
    {
        AppActivity_drawRouteData_T drawRouteData = {0u};

        switch(route)
        {
            case APP_DRAWROUTE_MAP:
                drawRouteData.route = APP_DRAWROUTE_MAP;
                drawRouteData.startIdx = APP_ROUTE_MAP_FIRST_ELEMENT;
                drawRouteData.endIdx = routePointsData.idxMap;
                drawRouteData.idxRoute = routePointsData.idxMap;
                drawRouteData.fileInfoPoints = mapFileInfo.points;
                drawRouteData.pointsLimit = APP_ROUTE_MAP_POINTS_LIMIT;
                drawRouteData.skip = &routeData.map.skip;
                drawRouteData.addedPoints = &routeData.map.addedPoints;
                view.FlushRouteList();
                break;
            case APP_DRAWROUTE_TRACK:
                drawRouteData.route = APP_DRAWROUTE_TRACK;
                drawRouteData.startIdx = routePointsData.idxTrack;
                drawRouteData.endIdx = 0u;
                drawRouteData.idxRoute = routePointsData.idxTrack;
                drawRouteData.fileInfoPoints = trackFileInfo.points;
                drawRouteData.pointsLimit = APP_ROUTE_MAP_ELEMENTS - routeData.map.addedPoints;
                drawRouteData.skip = &routeData.track.skip;
                drawRouteData.addedPoints = &routeData.track.addedPoints;
                break;
            default:
                break;
        }

        *drawRouteData.skip = 0u;
        uint32_t scaleVal = GetScaleValue();
        view.SetRouteScale(scaleVal);

        MapCenterCoordinates();

        SetArrowLocation();
        SetArrowAngle();

        SelectRoutePoints(route, &drawRouteData, APP_ROUTE_DONT_DRAW_PATH);
        SelectRoutePoints(route, &drawRouteData, APP_ROUTE_DRAW_PATH);

        view.TrackRedraw();

#if 0
        /* Loop for selecting best skip value */
        while(startCondition != true)
        {
            for (uint16_t idx = drawRouteData.startIdx; ((idx > drawRouteData.endIdx) && (drawRouteData.pointsLimit > addedPoints)) ; idx--)
            {
                bool coordsInView = CoordsInView(routePointsData.coords[idx]);

                if(true == coordsInView)
                {
                    AppActivity_coordinatesXY_T coordsXY = MapGPSCoordsToXY(routePointsData.coords[idx]);
                    bool newPoint = view.AddCoordsToRouteList(coordsXY, route, false);

                    if(true == newPoint)
                    {
                        addedPoints++;
                    }
                }

                /* Set index to 0 if array was overflowed */
                if((APP_ROUTE_SKIP_BOTTOM_LIMIT >= idx) && (true == routePointsData.overflow) && (true == firstCycle))
                {
                    idx = routePointsData.idxTrack - APP_ROUTE_MAP_OFFSET_FOR_TRACK;
                    firstCycle = false;
                }
                /* Skip some points depending on selected scale */
                else if(true == CanRouteBeSkipped(route, idx, *drawRouteData.skip))
                {
                    idx -= *drawRouteData.skip;
                }
            }

            if(addedPoints >= drawRouteData.pointsLimit)
            {
                startCondition = false;
                *drawRouteData.skip += 1u;
            }
            else
            {
                startCondition = true;
            }
            addedPoints = 0u;
        }


        /* Loop for adding points draw list */
        for (uint16_t idx = drawRouteData.startIdx; ((idx > drawRouteData.endIdx) && (drawRouteData.pointsLimit > addedPoints)) ; idx--)
        {
            bool coordsInView = CoordsInView(routePointsData.coords[idx]);

            if(true == coordsInView)
            {
                AppActivity_coordinatesXY_T coordsXY = MapGPSCoordsToXY(routePointsData.coords[idx]);
                bool newPoint = view.AddCoordsToRouteList(coordsXY, route, true);

                if(true == newPoint)
                {
                    addedPoints++;
                }
            }

            /* Set index to 0 if array was overflowed */
            if((APP_ROUTE_SKIP_BOTTOM_LIMIT >= idx) && (true == routePointsData.overflow) && (true == firstCycle))
            {
                idx = routePointsData.idxTrack - APP_ROUTE_MAP_OFFSET_FOR_TRACK;
                firstCycle = false;
            }
            /* Skip some points depending on selected scale */
            else if(true == CanRouteBeSkipped(route, idx, *drawRouteData.skip))
            {
                idx -= *drawRouteData.skip;
            }
        }
#endif
    }
}


/* Function called to iterate through route list and pass proper coordinates
   to view. Path will be drawn only when draw flag is set to true. It can be used
   to select best skip value before drawing on the screen. */
void AppActivityScreenPresenter::SelectRoutePoints(AppActivity_drawRoute_T route, AppActivity_drawRouteData_T *drawRouteData, bool draw)
{
    uint8_t retVal = RET_NOK;
    uint8_t addedPoints = 0u;
    bool firstCycle = true;             //TODO: what is the purpose of this?

    while(retVal != RET_OK)
    {
        for (uint16_t idx = drawRouteData->startIdx; ((idx > drawRouteData->endIdx) && (drawRouteData->pointsLimit > addedPoints)) ; idx--)
        {
            bool coordsInView = CoordsInView(routePointsData.coords[idx]);

            if(true == coordsInView)
            {
                AppActivity_coordinatesXY_T coordsXY = MapGPSCoordsToXY(routePointsData.coords[idx]);
                bool newPoint = view.AddCoordsToRouteList(coordsXY, route, draw);

                if(true == newPoint)
                {
                    addedPoints++;
                }
            }

            /* Set index to 0 if array was overflowed */
            if((APP_ROUTE_SKIP_BOTTOM_LIMIT >= idx) && (true == routePointsData.overflow) && (true == firstCycle))
            {
                idx = routePointsData.idxTrack - APP_ROUTE_MAP_OFFSET_FOR_TRACK;
                firstCycle = false;
            }
            /* Skip some points depending on selected scale */
            else if(true == CanRouteBeSkipped(route, idx, *drawRouteData->skip))
            {
                idx -= *drawRouteData->skip;
            }
        }

        if( (false == draw) && (addedPoints >= drawRouteData->pointsLimit) )
        {
            retVal = RET_NOK;
            *drawRouteData->skip += 1u;
            addedPoints = 0u;
        }
        else
        {
            retVal = RET_OK;
            *drawRouteData->addedPoints = addedPoints;
        }
    }
}


/* Method called to safely skip coordinates in route list
   to not read outside the range. */
bool AppActivityScreenPresenter::CanRouteBeSkipped(AppActivity_drawRoute_T route, uint16_t idx, uint8_t skip)
{
    bool retVal = false;

    if(route == APP_DRAWROUTE_MAP)
    {
        if((idx - skip) > routePointsData.idxMap)
        {
            retVal = true;
        }
    }
    else if(route == APP_DRAWROUTE_TRACK)
    {
        if((idx > skip) && ((uint16_t)(idx - skip) > APP_ROUTE_SKIP_BOTTOM_LIMIT))
        {
            retVal = true;
        }
    }
    else
    {
        /* Should not be here */
        retVal = false;
    }

    return retVal;
}


/* NOT USED: Function called to calculate number of coords to skip during
   selecting points to draw. */
void AppActivityScreenPresenter::CalculateSkippedCoords(AppActivity_drawRoute_T route)
{
    uint16_t fileInfoPoints = 0u;
    uint8_t addedPoints = 0u;
    uint8_t *skip = NULL;

    switch(route)
    {
        case APP_DRAWROUTE_MAP:
            fileInfoPoints = mapFileInfo.points;
            addedPoints = routeData.map.addedPoints;
            skip = &routeData.map.skip;
            break;
        case APP_DRAWROUTE_TRACK:
            fileInfoPoints = trackFileInfo.points;
            addedPoints = routeData.track.addedPoints;
            skip = &routeData.track.skip;
            break;
        default:
            break;
    }

    if(APP_ROUTE_MAP_ELEMENTS > fileInfoPoints)   /* Do not skip when there is less points than maximum possible to draw */
    {
        *skip = 0u;
    }
    else /* Calculate skip value based on previous results */
    {
        /* Small amount of points */
        if (addedPoints < APP_ROUTE_SKIP_DRAWN_POINTS_BOTTOM_LIMIT)
        {
            if(*skip == 0u)
            {
                /* Select default skip value */
                switch(routeData.scale)
                {
                    case APP_ROUTE_SCALE50:
                        *skip = APP_ROUTE_SKIPPED_COORDS_50;
                        break;
                    case APP_ROUTE_SCALE100:
                        *skip = APP_ROUTE_SKIPPED_COORDS_100;
                        break;
                    case APP_ROUTE_SCALE500:
                        *skip = APP_ROUTE_SKIPPED_COORDS_500;
                        break;
                    case APP_ROUTE_SCALE1000:
                        *skip = APP_ROUTE_SKIPPED_COORDS_1000;
                        break;
                    case APP_ROUTE_SCALE5000:
                        *skip = APP_ROUTE_SKIPPED_COORDS_5000;
                        break;
                    case APP_ROUTE_SCALE10000:
                        *skip = APP_ROUTE_SKIPPED_COORDS_10000;
                        break;
                    case APP_ROUTE_SCALEFULL:
                        *skip = (uint8_t)(fileInfoPoints / APP_ROUTE_MAP_ELEMENTS);
                        break;
                    default:
                        *skip = 0u;
                        break;
                }
            }
            else
            {
                *skip -= 1u;
            }
            *skip = (*skip != 0u) ? (*skip - 1u) : 0u;      //TODO check this
        }
        /* Enough points */
        else if((addedPoints >= APP_ROUTE_SKIP_DRAWN_POINTS_BOTTOM_LIMIT) && (addedPoints < APP_ROUTE_SKIP_DRAWN_POINTS_UPPER_LIMIT))
        {

        }
        /* Too much points */
        else
        {
            *skip = (*skip != APP_ROUTE_SKIP_MAX_VALUE) ? (*skip + 1u) : 0u;
        }
    }
}


/* Function called to find gps coordinates
   from file buffer. */
AppActivity_coordinatesGPS_T AppActivityScreenPresenter::GetCoordsGPSFromBuffer(uint8_t* buffer, uint8_t size, bool &foundCoords)
{
    uint8_t floatBuff[APP_ROUTE_FLOATBUFF_SIZE] = {0u};
    AppActivity_coordinatesGPS_T coords = {0.f};
    const char *latChar = "<trkpt lat=\"";
    const char *lonChar = "\" lon=\"";
    const char *endChar = "\">";

    char *startLatPtr = strstr((char*)buffer, latChar) + strlen(latChar);
    char *endLatPtr = strstr((char*)buffer, lonChar);
    size_t latLen = endLatPtr - startLatPtr;

    char *startLonPtr = strstr((char*)buffer, lonChar) + strlen(lonChar);
    char *endLonPtr = strstr((char*)buffer, endChar);
    size_t lonLen = endLonPtr - startLonPtr;

    if((NULL != startLatPtr) && (NULL != endLatPtr) && (NULL != startLonPtr) && (NULL != endLonPtr))
    {
        memcpy(floatBuff, startLatPtr, latLen);
        coords.lat = strtof((const char*)floatBuff, NULL);

        memset(floatBuff, 0u, APP_ROUTE_FLOATBUFF_SIZE);
        memcpy(floatBuff, startLonPtr, lonLen);
        coords.lon = strtof((const char*)floatBuff, NULL);

        foundCoords = true;
    }
    else
    {
        foundCoords = false;
    }

    return coords;
}


/* Method called to check whether coordinates are in the visible
   area on a display. */
bool AppActivityScreenPresenter::CoordsInView(AppActivity_coordinatesGPS_T coords)
{
    bool isInView = false;

    if((coords.lat < routeData.mapCoordsGPS.upLeft.lat)      && (coords.lon > routeData.mapCoordsGPS.upLeft.lon) &&
       (coords.lat < routeData.mapCoordsGPS.upRight.lat)     && (coords.lon < routeData.mapCoordsGPS.upRight.lon) &&
       (coords.lat > routeData.mapCoordsGPS.bottomLeft.lat)  && (coords.lon > routeData.mapCoordsGPS.bottomLeft.lon) &&
       (coords.lat > routeData.mapCoordsGPS.bottomRight.lat) && (coords.lon < routeData.mapCoordsGPS.bottomRight.lon))
    {
        isInView = true;
    }
    else if(APP_ROUTE_SCALEFULL == routeData.scale)
    {
        isInView = true;
    }
    else
    {
        isInView = false;
    }

    return isInView;
}


/* Method called to find latitude and longitude to corresponding
   corners of a display. */
void AppActivityScreenPresenter::MapCenterCoordinates(void)
{
    float scaleDistCoeff = MapScaleToDistance();

    if(APP_ROUTE_SCALEFULL == routeData.scale)
    {
        /* Set center from max coords */
        routeData.mapCoordsGPS.center.lat = routeData.maxCoordsGPS.center.lat;
        routeData.mapCoordsGPS.center.lon = routeData.maxCoordsGPS.center.lon;
    }
    else
    {
        if(true == appInternalData.wasFix)
        {
            /* Gps signal was fixed, set center from last gps value */
            routeData.mapCoordsGPS.center.lat = gpsSignals.latitude;
            routeData.mapCoordsGPS.center.lon = gpsSignals.longitude;
        }
        else
        {
            if(APP_ROUTE_MAP_FIRST_ELEMENT != routePointsData.idxMap)
            {
                /* Map is present, set center from the first coords from map */
                routeData.mapCoordsGPS.center.lat = routePoints[APP_ROUTE_MAP_FIRST_ELEMENT].lat;
                routeData.mapCoordsGPS.center.lon = routePoints[APP_ROUTE_MAP_FIRST_ELEMENT].lon;
            }
            else
            {
                /* No map, no fix before, there should be zeros */
                routeData.mapCoordsGPS.center.lat = routeData.maxCoordsGPS.center.lat;
                routeData.mapCoordsGPS.center.lon = routeData.maxCoordsGPS.center.lon;
            }
        }
    }

    routeData.mapCoordsGPS.upLeft = MapXYCoordsToGPS(routeData.mapCoordsXY->upLeft, scaleDistCoeff);
    routeData.mapCoordsGPS.upRight = MapXYCoordsToGPS(routeData.mapCoordsXY->upRight, scaleDistCoeff);
    routeData.mapCoordsGPS.bottomLeft = MapXYCoordsToGPS(routeData.mapCoordsXY->bottomLeft, scaleDistCoeff);
    routeData.mapCoordsGPS.bottomRight = MapXYCoordsToGPS(routeData.mapCoordsXY->bottomRight, scaleDistCoeff);
}


/* Function called to get scale value from enum indicator. */
uint32_t AppActivityScreenPresenter::GetScaleValue(void)
{
    uint32_t scaleVal = 0u;

    switch (routeData.scale)
    {
        case APP_ROUTE_SCALE50:
            scaleVal = APP_ROUTE_SCALE_50;
            break;
        case APP_ROUTE_SCALE100:
            scaleVal = APP_ROUTE_SCALE_100;
            break;
        case APP_ROUTE_SCALE500:
            scaleVal = APP_ROUTE_SCALE_500;
            break;
        case APP_ROUTE_SCALE1000:
            scaleVal = APP_ROUTE_SCALE_1000;
            break;
        case APP_ROUTE_SCALE5000:
            scaleVal = APP_ROUTE_SCALE_5000;
            break;
        case APP_ROUTE_SCALE10000:
            scaleVal = APP_ROUTE_SCALE_10000;
            break;
        case APP_ROUTE_SCALEFULL:
            scaleVal = CalculateFullScale();
            break;
        default:
            scaleVal = APP_ROUTE_SCALE_ERROR;
            break;
    }

    return scaleVal;
}


/* Method called to calculate scale value for full
   track view. */
uint32_t AppActivityScreenPresenter::CalculateFullScale(void)
{
    uint32_t scaleVal = 0.f;
    float maxDist = 0.f;

    /* Calculate maximum width and heigth of track in meters */
    float maxHeigth = CalculateDistance(routeData.maxCoordsGPS.N.lat, routeData.maxCoordsGPS.N.lon, routeData.maxCoordsGPS.S.lat, routeData.maxCoordsGPS.S.lon);
    float maxWidth  = CalculateDistance(routeData.maxCoordsGPS.E.lat, routeData.maxCoordsGPS.E.lon, routeData.maxCoordsGPS.W.lat, routeData.maxCoordsGPS.W.lon);

    maxDist = (maxHeigth > maxWidth) ? maxHeigth : maxWidth;

    if(maxDist > 0.f)
    {
        scaleVal = (uint32_t)(maxDist * (float)APP_ROUTE_SCALE_COEFF_M_IN_KM / APP_ROUTE_SCALE_FULL_COEFFVIEW);
    }
    else
    {
        scaleVal = APP_ROUTE_SCALE_MIN;
    }

    return scaleVal;
}


/* Method called to map enum scale to corresponding value
   in kilometers. */
float AppActivityScreenPresenter::MapScaleToDistance(void)
{
    float scaleDist = (float)GetScaleValue();

    /* Scale to 1px per x km */
    scaleDist /= (float)(APP_ROUTE_SCALE_WIDTH_PX * APP_ROUTE_SCALE_COEFF_M_IN_KM);

    return scaleDist;
}


/* Function called to map given XY coordinates to GPS ones. */
AppActivity_coordinatesGPS_T AppActivityScreenPresenter::MapXYCoordsToGPS(AppActivity_coordinatesXY_T coords, float scaleCoeff)
{
    AppActivity_coordinatesGPS_T coordsGPS = {0.f};

    float distX = ((routeData.mapCoordsXY->center.X - coords.X) * scaleCoeff) / (APP_DISTANCE_COEFF_TONUM * cosf(routeData.mapCoordsGPS.center.lat * APP_DISTANCE_COEFF_TORAD));
    float distY = ((routeData.mapCoordsXY->center.Y - coords.Y) * scaleCoeff) / (APP_DISTANCE_COEFF_TONUM);

    coordsGPS.lat = routeData.mapCoordsGPS.center.lat + distY;
    coordsGPS.lon = routeData.mapCoordsGPS.center.lon - distX;

    return coordsGPS;
}


/* Function called to map given GPS coordinates to XY ones. */
AppActivity_coordinatesXY_T AppActivityScreenPresenter::MapGPSCoordsToXY(AppActivity_coordinatesGPS_T coords)
{
    AppActivity_coordinatesXY_T coordsXY = {0u};

    coordsXY.X = (uint8_t)MapPointToLinearFunction(routeData.mapCoordsGPS.upLeft.lon,  routeData.mapCoordsXY->upLeft.X,
                                                   routeData.mapCoordsGPS.upRight.lon, routeData.mapCoordsXY->upRight.X,
                                                   coords.lon);

    coordsXY.Y = (uint8_t)MapPointToLinearFunction(routeData.mapCoordsGPS.upLeft.lat,     routeData.mapCoordsXY->upLeft.Y,
                                                   routeData.mapCoordsGPS.bottomLeft.lat, routeData.mapCoordsXY->bottomLeft.Y,
                                                   coords.lat);

    return coordsXY;
}


/* Method called to map point to linear function. 
   To given X corresponding Y will be returned. */
float AppActivityScreenPresenter::MapPointToLinearFunction(float x1, float y1, float x2, float y2, float X)
{
    float a = 0.f;
    float b = 0.f;
    float Y = 0.f;

    if(x2 != x1)
    {
        a = (y2-y1)/(x2-x1);
        b = (y1*x2 - x1*y2)/(x2-x1);
        Y = a*X + b;

        /* Check range */
        if((Y < APP_MAP_LINEAR_F_BOTTOM_LIMIT) || (Y > APP_MAP_LINEAR_F_UPPER_LIMIT))
        {
            Y = 0.f;
        }
    }

    return Y;
}


/* Method called to change scale on map to zoom in. */
void AppActivityScreenPresenter::ZoomIn(void)
{
    if(routeData.scale > APP_ROUTE_SCALE50)
    {
        routeData.scale = (AppActivity_routeScale_T)(routeData.scale - 1u);
        DrawRoute(APP_DRAWROUTE_MAP);
        DrawRoute(APP_DRAWROUTE_TRACK);
    }
    else
    {
        routeData.scale = APP_ROUTE_SCALE50;
    }
}


/* Method called to change scale on map to zoom out. */
void AppActivityScreenPresenter::ZoomOut(void)
{
    if((routeData.scale + 1u) < APP_ROUTE_MAX_SCALE)
    {
        routeData.scale = (AppActivity_routeScale_T)(routeData.scale + 1u);
        DrawRoute(APP_DRAWROUTE_MAP);
        DrawRoute(APP_DRAWROUTE_TRACK);
    }
    else
    {
        routeData.scale = (AppActivity_routeScale_T)(APP_ROUTE_MAX_SCALE - 1u);
    }
}


/* Method called to calculate a number of bytes to move track file pointer. */
uint32_t AppActivityScreenPresenter::CalculateFileOffset(uint16_t points)
{
    uint32_t offset = APP_ROUTE_FILE_HEADEROFFSET;

    if(points > APP_ROUTE_FILE_READLINES)
    {
        offset += (points - APP_ROUTE_FILE_READLINES) * APP_ROUTE_FILE_DATAOFFSET;
    }

    return offset;
}


/* Method called to save new coords to track list safely. */
void AppActivityScreenPresenter::SaveCoordsInCcmRam(float lat, float lon)
{
    if((routePointsData.idxTrack + 1u) < routePointsData.idxMap)
    {
        if((0u == routePointsData.idxTrack) && (0.f == routePointsData.coords[0u].lat) && (0.f == routePointsData.coords[0u].lon))
        {
            routePointsData.coords[0u].lat = lat;
            routePointsData.coords[0u].lon = lon;
        }
        else
        {
            routePointsData.idxTrack++;
            routePointsData.coords[routePointsData.idxTrack].lat = lat;
            routePointsData.coords[routePointsData.idxTrack].lon = lon;
        }
    }
    else
    {
        routePointsData.overflow = true;
        routePointsData.idxTrack = 0u;
        routePointsData.coords[0u].lat = 0.f;
        routePointsData.coords[0u].lon = 0.f;
    }
}


/* Method called to find extreme locations. */
void AppActivityScreenPresenter::FindMaxCoords(float lat, float lon)
{
    bool calcNewCenter = false;

    if((0.f == routeData.maxCoordsGPS.N.lat) && (0.f == routeData.maxCoordsGPS.E.lon) && 
       (0.f == routeData.maxCoordsGPS.W.lon) && (0.f == routeData.maxCoordsGPS.S.lat))
    {
        /* Assign current coords to all maxs during first call */
        routeData.maxCoordsGPS.N.lat = lat;
        routeData.maxCoordsGPS.N.lon = lon;
        routeData.maxCoordsGPS.E.lat = lat;
        routeData.maxCoordsGPS.E.lon = lon;
        routeData.maxCoordsGPS.W.lat = lat;
        routeData.maxCoordsGPS.W.lon = lon;
        routeData.maxCoordsGPS.S.lat = lat;
        routeData.maxCoordsGPS.S.lon = lon;
        calcNewCenter = true;
    }
    else
    {
        if(lat > routeData.maxCoordsGPS.N.lat)
        {
            routeData.maxCoordsGPS.N.lat = lat;
            routeData.maxCoordsGPS.N.lon = lon;
            calcNewCenter = true;
        }
        else if(lon > routeData.maxCoordsGPS.E.lon)
        {
            routeData.maxCoordsGPS.E.lat = lat;
            routeData.maxCoordsGPS.E.lon = lon;
            calcNewCenter = true;
        }
        else if(lon < routeData.maxCoordsGPS.W.lon)
        {
            routeData.maxCoordsGPS.W.lat = lat;
            routeData.maxCoordsGPS.W.lon = lon;
            calcNewCenter = true;
        }
        else if(lat < routeData.maxCoordsGPS.S.lat)
        {
            routeData.maxCoordsGPS.S.lat = lat;
            routeData.maxCoordsGPS.S.lon = lon;
            calcNewCenter = true;
        }
    }

    if(true == calcNewCenter)
    {
        routeData.maxCoordsGPS.center.lat = (routeData.maxCoordsGPS.N.lat + routeData.maxCoordsGPS.S.lat) / APP_ROUTE_MEANTWO_COEEF;
        routeData.maxCoordsGPS.center.lon = (routeData.maxCoordsGPS.E.lon + routeData.maxCoordsGPS.W.lon) / APP_ROUTE_MEANTWO_COEEF;
    }
}


/* Method called to clear extreme locations. */
void AppActivityScreenPresenter::ClearMaxCoords(void)
{
    routeData.maxCoordsGPS.N.lat = 0.f;
    routeData.maxCoordsGPS.N.lon = 0.f;
    routeData.maxCoordsGPS.E.lat = 0.f;
    routeData.maxCoordsGPS.E.lon = 0.f;
    routeData.maxCoordsGPS.W.lat = 0.f;
    routeData.maxCoordsGPS.W.lon = 0.f;
    routeData.maxCoordsGPS.S.lat = 0.f;
    routeData.maxCoordsGPS.S.lon = 0.f;
}


/* Method called to filter out wrong coordinates
   by selecting median from set. */
void AppActivityScreenPresenter::FilterCoords(void)
{
    static float lat[APP_COORDS_FILTER_ARR_SIZE] = {0.f};
    static float lon[APP_COORDS_FILTER_ARR_SIZE] = {0.f};
    static uint8_t idx = 0u;

    if(false == appInternalData.initFunc.Initialized_FilterCoords)
    {
        /* Local init */
        for(uint8_t i = 0u; i < APP_COORDS_FILTER_ARR_SIZE; i++)
        {
            lat[i] = gpsSignals.latitude;
            lon[i] = gpsSignals.longitude;
        }
        idx = 0u;
        appInternalData.initFunc.Initialized_FilterCoords = true;
    }

    lat[idx] = gpsSignals.latitude;
    lon[idx] = gpsSignals.longitude;

    gpsSignals.latitude = MedianFromArray(lat, APP_COORDS_FILTER_ARR_SIZE);
    gpsSignals.longitude = MedianFromArray(lon, APP_COORDS_FILTER_ARR_SIZE);

    idx = (idx >= (APP_COORDS_FILTER_ARR_SIZE - 1u)) ? 0u : (idx + 1u);

}


/* Method called to close and reopen gpx file.
   This behaviour should prevent unexpected loss of data. */
void AppActivityScreenPresenter::SaveFile(void)
{
    uint8_t retVal = FS_CloseFile((FS_File_T**)&trackFileInfo.filePtr);
    trackFileInfo.fileStatus = (RET_OK == retVal) ? APP_FILE_CLOSED : APP_FILE_ERROR;

    retVal = FS_OpenFile((FS_File_T**)&trackFileInfo.filePtr, trackFileInfo.name, FS_MODEAPPEND);
    trackFileInfo.fileStatus = (RET_OK == retVal) ? APP_FILE_CREATED : APP_FILE_ERROR;
}


/* Method called to select altitude signal depending
   on sensor status. Switch between sensor and gps. */
void AppActivityScreenPresenter::SelectAltitudeSource(void)
{
    appInternalData.sensorEnabled = (bool)DC_get_appSettings_settingsData_value_sensors();

    if(true == appInternalData.sensorEnabled)
    {
        altitudeInfo.ptrValue = &sensorData.altitude;
        altitudeInfo.altiSrc = APP_ALTI_SENSORS;
    }
    else
    {
        altitudeInfo.ptrValue = &gpsSignals.altitude;
        altitudeInfo.altiSrc = APP_ALTI_GPS;
    }
}


/* Method called to check whether current year is leap year or not. */
bool AppActivityScreenPresenter::IsLeapYear(uint16_t year)
{
    bool isLeap = false;
    year += APP_TIMEZONE_FILL_THOUSANDS;

    if( (((year % APP_TIMEZONE_MOD_4) == 0u) && ((year % APP_TIMEZONE_MOD_100) != 0u)) || ((year % APP_TIMEZONE_MOD_400) == 0u) )
    {
        isLeap = true;
    }

    return isLeap;
}


/* Method called to open map file and get
   information about it. */
void AppActivityScreenPresenter::GetMapInfo(void)
{
    uint8_t buffer[APP_MAXFILEBUFFERSIZE] = {0u};
    uint8_t fileDisplayPath[APP_MAP_FILENAME_DISPLAY_LEN] = {APP_MAP_DEFAULT_LABEL};
    AppActivity_coordinatesGPS_T newCoordsGPS = {0.f};
    AppActivity_coordinatesGPS_T prevCoordsGPS = {0.f};
    float distance = 0.f;
    uint8_t retVal = RET_OK;
    bool isMoreLines = true;
    bool foundCoords = false;

    /* Clear map data */
    size_t len = sizeof(AppActivity_coordinatesGPS_T) * (APP_ROUTE_COMMONARRAY_LENGTH - routePointsData.idxMap);
    memset(&routePoints[routePointsData.idxMap], 0u, len);
    routePointsData.idxMap = APP_ROUTE_MAP_FIRST_ELEMENT;
    mapFileInfo.points = 0u;
    routeData.map.addedPoints = 0u;
    routeData.map.skip = 0u;
    ClearMaxCoords();

    if(mapIndex.current > APP_MAP_NOFILE)
    {
        retVal |= FS_FindInputFile(mapIndex.current, mapFileInfo.name, fileDisplayPath, APP_MAP_FILENAME_DISPLAY_LEN);
        retVal |= FS_OpenFile((FS_File_T**)&mapFileInfo.filePtr, mapFileInfo.name, FS_MODEREAD);
        mapFileInfo.fileStatus = (RET_OK == retVal) ? APP_FILE_OPENED : APP_FILE_ERROR;

        if(APP_FILE_OPENED == mapFileInfo.fileStatus)
        {
            while((true == isMoreLines) && (RET_OK == retVal))
            {
                retVal = FS_ReadFile((FS_File_T*)mapFileInfo.filePtr, (uint8_t*)buffer, APP_MAXFILEBUFFERSIZE, (boolean*)&isMoreLines);
                newCoordsGPS = GetCoordsGPSFromBuffer(buffer, APP_MAXFILEBUFFERSIZE, foundCoords);

                if(true == foundCoords)
                {
                    routePoints[routePointsData.idxMap] = newCoordsGPS;
                    routePointsData.idxMap = (routePointsData.idxMap > 0u) ? (routePointsData.idxMap - 1u) : 0u;
                    mapFileInfo.points++;

                    FindMaxCoords(newCoordsGPS.lat, newCoordsGPS.lon);

                    if(mapFileInfo.points > APP_DISTANCE_MIN_POINTS_NUM)
                    {
                        distance += CalculateDistance(newCoordsGPS.lat, newCoordsGPS.lon, prevCoordsGPS.lat, prevCoordsGPS.lon);
                    }

                    prevCoordsGPS = newCoordsGPS;
                }
            }
        }
        retVal |= FS_CloseFile((FS_File_T**)&mapFileInfo.filePtr);
        mapFileInfo.fileStatus = (RET_OK == retVal) ? APP_FILE_CLOSED : APP_FILE_ERROR;
    }

    DrawRoute(APP_DRAWROUTE_MAP);

    view.ChangeMapDescription(fileDisplayPath, APP_MAP_FILENAME_DISPLAY_LEN, distance);
}


/* Method called to show user previous map. */
void AppActivityScreenPresenter::DisplayPreviousMap(void)
{
    mapIndex.current = (mapIndex.current > 0u) ? (mapIndex.current - 1u) : mapIndex.max;
    GetMapInfo();
}

/* Method called to show user next map. */
void AppActivityScreenPresenter::DisplayNextMap(void)
{
    mapIndex.current = (mapIndex.current < mapIndex.max) ? (mapIndex.current + 1u) : 0u;
    GetMapInfo();
}


/* Method called to confirm selected map, it makes other
   widgets visible. */
void AppActivityScreenPresenter::ConfirmMapSelection(void)
{
    if(routeData.map.addedPoints > 0u)
    {
        routeData.scale = APP_ROUTE_SCALEFULL;
    }
    else
    {
        routeData.scale = APP_ROUTE_SCALE100;
    }

    appInternalData.screen = APP_SCREEN_MAIN;

    view.SetActivityDataScreen(appInternalData.screen);
    view.NotifySignalChanged_activityData_time(activityData.time);
}


/* Method called to set arrow position depending
   on current scale. */
void AppActivityScreenPresenter::SetArrowLocation(void)
{
    AppActivity_coordinatesGPS_T coordsGPS = { gpsSignals.latitude, gpsSignals.longitude };
    AppActivity_coordinatesXY_T coordsXY = MapGPSCoordsToXY(coordsGPS);
    view.SetArrowLocation(coordsXY);
}


/* Method called to calculate angle of an arrow
   on a screen to follow currect direction. */
void AppActivityScreenPresenter::SetArrowAngle(void)
{
    AppActivity_coordinatesGPS_T coordsMean = {0.f};
    AppActivity_coordinatesGPS_T coordsCurr = {0.f};
    float a = 0.f;
    float a_nom = 0.f;
    float a_denom = 0.f;
    float angle = 0.f;

    //TODO: add here calulcations where points is less than 3
    if((trackFileInfo.points > APP_MAP_ARROW_CALC_POINTS) && (routePointsData.idxTrack > APP_MAP_ARROW_CALC_POINTS))
    {
        /* Calculate mean from last points. */
        uint16_t currTrackIdx = routePointsData.idxTrack;
        for(uint8_t i = 0u; i < APP_MAP_ARROW_CALC_POINTS; i++)
        {
            coordsMean.lat += routePointsData.coords[currTrackIdx].lat;
            coordsMean.lon += routePointsData.coords[currTrackIdx].lon;
            currTrackIdx--;
        }
        coordsMean.lat /= APP_MAP_ARROW_CALC_POINTS;
        coordsMean.lon /= APP_MAP_ARROW_CALC_POINTS;

        /* Linear regression. */
        currTrackIdx = routePointsData.idxTrack;
        for(uint8_t i = 0u; i < APP_MAP_ARROW_CALC_POINTS; i++)
        {
            coordsCurr = routePointsData.coords[currTrackIdx];
            a_nom += (coordsCurr.lon - coordsMean.lon)*(coordsCurr.lat - coordsMean.lat);
            a_denom += POW2( coordsCurr.lon - coordsMean.lon );
            currTrackIdx--;
        }

        if(a_denom > 0.f)
        {
            a = a_nom / a_denom;
            angle = -tanf(a) + APP_MAP_ARROW_ANGLE_OFFSET_RAD;
        }
    }

    view.SetArrowAngle(angle);
}


/* Method called to collect signals from Model:
    - latitude
    - longitude
    - altitude
    - time
    - date
    - fixQuality
    - satellitesNum
    - lonDir
    - latDir
    - altitude
    - pressure
    - temperature
    - state of sd card */
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_latitude(float newLatitude)
{
    ConvertLatLon(newLatitude, gpsSignals.latitude);
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_longitude(float newLongitude)
{
    ConvertLatLon(newLongitude, gpsSignals.longitude);
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_altitude(float newAltitude)
{
    gpsSignals.altitude = newAltitude;
    UpdateAltitude();
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_time(uint32_t newTime)
{
    gpsSignals.timeSec  = newTime % APP_TIME_COEFF1;
    gpsSignals.timeMin = (newTime / APP_TIME_COEFF1) % APP_TIME_COEFF1;
    gpsSignals.timeHr = ((newTime / APP_TIME_COEFF2) % APP_TIME_COEFF1);
    ApplyTimezone();
    UpdateTime();
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_date(uint32_t newDate)
{
    gpsSignals.dateYear = newDate % APP_DATE_COEFF1;
    gpsSignals.dateMon  = (newDate / APP_DATE_COEFF1) % APP_DATE_COEFF1;
    gpsSignals.dateDay  = ((newDate / APP_DATE_COEFF2) % APP_DATE_COEFF1);
    ApplyTimezone();
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    gpsSignals.fixQuality = newFixQuality;
    UpdateSignalFixStatus();
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum)
{
    gpsSignals.satellitesNum = newSatellitesNum;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_lonDir(char newLonDir)
{
    gpsSignals.lonDir = newLonDir;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_latDir(char newLatDir)
{
    gpsSignals.latDir = newLatDir;
}
void AppActivityScreenPresenter::NotifySignalChanged_sensorData_altitude(float newAltitude)
{
    sensorData.altitude = newAltitude;
    UpdateAltitude();
}
void AppActivityScreenPresenter::NotifySignalChanged_sensorData_pressure(float newPressure)
{
    sensorData.pressure = newPressure;
}
void AppActivityScreenPresenter::NotifySignalChanged_sensorData_temperature(float newTemperature)
{
    sensorData.temperature = newTemperature;
}
void AppActivityScreenPresenter::NotifySignalChanged_sdCardInfo_state(uint8_t state)
{
    sdCardData.state = state;
    UpdateSignalSdCard();
}
