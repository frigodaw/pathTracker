#include <gui/containers/ActivityDataContainter_Alti.hpp>

ActivityDataContainter_Alti::ActivityDataContainter_Alti()
{

}

void ActivityDataContainter_Alti::initialize()
{
    ActivityDataContainter_AltiBase::initialize();
}

void ActivityDataContainter_Alti::NotifySignalChanged_activityData_altitude(float newAltitude)
{
    Unicode::snprintfFloats(AltitudeTextBoxBuffer, ALTITUDETEXTBOX_SIZE, "%#.1f", &newAltitude);
    AltitudeTextBox.invalidate();
}

void ActivityDataContainter_Alti::NotifySignalChanged_activityData_slope(float newSlope)
{
    Unicode::snprintfFloats(SlopeTextBoxBuffer, SLOPETEXTBOX_SIZE, "%#.1f", &newSlope);
    SlopeTextBox.invalidate();
}

void ActivityDataContainter_Alti::NotifySignalChanged_activityData_altiUp(int32_t newAltiUp)
{
    Unicode::snprintf(AltiUpTextBoxBuffer, ALTIUPTEXTBOX_SIZE, "%d", newAltiUp);
    AltiUpTextBox.invalidate();
}

void ActivityDataContainter_Alti::NotifySignalChanged_activityData_altiDown(int32_t newAltiDown)
{
    Unicode::snprintf(AltiDownTextBoxBuffer, ALTIDOWNTEXTBOX_SIZE, "%d", newAltiDown);
    AltiDownTextBox.invalidate();
}

void ActivityDataContainter_Alti::NotifySignalChanged_activityData_altiMax(int32_t newAltiMax)
{
    Unicode::snprintf(AltiMaxTextBoxBuffer, ALTIMAXTEXTBOX_SIZE, "%d", newAltiMax);
    AltiMaxTextBox.invalidate();
}

void ActivityDataContainter_Alti::NotifySignalChanged_activityData_slopeMax(float newSlopeMax)
{
    Unicode::snprintfFloats(SlopeMaxTextBoxBuffer, SLOPEMAXTEXTBOX_SIZE, "%#.1f", &newSlopeMax);
    SlopeMaxTextBox.invalidate();
}

void ActivityDataContainter_Alti::NotifySignalChanged_sensorData_altitude(float newAltitude)
{
    Unicode::snprintfFloats(AltitudeTextBoxBuffer, ALTITUDETEXTBOX_SIZE, "%#.1f", &newAltitude);
    AltitudeTextBox.invalidate();
}