#include <gui/containers/ActivityDataContainter.hpp>

ActivityDataContainter::ActivityDataContainter()
{

}

void ActivityDataContainter::initialize()
{
    ActivityDataContainterBase::initialize();
}

void ActivityDataContainter::NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    Unicode::snprintf(FixTextBoxBuffer, FIXTEXTBOX_SIZE, "%d", newFixQuality);
    FixTextBox.invalidate();
}

void ActivityDataContainter::NotifySignalChanged_activityData_timer(uint32_t newTimer)
{
    Unicode::snprintf(TimerTextBoxBuffer, TIMERTEXTBOX_SIZE, "%d", newTimer);
    TimerTextBox.invalidate();
}