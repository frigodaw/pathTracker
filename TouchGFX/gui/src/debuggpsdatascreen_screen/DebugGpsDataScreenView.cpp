#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenView.hpp>

DebugGpsDataScreenView::DebugGpsDataScreenView()
{

}


void DebugGpsDataScreenView::setupScreen()
{
    DebugGpsDataScreenViewBase::setupScreen();
}


void DebugGpsDataScreenView::tearDownScreen()
{
    DebugGpsDataScreenViewBase::tearDownScreen();
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_latitude(float newLatitude)
{
    GpsDataCC.notifySignalChanged_gpsData_latitude(newLatitude);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_longitude(float newLongitude)
{
    GpsDataCC.notifySignalChanged_gpsData_longitude(newLongitude);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_altitude(float newAltitude)
{
    GpsDataCC.notifySignalChanged_gpsData_altitude(newAltitude);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_read(uint16_t newRead)
{
    GpsDataCC.notifySignalChanged_gpsData_read(newRead);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_write(uint16_t newWrite)
{
    GpsDataCC.notifySignalChanged_gpsData_write(newWrite);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_time(uint8_t newTimeHr, uint8_t newTimeMin, uint8_t newTimeSec)
{
    GpsDataCC.notifySignalChanged_gpsData_time(newTimeHr, newTimeMin, newTimeSec);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_date(uint8_t newDateDay, uint8_t newDateMon, uint8_t newDateYear)
{
    GpsDataCC.notifySignalChanged_gpsData_date(newDateDay, newDateMon, newDateYear);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    GpsDataCC.notifySignalChanged_gpsData_fixQuality(newFixQuality);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum)
{
    GpsDataCC.notifySignalChanged_gpsData_satellitesNum(newSatellitesNum);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_lonDir(char newLonDir)
{
    GpsDataCC.notifySignalChanged_gpsData_lonDir(newLonDir);
}


void DebugGpsDataScreenView::notifySignalChanged_gpsData_latDir(char newLatDir)
{
    GpsDataCC.notifySignalChanged_gpsData_latDir(newLatDir);
}
