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

    virtual void notifySignalChanged_gpsData_latitude(float newLatitude);
    virtual void notifySignalChanged_gpsData_longitude(float newLongitude);
    virtual void notifySignalChanged_gpsData_altitude(float newAltitude);
    virtual void notifySignalChanged_gpsData_read(uint16_t newRead);
    virtual void notifySignalChanged_gpsData_write(uint16_t newWrite);
    virtual void notifySignalChanged_gpsData_time(uint8_t newTimeHr, uint8_t newTimeMin, uint8_t newTimeSec);
    virtual void notifySignalChanged_gpsData_date(uint8_t newDateDay, uint8_t newDateMon, uint8_t newDateYear);
    virtual void notifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    virtual void notifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum);
    virtual void notifySignalChanged_gpsData_lonDir(char newLonDir);
    virtual void notifySignalChanged_gpsData_latDir(char newLatDir);
protected:
};

#endif // DEBUGGPSDATASCREENVIEW_HPP
