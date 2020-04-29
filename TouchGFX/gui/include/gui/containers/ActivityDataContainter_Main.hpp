#ifndef ACTIVITYDATACONTAINTER_MAIN_HPP
#define ACTIVITYDATACONTAINTER_MAIN_HPP

#include <gui_generated/containers/ActivityDataContainter_MainBase.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>

#define ACTIVTYCONTAINER_TIMER_COEFF1       3600u
#define ACTIVTYCONTAINER_TIMER_COEFF2       60u
#define ACTIVTYCONTAINER_TIMER_COEFF3       APP_MAX_CALL_COUNTER
#define ACTIVTYCONTAINER_TIME_COEFF1        100u
#define ACTIVTYCONTAINER_TIME_COEFF2        10000u

class ActivityDataContainter_Main : public ActivityDataContainter_MainBase
{
public:
    ActivityDataContainter_Main();
    ~ActivityDataContainter_Main() {}
    void initialize();

    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_activityData_timer(uint32_t newTimer);
    void NotifySignalChanged_activityData_distance(float newDistance);
    void NotifySignalChanged_activityData_time(uint32_t newTime);
    void NotifySignalChanged_activityData_speed(float newSpeed);
    void NotifySignalChanged_activityData_avgSpeed(float newAvgSpeed);
    void NotifySignalChanged_activityData_maxSpeed(float newMaxSpeed);
protected:
};

#endif // ACTIVITYDATACONTAINTER_MAIN_HPP
