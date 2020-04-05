#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenView.hpp>
#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenPresenter.hpp>

DebugGpsDataScreenPresenter::DebugGpsDataScreenPresenter(DebugGpsDataScreenView& v)
    : view(v)
{

}


void DebugGpsDataScreenPresenter::activate()
{

}


void DebugGpsDataScreenPresenter::deactivate()
{

}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_latitude(float newLatitude)
{
    view.notifySignalChanged_gpsData_latitude(newLatitude);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_longitude(float newLongitude)
{
    view.notifySignalChanged_gpsData_longitude(newLongitude);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_altitude(float newAltitude)
{
    view.notifySignalChanged_gpsData_altitude(newAltitude);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_read(uint16_t newRead)
{
    view.notifySignalChanged_gpsData_read(newRead);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_write(uint16_t newWrite)
{
    view.notifySignalChanged_gpsData_write(newWrite);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_time(uint8_t newTimeHr, uint8_t newTimeMin, uint8_t newTimeSec)
{
    view.notifySignalChanged_gpsData_time(newTimeHr, newTimeMin, newTimeSec);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_date(uint8_t newDateDay, uint8_t newDateMon, uint8_t newDateYear)
{
    view.notifySignalChanged_gpsData_date(newDateDay, newDateMon, newDateYear);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    view.notifySignalChanged_gpsData_fixQuality(newFixQuality);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum)
{
    view.notifySignalChanged_gpsData_satellitesNum(newSatellitesNum);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_lonDir(char newLonDir)
{
    view.notifySignalChanged_gpsData_lonDir(newLonDir);
}


void DebugGpsDataScreenPresenter::notifySignalChanged_gpsData_latDir(char newLatDir)
{
    view.notifySignalChanged_gpsData_latDir(newLatDir);
}