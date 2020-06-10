#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "BitmapDatabase.hpp"
#include "filesystem.h"
#include "dataCollector.h"

uint32_t timeDiff = 0u;
uint32_t timeMaxDiff = 0u;

/* Common array to store lat and lon coordinates recorded during activity 
   and from preloaded .gpx file. First part is for tracked coords, second one 
   for points to navigate. */
AppActivity_coordinatesGPS_T trackPoints[APP_TRACK_COMMONARRAY_LENGTH] __attribute__ ((section (".ccm"))) = {0.f};

static const AppActivity_mapCoordsXY_T mapCoordsXY = {{APP_TRACK_WINDOW_MID_X, APP_TRACK_WINDOW_MID_Y},
                                                      {APP_TRACK_WINDOW_UPLEFT_X, APP_TRACK_WINDOW_UPLEFT_Y},
                                                      {APP_TRACK_WINDOW_UPRIGHT_X, APP_TRACK_WINDOW_UPRIGHT_Y},
                                                      {APP_TRACK_WINDOW_BOTTOMLEFT_X, APP_TRACK_WINDOW_BOTTOMLEFT_Y},
                                                      {APP_TRACK_WINDOW_BOTTOMRIGHT_X, APP_TRACK_WINDOW_BOTTOMRIGHT_Y}};

AppActivityScreenPresenter::AppActivityScreenPresenter(AppActivityScreenView& v)
    : view(v)
{
    memset(&activityData, 0u, sizeof(activityData));
    memset(&appInternalData, 0u, sizeof(appInternalData));
    memset(&fileInfo, 0u, sizeof(fileInfo));
    memset(&gpsSignals, 0u, sizeof(gpsSignals));
    memset(&sensorData, 0u, sizeof(sensorData));
    memset(&trackData, 0u, sizeof(trackData));

    appInternalData.state = APP_STATE_INIT;
    appInternalData.screen = APP_SCREEN_MAIN;
    appInternalData.maxDistancePerSecond = APP_DISTANCE_MAXVALUE_KM_S;
    appInternalData.maxSlopePerSecond = APP_SLOPE_MAXVALUE_PERSEC;
    appInternalData.mainTimePeriod = APP_MAINPERIOD_MS;
    appInternalData.callCounter = APP_MAX_CALL_COUNTER;

    fileInfo.fileStatus = APP_FILE_NOFILE;

    trackData.scale = APP_TRACK_SCALE100;
    trackData.mapCoordsXY = (AppActivity_mapCoordsXY_T*)&mapCoordsXY;

    trackPointsData.coords = trackPoints;
    trackPointsData.idxTrack = APP_TRACK_TRACK_FIRST_ELEMENT;
    trackPointsData.idxMap = APP_TRACK_MAP_FIRST_ELEMENT;

    SetBitmapButton(BITMAP_BLUE_ICONS_PLAY_32_ID);
}

void AppActivityScreenPresenter::activate()
{
    uint32_t scaleVal = GetScaleValue();
    view.SetTrackScale(scaleVal);

    model->SignalRequestFromPresenter();
    model->MainPeriodFromPresenter(appInternalData.mainTimePeriod);

    appInternalData.state = APP_STATE_READY;
}

void AppActivityScreenPresenter::deactivate()
{

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

        snprintf(fileInfo.name, APP_FILENAMEMAXLEN, "%s/20%.2d%.2d%.2d_%.2d%.2d%.2d.gpx",
                FS_OUTPUTPATH, gpsSignals.dateYear, gpsSignals.dateMon, gpsSignals.dateDay,
                gpsSignals.timeHr, gpsSignals.timeMin, gpsSignals.timeSec);

        uint8_t retVal = FS_OpenFile((FS_File_T**)&fileInfo.filePtr, fileInfo.name, FS_MODEWRITE);
        if(RET_OK == retVal)
        {
            fileInfo.fileStatus = APP_FILE_CREATED;
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
            fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)initBufferOne);
            fileInfo.errorStatus |= FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)initBufferTwo);
        }
        else
        {
            /* File can not be created */
            fileInfo.fileStatus = APP_FILE_ERROR;
        }
    }
    else
    {
        /* No fix, file will not be created */
        fileInfo.fileStatus = APP_FILE_PENDING;
    }
}


