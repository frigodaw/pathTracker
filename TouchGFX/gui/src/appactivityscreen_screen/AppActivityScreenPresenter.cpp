#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>

AppActivityScreenPresenter::AppActivityScreenPresenter(AppActivityScreenView& v)
    : view(v)
{
    activityState = ACTIVITY_READY;
}

void AppActivityScreenPresenter::activate()
{
    model->SignalRequestFromPresenter();
}

void AppActivityScreenPresenter::deactivate()
{

}

void AppActivityScreenPresenter::StartStopActivity(void)
{
    if(ACTIVITY_READY == activityState)
    {
        activityState = ACTIVITY_RUNNING;
    }
    else if(ACTIVITY_PAUSED == activityState)
    {
        activityState = ACTIVITY_RUNNING;
    }
    else if(ACTIVITY_RUNNING == activityState)
    {
        activityState = ACTIVITY_PAUSED;
    }
}

void AppActivityScreenPresenter::FinishActivity(void)
{
    activityState = ACTIVITY_FINISHED;
}

/* Collecting signals from Model:
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
    latitude = newLatitude;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_longitude(float newLongitude)
{
    longitude = newLongitude;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_altitude(float newAltitude)
{
    altitude = newAltitude;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_time(uint32_t newTime)
{
    time = newTime;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_date(uint32_t newDate)
{
    date = newDate;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    fixQuality = newFixQuality;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum)
{
    satellitesNum = newSatellitesNum;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_lonDir(char newLonDir)
{
    lonDir = newLonDir;
}
void AppActivityScreenPresenter::NotifySignalChanged_gpsData_latDir(char newLatDir)
{
    latDir = newLatDir;
}