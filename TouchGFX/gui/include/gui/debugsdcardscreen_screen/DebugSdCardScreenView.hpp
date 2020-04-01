#ifndef DEBUGSDCARDSCREENVIEW_HPP
#define DEBUGSDCARDSCREENVIEW_HPP

#include <gui_generated/debugsdcardscreen_screen/DebugSdCardScreenViewBase.hpp>
#include <gui/debugsdcardscreen_screen/DebugSdCardScreenPresenter.hpp>

class DebugSdCardScreenView : public DebugSdCardScreenViewBase
{
public:
    DebugSdCardScreenView();
    virtual ~DebugSdCardScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // DEBUGSDCARDSCREENVIEW_HPP
