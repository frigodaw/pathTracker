#ifndef DEBUGGPSDATASCREENVIEW_HPP
#define DEBUGGPSDATASCREENVIEW_HPP

#include <gui_generated/debuggpsdatascreen_screen/DebugGpsDataScreenViewBase.hpp>
#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenPresenter.hpp>

class DebugGpsDataScreenView : public DebugGpsDataScreenViewBase
{
public:
    DebugGpsDataScreenView();
    ~DebugGpsDataScreenView() {}
    void setupScreen();
    void tearDownScreen();

    void NotifySignalChanged_gpsData_latitude(float newLatitude);
    void NotifySignalChanged_gpsData_longitude(float newLongitude);
    void NotifySignalChanged_gpsData_altitude(float newAltitude);
    void NotifySignalChanged_gpsData_time(uint32_t newTime);
    void NotifySignalChanged_gpsData_date(uint32_t newDate);
    void NotifySignalChanged_gpsData_read(uint16_t newRead);
    void NotifySignalChanged_gpsData_write(uint16_t newWrite);
    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum);
    void NotifySignalChanged_gpsData_lonDir(char newLonDir);
    void NotifySignalChanged_gpsData_latDir(char newLatDir);
protected:
};

#endif // DEBUGGPSDATASCREENVIEW_HPP
