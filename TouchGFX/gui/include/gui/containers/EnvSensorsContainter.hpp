#ifndef ENVSENSORSCONTAINTER_HPP
#define ENVSENSORSCONTAINTER_HPP

#include <gui_generated/containers/EnvSensorsContainterBase.hpp>

#define ENVSENSORSCONTAINER_PA_TO_HPA       100.f

class EnvSensorsContainter : public EnvSensorsContainterBase
{
public:
    EnvSensorsContainter();
    ~EnvSensorsContainter() {}
    void initialize();

    void NotifySignalChanged_sensorData_altitude(float newAltitude);
    void NotifySignalChanged_sensorData_pressure(float newPressure);
    void NotifySignalChanged_sensorData_temperature(float newTemperature);
protected:
};

#endif // ENVSENSORSCONTAINTER_HPP
