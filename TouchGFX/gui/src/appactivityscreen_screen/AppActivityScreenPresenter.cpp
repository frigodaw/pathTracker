#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "BitmapDatabase.hpp"
#include "filesystem.h"
#include "dataCollector.h"



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
    memset(&trackData, 0u, sizeof(trackData));

    appInternalData.state = APP_STATE_INIT;
    appInternalData.screen = APP_SCREEN_MAIN;
    appInternalData.maxDistancePerSecond = APP_DISTANCE_MAXVALUE_PERSEC;
    appInternalData.maxSlopePerSecond = APP_SLOPE_MAXVALUE_PERSEC;
    appInternalData.mainTimePeriod = APP_MAINPERIOD_MS;
    appInternalData.callCounter = APP_MAX_CALL_COUNTER;

    fileInfo.fileStatus = APP_FILE_NOFILE;
    trackData.scale = APP_TRACK_SCALE100;
    trackData.mapCoordsXY = (AppActivity_mapCoordsXY_T*)&mapCoordsXY;
    SetBitmapButton(BITMAP_BLUE_ICONS_PLAY_32_ID);
}

void AppActivityScreenPresenter::activate()
{
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
            fileInfo.errorStatus = 0xFFu;
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

            /* Insert data to file only if fix is present */
            if(true == IsFix())
            {
                appInternalData.callCounter++;

                /* This parts of code executes every 1 second */
                if(appInternalData.callCounter >= APP_MAX_CALL_COUNTER)
                {
                    appInternalData.callCounter = 0u;

                    InsertDataIntoFile();

                    CalculateSpeedAndDistance();

                    CalculateAltitude();

                    DrawTrack();
                }
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

    ConvertFloatToInt(gpsSignals.latitude,  lat, APP_LATLON_PRECISION);
    ConvertFloatToInt(gpsSignals.longitude, lon, APP_LATLON_PRECISION);
    ConvertFloatToInt(sensorData.altitude,  alt, APP_ALTI_PRECISION);

    snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE,
        "\t\t\t<trkpt lat=\"%d.%.*lu\" lon=\"%d.%.*lu\"><ele>%d.%.*lu</ele><time>20%.2d-%.2d-%.2dT%.2d:%.2d:%.2d.000Z</time></trkpt>\n",
        lat.sint, APP_LATLON_PRECISION, lat.frac, lon.sint, APP_LATLON_PRECISION, lon.frac, alt.sint, APP_ALTI_PRECISION, alt.frac,
        gpsSignals.dateYear, gpsSignals.dateMon, gpsSignals.dateDay, gpsSignals.timeHr, gpsSignals.timeMin, gpsSignals.timeSec);
    fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)buffer);

#if 0   /* Default saving mode */
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

    return true;
}


/* Method called to update fix icon on the screen. */
void AppActivityScreenPresenter::UpdateSignalFixStatus(void)
{
    bool isFix = IsFix();

    view.ShowFixImage(isFix);
}


/* Method called to calculate total distance and speed. */
void AppActivityScreenPresenter::CalculateSpeedAndDistance(void)
{
    static float lastLat = 0.f;
    static float lastLon = 0.f;

    if((0.f == lastLat) || (0.f == lastLon))
    {
        /* Do not proceed on first entry */
    }
    else
    {
        float distance = sqrtf( POW2( (gpsSignals.latitude - lastLat) ) + 
                                POW2( (cosf(lastLat * APP_DISTANCE_COEFF_TORAD) * (gpsSignals.longitude - lastLon) ) ) ) *
                            APP_DISTANCE_COEFF_TONUM;

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
    MapCoordinates();

    if(APP_SCREEN_MAP == appInternalData.screen)
    {
        uint8_t buffer[APP_MAXFILEBUFFERSIZE] = {0u};
        AppActivity_coordinatesGPS_T coordsGPS = {0.f};
        boolean isMoreLines = true;
        bool coordsInView = false;
        const uint8_t offset = APP_TRACK_FILEOFFSET;

        uint8_t retVal = FS_Lseek((FS_File_T**)&fileInfo.filePtr, (uint32_t)offset);

        if(RET_OK == retVal)
        {
            view.FlushTrackList();

            while(true == isMoreLines)
            {
                fileInfo.errorStatus = FS_ReadFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)buffer, APP_MAXFILEBUFFERSIZE, &isMoreLines);
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


/* Method called to find latitude and longitue to correspoinding
   cornes of a display. */
void AppActivityScreenPresenter::MapCoordinates(void)
{
    float scaleDistCoeff = MapScaleToDistance(trackData.scale);

    trackData.mapCoordsGPS.center.lat = gpsSignals.latitude;
    trackData.mapCoordsGPS.center.lon = gpsSignals.longitude;

    trackData.mapCoordsGPS.upLeft = MapXYCoordsToGPS(trackData.mapCoordsXY->upLeft, scaleDistCoeff);
    trackData.mapCoordsGPS.upRight = MapXYCoordsToGPS(trackData.mapCoordsXY->upRight, scaleDistCoeff);
    trackData.mapCoordsGPS.bottomLeft = MapXYCoordsToGPS(trackData.mapCoordsXY->bottomLeft, scaleDistCoeff);
    trackData.mapCoordsGPS.bottomRight = MapXYCoordsToGPS(trackData.mapCoordsXY->bottomRight, scaleDistCoeff);
}


/* Method called to map enum scale to corresponding value
   in kilometers. */
float AppActivityScreenPresenter::MapScaleToDistance(AppActivity_trackScale_T scaleEnum)
{
    float scaleDist = 0.f;

    switch (scaleEnum)
    {
        case APP_TRACK_SCALE50:
            scaleDist = APP_TRACK_SCALE_50;
            break;
        case APP_TRACK_SCALE100:
            scaleDist = APP_TRACK_SCALE_100;
            break;
        case APP_TRACK_SCALE500:
            scaleDist = APP_TRACK_SCALE_500;
            break;
        case APP_TRACK_SCALE1000:
            scaleDist = APP_TRACK_SCALE_1000;
            break;
        default:
            scaleDist = APP_TRACK_SCALE_ERROR;
            break;
    }

    /* Scale to 1px per x km */
    scaleDist /= (float)(APP_TRACK_SCALE_WIDTH_PX * APP_TRACK_SCALE_COEFF_MINKM);

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


/* Method called to collect signals from Model:
    - latitude
    - longitude
    - altitude
    - time
    - date
    - fixQuality
    - satellitesNum
    - lonDir
    - latDir */
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
