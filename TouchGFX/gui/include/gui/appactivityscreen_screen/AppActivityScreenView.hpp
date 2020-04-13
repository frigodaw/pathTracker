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
protected:
};

#endif // APPACTIVITYSCREENVIEW_HPP
