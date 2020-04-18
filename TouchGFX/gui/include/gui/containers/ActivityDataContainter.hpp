#ifndef ACTIVITYDATACONTAINTER_HPP
#define ACTIVITYDATACONTAINTER_HPP

#include <gui_generated/containers/ActivityDataContainterBase.hpp>

class ActivityDataContainter : public ActivityDataContainterBase
{
public:
    ActivityDataContainter();
    ~ActivityDataContainter() {}
    void initialize();

    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_activityData_timer(uint32_t newTimer);
protected:
};

#endif // ACTIVITYDATACONTAINTER_HPP
