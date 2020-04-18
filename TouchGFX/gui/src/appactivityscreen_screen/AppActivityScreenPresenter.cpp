#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>
#include <stdio.h>
#include <string.h>
#include "BitmapDatabase.hpp"
#include "filesystem.h"


AppActivityScreenPresenter::AppActivityScreenPresenter(AppActivityScreenView& v)
    : view(v)
{
    mainTimePeriod = APP_MAINPERIOD_MS;
    activityData.state = APP_READY;
    fileInfo.fileStatus = APP_NOFILE;
    SetBitmapButton(BITMAP_BLUE_ICONS_PLAY_32_ID);
}

void AppActivityScreenPresenter::activate()
{
    model->SignalRequestFromPresenter();
    model->MainPeriodFromPresenter(mainTimePeriod);
}

void AppActivityScreenPresenter::deactivate()
{

}


/* Method called after pause/start button
   is pressed. Its point is to change activityData.state
   to proper state. */
void AppActivityScreenPresenter::StartStopActivity(void)
{
    if(APP_READY == activityData.state)
    {
        activityData.state = APP_RUNNING;
        InitActivity();
        SetBitmapButton(BITMAP_BLUE_ICONS_PAUSE_32_ID);
    }
    else if(APP_PAUSED == activityData.state)
    {
        activityData.state = APP_RUNNING;
        SetBitmapButton(BITMAP_BLUE_ICONS_PAUSE_32_ID);
    }
    else if(APP_RUNNING == activityData.state)
    {
        activityData.state = APP_PAUSED;
        SetBitmapButton(BITMAP_BLUE_ICONS_PLAY_32_ID);
    }
}


/* Method called when starting an activity to
   prepare output file. */
void AppActivityScreenPresenter::InitActivity(void)
{
    snprintf(fileInfo.name, APP_FILENAMEMAXLEN, "%s/20%.2d%.2d%.2d_%.2d%.2d%.2d.gpx",
             FS_OUTPUTPATH, gpsSignals.dateYear, gpsSignals.dateMon, gpsSignals.dateDay,
             gpsSignals.timeHr, gpsSignals.timeMin, gpsSignals.timeSec);

    uint8_t retVal = FS_OpenFile((FS_File_T**)&fileInfo.filePtr, fileInfo.name, FS_MODEWRITE);
    if(RET_OK == retVal)
    {
        fileInfo.fileStatus = APP_FILECREATED;

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
        fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)initBufferTwo);
    }
}


/* Method called to finish activity, close file and
   to change activityData.state. */
void AppActivityScreenPresenter::FinishActivity(void)
{
    if(APP_NOFILE != fileInfo.fileStatus)
    {
        const char* finishBuffer = 
            "\t\t</trkseg>\n"
            "\t</trk>\n"
            "</gpx>";
        fileInfo.errorStatus = FS_WriteFile((FS_File_T*)fileInfo.filePtr, (uint8_t*)finishBuffer);

        uint8_t retVal = FS_CloseFile((FS_File_T**)&fileInfo.filePtr);
        if(RET_OK == retVal)
        {
            fileInfo.fileStatus = APP_FILECLOSED;
        }
        else
        {
            fileInfo.fileStatus = APP_FILEERROR;
        }
        fileInfo.filePtr = NULL;
        activityData.state = APP_FINISHED;
    }
}


/* Main method, triggered periodically by model. */
void AppActivityScreenPresenter::Main(void)
{
    const uint8_t maxCallCounter = (APP_MS_IN_SEC) / (APP_MAINPERIOD_MS);
    static uint8_t callCounter = maxCallCounter;

    if((APP_FILECREATED == fileInfo.fileStatus) && (APP_RUNNING == activityData.state))
    {
        callCounter++;

        activityData.timer++;
        view.NotifySignalChanged_activityData_timer(activityData.timer);

        if(callCounter >= maxCallCounter)
        {
            callCounter = 0u;
            InsertDataIntoFile();
        }
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
    view.NotifySignalChanged_gpsData_fixQuality(newFixQuality);
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
