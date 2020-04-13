#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>
#include "BitmapDatabase.hpp"
#include "stdio.h"
#include "filesystem.h"

AppActivityScreenPresenter::AppActivityScreenPresenter(AppActivityScreenView& v)
    : view(v)
{
    mainTimePeriod = 250u;
    activityState = APP_READY;
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
   is pressed. Its point is to change activityState
   to proper state. */
void AppActivityScreenPresenter::StartStopActivity(void)
{
    if(APP_READY == activityState)
    {
        activityState = APP_RUNNING;
        InitActivity();
        SetBitmapButton(BITMAP_BLUE_ICONS_PAUSE_32_ID);
    }
    else if(APP_PAUSED == activityState)
    {
        activityState = APP_RUNNING;
        SetBitmapButton(BITMAP_BLUE_ICONS_PAUSE_32_ID);
    }
    else if(APP_RUNNING == activityState)
    {
        activityState = APP_PAUSED;
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
    }
}


/* Method called to finish activity, close file and
   to change activityState. */
void AppActivityScreenPresenter::FinishActivity(void)
{
    if(APP_NOFILE != fileInfo.fileStatus)
    {
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
        activityState = APP_FINISHED;
    }
}


/* Main method, triggered periodically by model. */
void AppActivityScreenPresenter::Main(void)
{
    if((APP_FILECREATED == fileInfo.fileStatus) && (APP_RUNNING == activityState))
    {
        uint8_t buffer[APP_MAXFILEBUFFERSIZE] = {0u};
        snprintf((char*)buffer, APP_MAXFILEBUFFERSIZE, "Date: %.2d.%.2d.20%.2d\nTime: %.2d:%.2d:%.2d\n\n",
                gpsSignals.dateDay, gpsSignals.dateMon, gpsSignals.dateYear,
                gpsSignals.timeHr, gpsSignals.timeMin, gpsSignals.timeSec);
        uint8_t retVal = FS_WriteFile((FS_File_T*)fileInfo.filePtr, buffer);
        if(RET_OK == retVal)
        {
            (void)buffer;
        }
    }
}


/* Method called to set proper bitmap on
   the button */
void AppActivityScreenPresenter::SetBitmapButton(const uint16_t bitmapId)
{
    view.SetBitmapButton(bitmapId);
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
    gpsSignals.latitude = newLatitude;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_longitude(float newLongitude)
{
    gpsSignals.longitude = newLongitude;
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
