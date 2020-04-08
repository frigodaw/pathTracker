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

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

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

    virtual ~DebugGpsDataScreenPresenter() {};

private:
    DebugGpsDataScreenPresenter();

    DebugGpsDataScreenView& view;
};

#endif // DEBUGGPSDATASCREENPRESENTER_HPP
