#include <gui/containers/EnvSensorsContainter.hpp>

EnvSensorsContainter::EnvSensorsContainter()
{

}

void EnvSensorsContainter::initialize()
{
    EnvSensorsContainterBase::initialize();
}

void EnvSensorsContainter::NotifySignalChanged_sensorData_altitude(float newAltitude)
{
    Unicode::snprintfFloats(AltitudeTextBoxBuffer, ALTITUDETEXTBOX_SIZE, "%#.2f", &newAltitude);
    AltitudeTextBox.invalidate();
}

void EnvSensorsContainter::NotifySignalChanged_sensorData_pressure(float newPressure)
{
    newPressure /= ENVSENSORSCONTAINER_PA_TO_HPA;
    Unicode::snprintfFloats(PressureTextBoxBuffer, PRESSURETEXTBOX_SIZE, "%#.2f", &newPressure);
    PressureTextBox.invalidate();
}

void EnvSensorsContainter::NotifySignalChanged_sensorData_temperature(float newTemperature)
{
    Unicode::snprintfFloats(TemperatureTextBoxBuffer, TEMPERATURETEXTBOX_SIZE, "%#.2f", &newTemperature);
    TemperatureTextBox.invalidate();
}
