#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "BitmapDatabase.hpp"
#include "filesystem.h"
#include "dataCollector.h"


AppActivityScreenPresenter::AppActivityScreenPresenter(AppActivityScreenView& v)
    : view(v)
{
    memset(&activityData, 0u, sizeof(activityData));
    memset(&appInternalData, 0u, sizeof(appInternalData));
    memset(&fileInfo, 0u, sizeof(fileInfo));
    memset(&gpsSignals, 0u, sizeof(gpsSignals));

    appInternalData.maxDistancePerSecond = APP_DISTANCE_MAXVALUE_PERSEC;
    appInternalData.mainTimePeriod = APP_MAINPERIOD_MS;
    appInternalData.callCounter = APP_MAX_CALL_COUNTER;
    activityData.state = APP_STATE_READY;
    fileInfo.fileStatus = APP_FILE_NOFILE;
    SetBitmapButton(BITMAP_BLUE_ICONS_PLAY_32_ID);
}

void AppActivityScreenPresenter::activate()
{
    model->SignalRequestFromPresenter();
    model->MainPeriodFromPresenter(appInternalData.mainTimePeriod);

    UpdateTime();
}

void AppActivityScreenPresenter::deactivate()
{

}


/* Method called after pause/start button
   is pressed. Its point is to change activityData.state
   to proper state. */
void AppActivityScreenPresenter::StartStopActivity(void)
{
    if(APP_STATE_READY == activityData.state)
    {
        InitActivity();
    }
    else if(APP_STATE_PAUSED == activityData.state)
    {
        activityData.state = APP_STATE_RUNNING;
        SetBitmapButton(BITMAP_BLUE_ICONS_PAUSE_32_ID);
    }
    else if(APP_STATE_RUNNING == activityData.state)
    {
        activityData.state = APP_STATE_PAUSED;
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
            activityData.state = APP_STATE_RUNNING;
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
   to change activityData.state. */
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
        activityData.state = APP_STATE_FINISHED;
    }
}


/* Main method, triggered periodically by model. */
void AppActivityScreenPresenter::Main(void)
{
    UpdateTime();

    if(APP_FILE_CREATED == fileInfo.fileStatus)
    {
        if(APP_STATE_RUNNING == activityData.state)
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
                }
            }
        }
    }
    else if(APP_FILE_PENDING == fileInfo.fileStatus)
    {
        InitActivity();
    }
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
    ConvertFloatToInt(gpsSignals.altitude,  alt, APP_ALT_PRECISION);

    snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE,
        "\t\t\t<trkpt lat=\"%d.%.*lu\" lon=\"%d.%.*lu\">\n"
        "\t\t\t\t<ele>%d.%.*lu</ele>\n",
        lat.sint, APP_LATLON_PRECISION, lat.frac, lon.sint, APP_LATLON_PRECISION, lon.frac, alt.sint, APP_ALT_PRECISION, alt.frac);
    fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)buffer);

    memset(buffer, 0u, sizeof(buffer));
    snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE,
        "\t\t\t\t<time>20%.2d-%.2d-%.2dT%.2d:%.2d:%.2d.000Z</time>\n"
        "\t\t\t</trkpt>\n",
        gpsSignals.dateYear, gpsSignals.dateMon, gpsSignals.dateDay, gpsSignals.timeHr, gpsSignals.timeMin, gpsSignals.timeSec);
    fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, buffer);
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
        float distance = sqrtf( powf( (gpsSignals.latitude - lastLat), APP_DISTNACE_COEFF_POWER_TWO ) + 
                                powf( (cos(lastLat * APP_DISTANCE_COEFF_PI / APP_DISTANCE_COEFF_ANGLEHALF) * (gpsSignals.longitude - lastLon) ), APP_DISTNACE_COEFF_POWER_TWO ) ) *
                            APP_DISTANCE_COEFF_PERIMETER/APP_DISTANCE_COEFF_ANGLEFULL;

        /* To prevent calculation and data acquisition mistakes */
        if(appInternalData.maxDistancePerSecond > distance)
        {
            activityData.distance += distance;
            activityData.speed = distance * APP_SEC_IN_HR;
            activityData.avgSpeed = (float)(activityData.distance / (activityData.timer / APP_SEC_IN_HR / APP_TIMER_COEFF_TOSEC));
            activityData.maxSpeed = (activityData.speed > activityData.maxSpeed) ? activityData.speed : activityData.maxSpeed;
        }
    }

    view.NotifySignalChanged_activityData_distance(activityData.distance);
    view.NotifySignalChanged_activityData_speed(activityData.speed);
    view.NotifySignalChanged_activityData_avgSpeed(activityData.avgSpeed);
    view.NotifySignalChanged_activityData_maxSpeed(activityData.maxSpeed);

    lastLat = gpsSignals.latitude;
    lastLon = gpsSignals.longitude;
}


/* Method called to increment activity timer. Signal is synchronized
   with global interrupt timer tim_t_100ms. */
void AppActivityScreenPresenter::IncrementTimer(void)
{
    uint32_t newTime = DC_get_main_tim_t_100ms();

    activityData.timer += newTime - appInternalData.lastTime;
    appInternalData.lastTime = newTime;

    view.NotifySignalChanged_activityData_timer(activityData.timer);
}


/* Method called to update activity time (clock). */
void AppActivityScreenPresenter::UpdateTime(void)
{
    activityData.time = gpsSignals.timeHr * 10000 + gpsSignals.timeMin * 100 + gpsSignals.timeSec;
    view.NotifySignalChanged_activityData_time(activityData.time);
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
