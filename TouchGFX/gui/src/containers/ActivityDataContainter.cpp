#include <gui/containers/ActivityDataContainter.hpp>

ActivityDataContainter::ActivityDataContainter()
{

}

void ActivityDataContainter::initialize()
{
    ActivityDataContainterBase::initialize();
}

void ActivityDataContainter::NotifySignalChanged_activityData_timer(uint32_t newTimer)
{
    uint8_t timerMs  = newTimer % ACTIVTYCONTAINER_TIMER_COEFF3;
    newTimer /= ACTIVTYCONTAINER_TIMER_COEFF3;
    uint8_t timerHr  = (uint8_t)(newTimer / ACTIVTYCONTAINER_TIMER_COEFF1);
    uint8_t timerMin = (uint8_t)((newTimer - timerHr * ACTIVTYCONTAINER_TIMER_COEFF1)/(ACTIVTYCONTAINER_TIMER_COEFF2));
    uint8_t timerSec = newTimer - (timerHr * ACTIVTYCONTAINER_TIMER_COEFF1 + timerMin * ACTIVTYCONTAINER_TIMER_COEFF2);

    Unicode::snprintf(TimerTextBoxBuffer, TIMERTEXTBOX_SIZE, "%.2d:%.2d:%.2d.%.1d", timerHr, timerMin, timerSec, timerMs);
    TimerTextBox.invalidate();
}

void ActivityDataContainter::NotifySignalChanged_activityData_distance(float newDistance)
{
    Unicode::snprintfFloats(DistanceTextBoxBuffer, DISTANCETEXTBOX_SIZE, "%#.2f", &newDistance);
    DistanceTextBox.invalidate();
}

void ActivityDataContainter::NotifySignalChanged_activityData_speed(float newSpeed)
{
    Unicode::snprintfFloats(SpeedTextBoxBuffer, SPEEDTEXTBOX_SIZE, "%#.2f", &newSpeed);
    SpeedTextBox.invalidate();
}

void ActivityDataContainter::NotifySignalChanged_activityData_avgSpeed(float newAvgSpeed)
{
    Unicode::snprintfFloats(AvgSpeedTextBoxBuffer, AVGSPEEDTEXTBOX_SIZE, "%#.2f", &newAvgSpeed);
    AvgSpeedTextBox.invalidate();
}
