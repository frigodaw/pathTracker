#include <gui/debugenvsensorsscreen_screen/DebugEnvSensorsScreenView.hpp>
#include <gui/debugenvsensorsscreen_screen/DebugEnvSensorsScreenPresenter.hpp>

DebugEnvSensorsScreenPresenter::DebugEnvSensorsScreenPresenter(DebugEnvSensorsScreenView& v)
    : view(v)
{

}

void DebugEnvSensorsScreenPresenter::activate()
{
    model->SignalRequestFromPresenter();
}

void DebugEnvSensorsScreenPresenter::deactivate()
{

}

void DebugEnvSensorsScreenPresenter::NotifySignalChanged_sensorData_altitude(float newAltitude)
{
    view.NotifySignalChanged_sensorData_altitude(newAltitude);
}

void DebugEnvSensorsScreenPresenter::NotifySignalChanged_sensorData_pressure(float newPressure)
{
    view.NotifySignalChanged_sensorData_pressure(newPressure);
}

void DebugEnvSensorsScreenPresenter::NotifySignalChanged_sensorData_temperature(float newTemperature)
{
    view.NotifySignalChanged_sensorData_temperature(newTemperature);
}