/* Method called to finish activity, close file and
   to change appInternalData.state. */
void AppActivityScreenPresenter::FinishActivity(void)
{
    if(APP_FILE_NOFILE != fileInfo.fileStatus)
    {
        const char* finishBuffer = 
            "\t\t</trkseg>\n"
            "\t</trk>\n"
            "</gpx>";
        fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)finishBuffer);

        uint8_t retVal = FS_CloseFile((FS_File_T**)&fileInfo.filePtr);
        if(RET_OK == retVal)
        {
            fileInfo.fileStatus = APP_FILE_CLOSED;
        }
        else
        {
            fileInfo.fileStatus = APP_FILE_ERROR;
        }
        fileInfo.filePtr = NULL;
        appInternalData.state = APP_STATE_FINISHED;
    }
}


/* Main method, triggered periodically by model. */
void AppActivityScreenPresenter::Main(void)
{
    if(APP_FILE_CREATED == fileInfo.fileStatus)
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

                DrawTrack();
            }

            if(appInternalData.fileCounter >= APP_MAX_FILE_COUNTER)
            {
                appInternalData.fileCounter= 0u;

                SaveFile();
            }
            
        }
    }
    else if(APP_FILE_PENDING == fileInfo.fileStatus)
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
    uint8_t buffer[APP_MAXFILEBUFFERSIZE] = {0u};

    AppActivity_floatToInt_T lat = {0u};
    AppActivity_floatToInt_T lon = {0u};
    AppActivity_floatToInt_T alt = {0u};

    SaveCoordsInCcmRam(gpsSignals.latitude, gpsSignals.longitude);
    FindMaxCoords(gpsSignals.latitude, gpsSignals.longitude);

    ConvertFloatToInt(gpsSignals.latitude,  lat, APP_LATLON_PRECISION);
    ConvertFloatToInt(gpsSignals.longitude, lon, APP_LATLON_PRECISION);
    ConvertFloatToInt(sensorData.altitude,  alt, APP_ALTI_PRECISION);

#ifdef APP_GPXFILE_SHORTMODE
    snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE,
        "\t\t\t<trkpt lat=\"%d.%.*lu\" lon=\"%d.%.*lu\"><ele>%d.%.*lu</ele><time>20%.2d-%.2d-%.2dT%.2d:%.2d:%.2d.000Z</time></trkpt>\n",
        lat.sint, APP_LATLON_PRECISION, lat.frac, lon.sint, APP_LATLON_PRECISION, lon.frac, alt.sint, APP_ALTI_PRECISION, alt.frac,
        gpsSignals.dateYear, gpsSignals.dateMon, gpsSignals.dateDay, gpsSignals.timeHr, gpsSignals.timeMin, gpsSignals.timeSec);
    fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)buffer);

#else   /* Default saving mode */
    snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE,
        "\t\t\t<trkpt lat=\"%d.%.*lu\" lon=\"%d.%.*lu\">\n"
        "\t\t\t\t<ele>%d.%.*lu</ele>\n",
        lat.sint, APP_LATLON_PRECISION, lat.frac, lon.sint, APP_LATLON_PRECISION, lon.frac, alt.sint, APP_ALTI_PRECISION, alt.frac);
    fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)buffer);

    memset(buffer, 0u, sizeof(buffer));
    snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE,
        "\t\t\t\t<time>20%.2d-%.2d-%.2dT%.2d:%.2d:%.2d.000Z</time>\n"
        "\t\t\t</trkpt>\n",
        gpsSignals.dateYear, gpsSignals.dateMon, gpsSignals.dateDay, gpsSignals.timeHr, gpsSignals.timeMin, gpsSignals.timeSec);
    fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)buffer);
