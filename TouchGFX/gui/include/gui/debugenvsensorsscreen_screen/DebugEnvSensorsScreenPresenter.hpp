#ifndef DEBUGENVSENSORSSCREENPRESENTER_HPP
#define DEBUGENVSENSORSSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DebugEnvSensorsScreenView;

class DebugEnvSensorsScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DebugEnvSensorsScreenPresenter(DebugEnvSensorsScreenView& v);
    void activate();
    void deactivate();
    ~DebugEnvSensorsScreenPresenter() {};

    void NotifySignalChanged_sensorData_altitude(float newAltitude);
    void NotifySignalChanged_sensorData_pressure(float newPressure);
    void NotifySignalChanged_sensorData_temperature(float newTemperature);

private:
    DebugEnvSensorsScreenPresenter();

    DebugEnvSensorsScreenView& view;
};

#endif // DEBUGENVSENSORSSCREENPRESENTER_HPP
