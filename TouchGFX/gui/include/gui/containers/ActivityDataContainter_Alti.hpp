#ifndef ACTIVITYDATACONTAINTER_ALTI_HPP
#define ACTIVITYDATACONTAINTER_ALTI_HPP

#include <gui_generated/containers/ActivityDataContainter_AltiBase.hpp>

class ActivityDataContainter_Alti : public ActivityDataContainter_AltiBase
{
public:
    ActivityDataContainter_Alti();
    ~ActivityDataContainter_Alti() {}
    void initialize();

    void NotifySignalChanged_activityData_altitude(float newAltitude);
    void NotifySignalChanged_activityData_slope(float newSlope);
    void NotifySignalChanged_activityData_altiUp(int32_t newAltiUp);
    void NotifySignalChanged_activityData_altiDown(int32_t newAltiDown);
    void NotifySignalChanged_activityData_altiMax(int32_t newAltiMax);
    void NotifySignalChanged_activityData_slopeMax(float newSlopeMax);
    void NotifySignalChanged_sensorData_altitude(float newAltitude);
protected:
};

#endif // ACTIVITYDATACONTAINTER_ALTI_HPP