#endif

    UpdateSignalSdCard();
    fileInfo.points++;
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

    if((APP_SDCARD_INITIALIZED == sdCardData.state) && (APP_FILE_ERROR != fileInfo.fileStatus) && (0u == fileInfo.errorStatus))
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


float AppActivityScreenPresenter::CalculateDistance(float lat, float lon, float lastLat, float lastLon)
{
    float distance = sqrtf( POW2( (lat - lastLat) ) + 
                            POW2( (cosf(lastLat * APP_DISTANCE_COEFF_TORAD) * (lon - lastLon) ) ) 
                          ) * APP_DISTANCE_COEFF_TONUM;

    return distance;
}


/* Method called to calculate total distance and speed. */
void AppActivityScreenPresenter::CalculateSpeedAndDistance(void)
{
    static float lastLat = 0.f;
    static float lastLon = 0.f;

    if(APP_DISTANCE_MIN_POINTS_NUM > fileInfo.points)
    {
        /* Do not proceed on first entry when there is no enough points */
    }
    else
    {
        float distance = CalculateDistance(gpsSignals.latitude, gpsSignals.longitude, lastLat, lastLon);

        /* To prevent calculation and data acquisition mistakes */
        if(appInternalData.maxDistancePerSecond > distance)
        {
            activityData.distance += distance;
            activityData.speed = distance * APP_SEC_IN_HR;
            activityData.avgSpeed = (float)(activityData.distance / (activityData.timer / APP_SEC_IN_HR / APP_TIMER_COEFF_TOSEC));
            activityData.maxSpeed = (activityData.speed > activityData.maxSpeed) ? activityData.speed : activityData.maxSpeed;
        }
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
    static uint8_t idx = APP_ALTI_INTERVAL;
    static float   lastDist[APP_ALTI_INTERVAL] = {0u};
    static float   lastAlti[APP_ALTI_INTERVAL] = {0u};
    static float   lastDistance = activityData.distance;
    static float   lastAltitude = sensorData.altitude;

    if(APP_ALTI_INTERVAL == idx)
    {
        /* Local init */
        for(uint8_t i=0u; i < APP_ALTI_INTERVAL; i++)
        {
            lastDist[i] = 0.f;
            lastAlti[i] = sensorData.altitude;
        }
        idx = 0u;
    }

    /* Add new data to cyclic buffer */
    lastDist[idx] = activityData.distance;
    lastAlti[idx] = sensorData.altitude;

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

    if(distDiff > 0.f)
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
        view.NotifySignalChanged_activityData_altiUp(activityData.altiUp);
        view.NotifySignalChanged_activityData_altiDown(activityData.altiDown);
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

    uint32_t diff = newTime - appInternalData.lastTime;

    timeDiff = diff;
    timeMaxDiff = (diff > timeMaxDiff) ? diff : timeMaxDiff;

    activityData.timer += newTime - appInternalData.lastTime;
    appInternalData.lastTime = newTime;

    if(APP_SCREEN_MAIN == appInternalData.screen)
    {
        view.NotifySignalChanged_activityData_timer(activityData.timer);
    }
}


/* Method called to update activity time (clock). */
void AppActivityScreenPresenter::UpdateTime(void)
{
    activityData.time = gpsSignals.timeHr * APP_TIME_COEFF2 + gpsSignals.timeMin * APP_TIME_COEFF1 + gpsSignals.timeSec;

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
            view.NotifySignalChanged_sensorData_altitude(sensorData.altitude);
        }
    }
}


/* Method called to draw track from points recorded and
   saved in gpx file. */
