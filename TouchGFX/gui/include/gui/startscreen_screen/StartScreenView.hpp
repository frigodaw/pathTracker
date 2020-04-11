#ifndef STARTSCREENVIEW_HPP
#define STARTSCREENVIEW_HPP

#include <gui_generated/startscreen_screen/StartScreenViewBase.hpp>
#include <gui/startscreen_screen/StartScreenPresenter.hpp>

class StartScreenView : public StartScreenViewBase
{
public:
    StartScreenView();
    ~StartScreenView() {}
    void setupScreen();
    void tearDownScreen();
protected:
};

#endif // STARTSCREENVIEW_HPP
