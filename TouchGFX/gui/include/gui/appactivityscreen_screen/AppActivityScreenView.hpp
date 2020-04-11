#ifndef APPACTIVITYSCREENVIEW_HPP
#define APPACTIVITYSCREENVIEW_HPP

#include <gui_generated/appactivityscreen_screen/AppActivityScreenViewBase.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>

class AppActivityScreenView : public AppActivityScreenViewBase
{
public:
    AppActivityScreenView();
    virtual ~AppActivityScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void StartStopActivity(void);
    void FinishActivity(void);
protected:
};

#endif // APPACTIVITYSCREENVIEW_HPP