void AppActivityScreenPresenter::DrawTrack(void)
{
    if(APP_SCREEN_MAP == appInternalData.screen)
    {
        bool firstCycle = true;
        uint8_t addedPoints = 0u;
        uint32_t scaleVal = GetScaleValue();
        view.SetTrackScale(scaleVal);
        view.FlushTrackList();

        CalculateSkippedCoords();
        MapCenterCoordinates();

        for (uint16_t idx = trackPointsData.idxTrack; ((idx > 0u) && (APP_TRACK_MAP_ELEMENTS > addedPoints)) ; idx--)
        {
            bool coordsInView = CoordsInView(trackPointsData.coords[idx]);

            if(true == coordsInView)
            {
                AppActivity_coordinatesXY_T coordsXY = MapGPSCoordsToXY(trackPointsData.coords[idx]);
                bool newPoint = view.AddCoordsToTrackList(coordsXY);

                if(true == newPoint)
                {
                    addedPoints++;
                }
            }

            /* Set index to 0 if array was overflowed */
            if((APP_TRACK_SKIP_BOTTOM_LIMIT >= idx) && (true == trackPointsData.overflow) && (true == firstCycle))
            {
                idx = trackPointsData.idxMap - APP_TRACK_MAP_OFFSET_FOR_TRACK;
                firstCycle = false;
            }
            /* Skip some points depending on selected scale */
            else if((idx > trackData.skip) && ((uint16_t)(idx - trackData.skip) > APP_TRACK_SKIP_BOTTOM_LIMIT))
            {
                idx -= trackData.skip;
            }
        }

        trackData.addedPoints = addedPoints;
        view.TrackRedraw();

#if 0
        uint8_t buffer[APP_MAXFILEBUFFERSIZE] = {0u};
        AppActivity_coordinatesGPS_T coordsGPS = {0.f};
        uint32_t offset = 0u;
        uint8_t readLines = 0u;
        uint8_t retVal = RET_OK;
        boolean isMoreLines = true;
        bool coordsInView = false;

        offset = CalculateFileOffset(fileInfo.points);
        scaleVal = GetScaleValue(trackData.scale);
        retVal = FS_Lseek((FS_File_T**)&fileInfo.filePtr, offset);

        if(RET_OK == retVal)
        {
            view.FlushTrackList();
            view.SetTrackScale(scaleVal);

            while((true == isMoreLines) && (APP_TRACK_FILE_READLINES > readLines))
            {
                fileInfo.errorStatus = FS_ReadFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)buffer, APP_MAXFILEBUFFERSIZE, &isMoreLines);
                readLines++;

                coordsGPS = GetCoordsGPSFromBuffer(buffer, APP_MAXFILEBUFFERSIZE);
                coordsInView = CoordsInView(coordsGPS);

                if(true == coordsInView)
                {
                    AppActivity_coordinatesXY_T coordsXY = MapGPSCoordsToXY(coordsGPS);
                    view.AddCoordsToTrackList(coordsXY);
                }
            }

            view.TrackRedraw();
        }

        retVal = FS_LseekEnd((FS_File_T**)&fileInfo.filePtr);
#endif
    }
}


/* Function called to calculate number of coords to skip during
   selecting points to draw. */
void AppActivityScreenPresenter::CalculateSkippedCoords(void)
{
    if(APP_TRACK_MAP_ELEMENTS > fileInfo.points)   /* Do not skip when there is less points than maximum possible to draw */
    {
        trackData.skip = 0u;
    }
    else /* Calculate skip value based on previous results */
    {
        /* Small amount of points */
        if (trackData.addedPoints < APP_TRACK_SKIP_DRAWN_POINTS_BOTTOM_LIMIT)
        {
            if(trackData.skip == 0u)
            {
                /* Select default skip value */
                switch(trackData.scale)
                {
                    case APP_TRACK_SCALE50:
                        trackData.skip = APP_TRACK_SKIPPED_COORDS_50;
                        break;
                    case APP_TRACK_SCALE100:
                        trackData.skip = APP_TRACK_SKIPPED_COORDS_100;
                        break;
                    case APP_TRACK_SCALE500:
                        trackData.skip = APP_TRACK_SKIPPED_COORDS_500;
                        break;
                    case APP_TRACK_SCALE1000:
                        trackData.skip = APP_TRACK_SKIPPED_COORDS_1000;
                        break;
                    case APP_TRACK_SCALE5000:
                        trackData.skip = APP_TRACK_SKIPPED_COORDS_5000;
                        break;
                    case APP_TRACK_SCALEFULL:
                        trackData.skip = (uint8_t)(fileInfo.points / APP_TRACK_MAP_ELEMENTS);
                        break;
                    default:
                        trackData.skip = 0u;
                        break;
                }
            }
            else
            {
                trackData.skip -= 1u;
            }
            trackData.skip = (trackData.skip != 0u) ? (trackData.skip - 1u) : 0u;
        }
        /* Enough points */
        else if((trackData.addedPoints >= APP_TRACK_SKIP_DRAWN_POINTS_BOTTOM_LIMIT) && (trackData.addedPoints < APP_TRACK_SKIP_DRAWN_POINTS_UPPER_LIMIT))
        {

        }
        /* Too much points */
        else
        {
            trackData.skip = (trackData.skip != APP_TRACK_SKIP_MAX_VALUE) ? (trackData.skip + 1u) : 0u;
        }
    }
}


