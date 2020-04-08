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


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_latitude(float newLatitude)
{
    GpsDataCC.NotifySignalChanged_gpsData_latitude(newLatitude);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_longitude(float newLongitude)
{
    GpsDataCC.NotifySignalChanged_gpsData_longitude(newLongitude);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_altitude(float newAltitude)
{
    GpsDataCC.NotifySignalChanged_gpsData_altitude(newAltitude);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_time(uint32_t newTime)
{
    GpsDataCC.NotifySignalChanged_gpsData_time(newTime);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_date(uint32_t newDate)
{
    GpsDataCC.NotifySignalChanged_gpsData_date(newDate);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_read(uint16_t newRead)
{
    GpsDataCC.NotifySignalChanged_gpsData_read(newRead);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_write(uint16_t newWrite)
{
    GpsDataCC.NotifySignalChanged_gpsData_write(newWrite);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    GpsDataCC.NotifySignalChanged_gpsData_fixQuality(newFixQuality);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum)
{
    GpsDataCC.NotifySignalChanged_gpsData_satellitesNum(newSatellitesNum);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_lonDir(char newLonDir)
{
    GpsDataCC.NotifySignalChanged_gpsData_lonDir(newLonDir);
}


void DebugGpsDataScreenView::NotifySignalChanged_gpsData_latDir(char newLatDir)
{
    GpsDataCC.NotifySignalChanged_gpsData_latDir(newLatDir);
}
