#ifndef ACTIVITYDATACONTAINTER_HPP
#define ACTIVITYDATACONTAINTER_HPP

#include <gui_generated/containers/ActivityDataContainterBase.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>

#define ACTIVTYCONTAINER_TIMER_COEFF1       3600u
#define ACTIVTYCONTAINER_TIMER_COEFF2       60u
#define ACTIVTYCONTAINER_TIMER_COEFF3       APP_MAX_CALL_COUNTER

class ActivityDataContainter : public ActivityDataContainterBase
{
public:
    ActivityDataContainter();
    ~ActivityDataContainter() {}
    void initialize();

    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_activityData_timer(uint32_t newTimer);
    void NotifySignalChanged_activityData_distance(float newDistance);
    void NotifySignalChanged_activityData_speed(float newSpeed);
    void NotifySignalChanged_activityData_avgSpeed(float newAvgSpeed);
protected:
};

#endif // ACTIVITYDATACONTAINTER_HPP
