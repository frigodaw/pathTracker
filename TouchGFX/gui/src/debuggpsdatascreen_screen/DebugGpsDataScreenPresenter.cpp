#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenView.hpp>
#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenPresenter.hpp>

DebugGpsDataScreenPresenter::DebugGpsDataScreenPresenter(DebugGpsDataScreenView& v)
    : view(v)
{

}


void DebugGpsDataScreenPresenter::activate()
{
    model->SignalRequestFromPresenter();
}


void DebugGpsDataScreenPresenter::deactivate()
{

}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_latitude(float newLatitude)
{
    view.NotifySignalChanged_gpsData_latitude(newLatitude);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_longitude(float newLongitude)
{
    view.NotifySignalChanged_gpsData_longitude(newLongitude);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_altitude(float newAltitude)
{
    view.NotifySignalChanged_gpsData_altitude(newAltitude);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_time(uint32_t newTime)
{
    view.NotifySignalChanged_gpsData_time(newTime);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_date(uint32_t newDate)
{
    view.NotifySignalChanged_gpsData_date(newDate);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_read(uint16_t newRead)
{
    view.NotifySignalChanged_gpsData_read(newRead);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_write(uint16_t newWrite)
{
    view.NotifySignalChanged_gpsData_write(newWrite);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    view.NotifySignalChanged_gpsData_fixQuality(newFixQuality);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum)
{
    view.NotifySignalChanged_gpsData_satellitesNum(newSatellitesNum);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_lonDir(char newLonDir)
{
    view.NotifySignalChanged_gpsData_lonDir(newLonDir);
}


void DebugGpsDataScreenPresenter::NotifySignalChanged_gpsData_latDir(char newLatDir)
{
    view.NotifySignalChanged_gpsData_latDir(newLatDir);
}