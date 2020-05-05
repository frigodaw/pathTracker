#ifndef DEBUGENVSENSORSSCREENVIEW_HPP
#define DEBUGENVSENSORSSCREENVIEW_HPP

#include <gui_generated/debugenvsensorsscreen_screen/DebugEnvSensorsScreenViewBase.hpp>
#include <gui/debugenvsensorsscreen_screen/DebugEnvSensorsScreenPresenter.hpp>

class DebugEnvSensorsScreenView : public DebugEnvSensorsScreenViewBase
{
public:
    DebugEnvSensorsScreenView();
    ~DebugEnvSensorsScreenView() {}
    void setupScreen();
    void tearDownScreen();

    void NotifySignalChanged_sensorData_altitude(float newAltitude);
    void NotifySignalChanged_sensorData_pressure(float newPressure);
    void NotifySignalChanged_sensorData_temperature(float newTemperature);
protected:
};

#endif // DEBUGENVSENSORSSCREENVIEW_HPP
