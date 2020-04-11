#ifndef DEBUGDRAWSCREENVIEW_HPP
#define DEBUGDRAWSCREENVIEW_HPP

#include <gui_generated/debugdrawscreen_screen/DebugDrawScreenViewBase.hpp>
#include <gui/debugdrawscreen_screen/DebugDrawScreenPresenter.hpp>

class DebugDrawScreenView : public DebugDrawScreenViewBase
{
public:
    DebugDrawScreenView();
    ~DebugDrawScreenView() {}
    void setupScreen();
    void tearDownScreen();
protected:
};

#endif // DEBUGDRAWSCREENVIEW_HPP