/* Function called to find gps coordinates
   from file buffer. */
AppActivity_coordinatesGPS_T AppActivityScreenPresenter::GetCoordsGPSFromBuffer(uint8_t* buffer, uint8_t size)
{
    AppActivity_coordinatesGPS_T coords = {0.f};
    const char* wantedChar = "\"";

    uint8_t latStart = 0u;
    uint8_t latEnd   = 0u;
    uint8_t lonStart = 0u;
    uint8_t lonEnd   = 0u;
    AppActivity_trackCoordsFromBuffer_T cnt = APP_TRACK_LATSTART;

    for(uint8_t i=0u; i < size; i++)
    {
        if(buffer[i] == *wantedChar)
        {
            switch (cnt)
            {
                case APP_TRACK_LATSTART:
                    latStart = i+1u;
                    break;
                case APP_TRACK_LATEND:
                    latEnd = i-1u;
                    break;
                case APP_TRACK_LONSTART:
                    lonStart = i+1u;
                    break;
                case APP_TRACK_LONEND:
                    lonEnd = i-1u;
                    break;
                default:
                    break;
            }
            cnt = (AppActivity_trackCoordsFromBuffer_T)(cnt + 1u);
        }
        if(APP_TRACK_FOUNDALL == cnt)
        {
            break;
        }
    }

    if(APP_TRACK_FOUNDALL == cnt)
    {
        uint8_t floatBuff[APP_TRACK_FLOATBUFF_SIZE] = {0u};
        memcpy (floatBuff, &buffer[latStart], latEnd-latStart+1u);
        memcpy (&floatBuff[APP_TRACK_FLOATBUFF_HALF], &buffer[lonStart], lonEnd-lonStart+1u);
        coords.lat = strtof((const char*)floatBuff, NULL);
        coords.lon = strtof((const char*)&floatBuff[APP_TRACK_FLOATBUFF_HALF], NULL);
    }

    return coords;
}


/* Method called to check whether coordinates are in the visible
   area on a display. */
bool AppActivityScreenPresenter::CoordsInView(AppActivity_coordinatesGPS_T coords)
{
    bool isInView = false;

    if((coords.lat < trackData.mapCoordsGPS.upLeft.lat)      && (coords.lon > trackData.mapCoordsGPS.upLeft.lon) &&
       (coords.lat < trackData.mapCoordsGPS.upRight.lat)     && (coords.lon < trackData.mapCoordsGPS.upRight.lon) &&
       (coords.lat > trackData.mapCoordsGPS.bottomLeft.lat)  && (coords.lon > trackData.mapCoordsGPS.bottomLeft.lon) &&
       (coords.lat > trackData.mapCoordsGPS.bottomRight.lat) && (coords.lon < trackData.mapCoordsGPS.bottomRight.lon))
    {
        isInView = true;
    }

    return isInView;
}


/* Method called to find latitude and longitude to corresponding
   corners of a display. */
