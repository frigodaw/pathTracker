#ifndef DEBUGDRAWSCREENVIEW_HPP
#define DEBUGDRAWSCREENVIEW_HPP

#include <gui_generated/debugdrawscreen_screen/DebugDrawScreenViewBase.hpp>
#include <gui/debugdrawscreen_screen/DebugDrawScreenPresenter.hpp>

class DebugDrawScreenView : public DebugDrawScreenViewBase
{
public:
    DebugDrawScreenView();
    virtual ~DebugDrawScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // DEBUGDRAWSCREENVIEW_HPP
