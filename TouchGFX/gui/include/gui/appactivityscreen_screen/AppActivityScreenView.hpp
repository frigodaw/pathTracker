#ifndef APPACTIVITYSCREENVIEW_HPP
#define APPACTIVITYSCREENVIEW_HPP

#include <gui_generated/appactivityscreen_screen/AppActivityScreenViewBase.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>

#define APP_ALPHA_FULLVISIBLE           255u
#define APP_ALPHA_INVISIBLE             0u

class AppActivityScreenView : public AppActivityScreenViewBase
{
public:
    AppActivityScreenView();
    ~AppActivityScreenView() {}
    void setupScreen();
    void tearDownScreen();

    void StartStopActivity(void);
    void FinishActivity(void);
    void SetBitmapButton(const uint16_t bitmapId);
    void ShowFixImage(bool isFix);

    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_activityData_timer(uint32_t newTimer);
    void NotifySignalChanged_activityData_distance(float newDistance);
    void NotifySignalChanged_activityData_time(uint32_t newTime);
    void NotifySignalChanged_activityData_speed(float newSpeed);
    void NotifySignalChanged_activityData_avgSpeed(float newAvgSpeed);
    void NotifySignalChanged_activityData_maxSpeed(float newMaxSpeed);
protected:
};

#endif // APPACTIVITYSCREENVIEW_HPP
