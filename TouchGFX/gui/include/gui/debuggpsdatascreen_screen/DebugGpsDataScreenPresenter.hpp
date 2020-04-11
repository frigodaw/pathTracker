#ifndef DEBUGGPSDATASCREENPRESENTER_HPP
#define DEBUGGPSDATASCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DebugGpsDataScreenView;

class DebugGpsDataScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DebugGpsDataScreenPresenter(DebugGpsDataScreenView& v);
    ~DebugGpsDataScreenPresenter() {};
    void activate();
    void deactivate();

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


private:
    DebugGpsDataScreenPresenter();

    DebugGpsDataScreenView& view;
};

#endif // DEBUGGPSDATASCREENPRESENTER_HPP
