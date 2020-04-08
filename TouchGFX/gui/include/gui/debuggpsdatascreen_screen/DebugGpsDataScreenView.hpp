#ifndef DEBUGGPSDATASCREENVIEW_HPP
#define DEBUGGPSDATASCREENVIEW_HPP

#include <gui_generated/debuggpsdatascreen_screen/DebugGpsDataScreenViewBase.hpp>
#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenPresenter.hpp>

class DebugGpsDataScreenView : public DebugGpsDataScreenViewBase
{
public:
    DebugGpsDataScreenView();
    virtual ~DebugGpsDataScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void NotifySignalChanged_gpsData_latitude(float newLatitude);
    virtual void NotifySignalChanged_gpsData_longitude(float newLongitude);
    virtual void NotifySignalChanged_gpsData_altitude(float newAltitude);
    virtual void NotifySignalChanged_gpsData_time(uint32_t newTime);
    virtual void NotifySignalChanged_gpsData_date(uint32_t newDate);
    virtual void NotifySignalChanged_gpsData_read(uint16_t newRead);
    virtual void NotifySignalChanged_gpsData_write(uint16_t newWrite);
    virtual void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    virtual void NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum);
    virtual void NotifySignalChanged_gpsData_lonDir(char newLonDir);
    virtual void NotifySignalChanged_gpsData_latDir(char newLatDir);
protected:
};

#endif // DEBUGGPSDATASCREENVIEW_HPP
