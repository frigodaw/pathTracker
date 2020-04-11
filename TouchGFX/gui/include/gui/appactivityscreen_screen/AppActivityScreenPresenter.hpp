#ifndef APPACTIVITYSCREENPRESENTER_HPP
#define APPACTIVITYSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

typedef enum
{
    ACTIVITY_READY,
    ACTIVITY_RUNNING,
    ACTIVITY_PAUSED,
    ACTIVITY_FINISHED
}AppActivity_activityState_T;

class AppActivityScreenView;

class AppActivityScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    AppActivityScreenPresenter(AppActivityScreenView& v);
    ~AppActivityScreenPresenter() {};
    void activate();
    void deactivate();

    void StartStopActivity(void);
    void FinishActivity(void);

    void NotifySignalChanged_gpsData_latitude(float newLatitude);
    void NotifySignalChanged_gpsData_longitude(float newLongitude);
    void NotifySignalChanged_gpsData_altitude(float newAltitude);
    void NotifySignalChanged_gpsData_time(uint32_t newTime);
    void NotifySignalChanged_gpsData_date(uint32_t newDate);
    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum);
    void NotifySignalChanged_gpsData_lonDir(char newLonDir);
    void NotifySignalChanged_gpsData_latDir(char newLatDir);

private:
    AppActivityScreenPresenter();
    AppActivityScreenView& view;

    AppActivity_activityState_T activityState;

    /* gps data */
    float latitude;
    float longitude;
    float altitude;
    uint32_t time;
    uint32_t date;
    uint8_t fixQuality;
    uint8_t satellitesNum;
    char lonDir;
    char latDir;
};

#endif // APPACTIVITYSCREENPRESENTER_HPP
