#ifndef APPACTIVITYSCREENVIEW_HPP
#define APPACTIVITYSCREENVIEW_HPP

#include <gui_generated/appactivityscreen_screen/AppActivityScreenViewBase.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>

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

    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_activityData_timer(uint32_t newTimer);
protected:
};

#endif // APPACTIVITYSCREENVIEW_HPP