void AppActivityScreenPresenter::MapCenterCoordinates(void)
{
    float scaleDistCoeff = MapScaleToDistance();

    if(APP_TRACK_SCALEFULL == trackData.scale)
    {
        trackData.mapCoordsGPS.center.lat = trackData.maxCoordsGPS.center.lat;
        trackData.mapCoordsGPS.center.lon = trackData.maxCoordsGPS.center.lon;
    }
    else
    {
        trackData.mapCoordsGPS.center.lat = gpsSignals.latitude;
        trackData.mapCoordsGPS.center.lon = gpsSignals.longitude;
    }

    trackData.mapCoordsGPS.upLeft = MapXYCoordsToGPS(trackData.mapCoordsXY->upLeft, scaleDistCoeff);
    trackData.mapCoordsGPS.upRight = MapXYCoordsToGPS(trackData.mapCoordsXY->upRight, scaleDistCoeff);
    trackData.mapCoordsGPS.bottomLeft = MapXYCoordsToGPS(trackData.mapCoordsXY->bottomLeft, scaleDistCoeff);
    trackData.mapCoordsGPS.bottomRight = MapXYCoordsToGPS(trackData.mapCoordsXY->bottomRight, scaleDistCoeff);
}


/* Function called to get scale value from enum indicator. */
uint32_t AppActivityScreenPresenter::GetScaleValue(void)
{
    uint32_t scaleVal = 0u;

    switch (trackData.scale)
    {
        case APP_TRACK_SCALE50:
            scaleVal = APP_TRACK_SCALE_50;
            break;
        case APP_TRACK_SCALE100:
            scaleVal = APP_TRACK_SCALE_100;
            break;
        case APP_TRACK_SCALE500:
            scaleVal = APP_TRACK_SCALE_500;
            break;
        case APP_TRACK_SCALE1000:
            scaleVal = APP_TRACK_SCALE_1000;
            break;
        case APP_TRACK_SCALE5000:
            scaleVal = APP_TRACK_SCALE_5000;
            break;
        case APP_TRACK_SCALEFULL:
            scaleVal = CalculateFullScale();
            break;
        default:
            scaleVal = APP_TRACK_SCALE_ERROR;
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
    float maxHeigth = CalculateDistance(trackData.maxCoordsGPS.N.lat, trackData.maxCoordsGPS.N.lon, trackData.maxCoordsGPS.S.lat, trackData.maxCoordsGPS.S.lon);
    float maxWidth  = CalculateDistance(trackData.maxCoordsGPS.E.lat, trackData.maxCoordsGPS.E.lon, trackData.maxCoordsGPS.W.lat, trackData.maxCoordsGPS.W.lon);

    maxDist = (maxHeigth > maxWidth) ? maxHeigth : maxWidth;
    scaleVal = (uint32_t)(maxDist * (float)APP_TRACK_SCALE_COEFF_M_IN_KM / APP_TRACK_SCALE_FULL_COEFFVIEW);

    return scaleVal;
}


/* Method called to map enum scale to corresponding value
   in kilometers. */
float AppActivityScreenPresenter::MapScaleToDistance(void)
{
    float scaleDist = (float)GetScaleValue();

    /* Scale to 1px per x km */
    scaleDist /= (float)(APP_TRACK_SCALE_WIDTH_PX * APP_TRACK_SCALE_COEFF_M_IN_KM);

    return scaleDist;
}


/* Function called to map given XY coordinates to GPS ones. */
AppActivity_coordinatesGPS_T AppActivityScreenPresenter::MapXYCoordsToGPS(AppActivity_coordinatesXY_T coords, float scaleCoeff)
{
    AppActivity_coordinatesGPS_T coordsGPS = {0.f};

    float distX = ((trackData.mapCoordsXY->center.X - coords.X) * scaleCoeff) / (APP_DISTANCE_COEFF_TONUM * cosf(trackData.mapCoordsGPS.center.lat * APP_DISTANCE_COEFF_TORAD));
    float distY = ((trackData.mapCoordsXY->center.Y - coords.Y) * scaleCoeff) / (APP_DISTANCE_COEFF_TONUM);

    coordsGPS.lat = trackData.mapCoordsGPS.center.lat + distY;
    coordsGPS.lon = trackData.mapCoordsGPS.center.lon - distX;

    return coordsGPS;
}


/* Function called to map given GPS coordinates to XY ones. */
AppActivity_coordinatesXY_T AppActivityScreenPresenter::MapGPSCoordsToXY(AppActivity_coordinatesGPS_T coords)
{
    AppActivity_coordinatesXY_T coordsXY = {0u};

    coordsXY.X = MapPointToLinearFunction(trackData.mapCoordsGPS.upLeft.lon,  trackData.mapCoordsXY->upLeft.X,
                                          trackData.mapCoordsGPS.upRight.lon, trackData.mapCoordsXY->upRight.X,
                                          coords.lon);

    coordsXY.Y = MapPointToLinearFunction(trackData.mapCoordsGPS.upLeft.lat,     trackData.mapCoordsXY->upLeft.Y,
                                          trackData.mapCoordsGPS.bottomLeft.lat, trackData.mapCoordsXY->bottomLeft.Y,
                                          coords.lat);

    return coordsXY;
}


/* Method called to map point to linear function. 
   To given X corresponding Y will be returned. */
float AppActivityScreenPresenter::MapPointToLinearFunction(float x1, float y1, float x2, float y2, float X)
{
    float a = (y2-y1)/(x2-x1);
    float b = (y1*x2 - x1*y2)/(x2-x1);
    float Y = a*X + b;

    return Y;
}


/* Method called to change scale on map to zoom in. */
void AppActivityScreenPresenter::ZoomIn(void)
{
    if(trackData.scale > APP_TRACK_SCALE50)
    {
        trackData.scale = (AppActivity_trackScale_T)(trackData.scale - 1u);
        DrawTrack();
    }
    else
    {
        trackData.scale = APP_TRACK_SCALE50;
    }
}


/* Method called to change scale on map to zoom out. */
void AppActivityScreenPresenter::ZoomOut(void)
{
    if((trackData.scale + 1u) < APP_TRACK_MAX_SCALE)
    {
        trackData.scale = (AppActivity_trackScale_T)(trackData.scale + 1u);
        DrawTrack();
    }
    else
    {
        trackData.scale = (AppActivity_trackScale_T)(APP_TRACK_MAX_SCALE - 1u);
    }
}


/* Method called to calculate a number of bytes to move track file pointer. */
uint32_t AppActivityScreenPresenter::CalculateFileOffset(uint16_t points)
{
    uint32_t offset = APP_TRACK_FILE_HEADEROFFSET;

    if(points > APP_TRACK_FILE_READLINES)
    {
        offset += (points - APP_TRACK_FILE_READLINES) * APP_TRACK_FILE_DATAOFFSET;
    }

    return offset;
}


/* Method called to save new coords to track list safely. */
void AppActivityScreenPresenter::SaveCoordsInCcmRam(float lat, float lon)
{
    if((trackPointsData.idxTrack + 1u) < trackPointsData.idxMap)
    {
        if((0u == trackPointsData.idxTrack) && (0.f == trackPointsData.coords[0u].lat) && (0.f == trackPointsData.coords[0u].lon))
        {
            trackPointsData.coords[0u].lat = lat;
            trackPointsData.coords[0u].lon = lon;
        }
        else
        {
            trackPointsData.idxTrack++;
            trackPointsData.coords[trackPointsData.idxTrack].lat = lat;
            trackPointsData.coords[trackPointsData.idxTrack].lon = lon;
        }
    }
    else
    {
        trackPointsData.overflow = true;
        trackPointsData.idxTrack = 0u;
        trackPointsData.coords[0u].lat = 0.f;
        trackPointsData.coords[0u].lon = 0.f;
    }
}


/* Method called to find extreme locations. */
void AppActivityScreenPresenter::FindMaxCoords(float lat, float lon)
{
    bool calcNewCenter = false;

    if((0.f == trackData.maxCoordsGPS.N.lat) && (0.f == trackData.maxCoordsGPS.E.lon) && 
       (0.f == trackData.maxCoordsGPS.W.lon) && (0.f == trackData.maxCoordsGPS.S.lat))
    {
        /* Assign current coords to all maxs during first call */
        trackData.maxCoordsGPS.N.lat = lat;
        trackData.maxCoordsGPS.N.lon = lon;
        trackData.maxCoordsGPS.E.lat = lat;
        trackData.maxCoordsGPS.E.lon = lon;
        trackData.maxCoordsGPS.W.lat = lat;
        trackData.maxCoordsGPS.W.lon = lon;
        trackData.maxCoordsGPS.S.lat = lat;
        trackData.maxCoordsGPS.S.lon = lon;
        calcNewCenter = true;
    }
    else
    {
        if(lat > trackData.maxCoordsGPS.N.lat)
        {
            trackData.maxCoordsGPS.N.lat = lat;
            trackData.maxCoordsGPS.N.lon = lon;
            calcNewCenter = true;
        }
        else if(lon > trackData.maxCoordsGPS.E.lon)
        {
            trackData.maxCoordsGPS.E.lat = lat;
            trackData.maxCoordsGPS.E.lon = lon;
            calcNewCenter = true;
        }
        else if(lon < trackData.maxCoordsGPS.W.lon)
        {
            trackData.maxCoordsGPS.W.lat = lat;
            trackData.maxCoordsGPS.W.lon = lon;
            calcNewCenter = true;
        }
        else if(lat < trackData.maxCoordsGPS.S.lat)
        {
            trackData.maxCoordsGPS.S.lat = lat;
            trackData.maxCoordsGPS.S.lon = lon;
            calcNewCenter = true;
        }
    }

    if(true == calcNewCenter)
    {
        trackData.maxCoordsGPS.center.lat = (trackData.maxCoordsGPS.N.lat + trackData.maxCoordsGPS.S.lat) / APP_TRACK_MEANTWO_COEEF;
        trackData.maxCoordsGPS.center.lon = (trackData.maxCoordsGPS.E.lon + trackData.maxCoordsGPS.W.lon) / APP_TRACK_MEANTWO_COEEF;
    }
}


/* Method called to filter out wrong coordinates
   by selecting median from set. */
void AppActivityScreenPresenter::FilterCoords(void)
{
    static float lat[APP_COORDS_FILTER_ARR_SIZE] = {0.f};
    static float lon[APP_COORDS_FILTER_ARR_SIZE] = {0.f};
    static uint8_t idx = APP_COORDS_FILTER_IDX_INIT;

    if(APP_COORDS_FILTER_IDX_INIT == idx)
    {
        for(uint8_t i = 0u; i < APP_COORDS_FILTER_ARR_SIZE; i++)
        {
            lat[i] = gpsSignals.latitude;
            lon[i] = gpsSignals.longitude;
        }
        idx = 0u;
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
    uint8_t retVal = FS_CloseFile((FS_File_T**)&fileInfo.filePtr);
    fileInfo.fileStatus = (RET_OK == retVal) ? APP_FILE_CLOSED : APP_FILE_ERROR;

    retVal = FS_OpenFile((FS_File_T**)&fileInfo.filePtr, fileInfo.name, FS_MODEAPPEND);
    fileInfo.fileStatus = (RET_OK == retVal) ? APP_FILE_CREATED : APP_FILE_ERROR;
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
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_time(uint32_t newTime)
{
    gpsSignals.timeSec  = newTime % APP_TIME_COEFF1;
    gpsSignals.timeMin = (newTime / APP_TIME_COEFF1) % APP_TIME_COEFF1;
    gpsSignals.timeHr = ((newTime / APP_TIME_COEFF2) % APP_TIME_COEFF1);
    UpdateTime();
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_date(uint32_t newDate)
{
    gpsSignals.dateYear = newDate % APP_DATE_COEFF1;
    gpsSignals.dateMon  = (newDate / APP_DATE_COEFF1) % APP_DATE_COEFF1;
    gpsSignals.dateDay  = ((newDate / APP_DATE_COEFF2) % APP_DATE_COEFF1);
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
