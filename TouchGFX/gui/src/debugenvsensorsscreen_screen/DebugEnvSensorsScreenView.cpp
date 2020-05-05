#include <gui/debugenvsensorsscreen_screen/DebugEnvSensorsScreenView.hpp>

DebugEnvSensorsScreenView::DebugEnvSensorsScreenView()
{

}

void DebugEnvSensorsScreenView::setupScreen()
{
    DebugEnvSensorsScreenViewBase::setupScreen();
}

void DebugEnvSensorsScreenView::tearDownScreen()
{
    DebugEnvSensorsScreenViewBase::tearDownScreen();
}

void DebugEnvSensorsScreenView::NotifySignalChanged_sensorData_altitude(float newAltitude)
{
    EnvSensorsDataCC.NotifySignalChanged_sensorData_altitude(newAltitude);
}

void DebugEnvSensorsScreenView::NotifySignalChanged_sensorData_pressure(float newPressure)
{
    EnvSensorsDataCC.NotifySignalChanged_sensorData_pressure(newPressure);
}

void DebugEnvSensorsScreenView::NotifySignalChanged_sensorData_temperature(float newTemperature)
{
    EnvSensorsDataCC.NotifySignalChanged_sensorData_temperature(newTemperature);
}
