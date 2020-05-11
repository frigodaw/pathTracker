#ifndef DEBUGDRAWSCREENPRESENTER_HPP
#define DEBUGDRAWSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

#define DEBUGDRAWSCREEN_MAIN_INTERVAL   500u

class DebugDrawScreenView;

class DebugDrawScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DebugDrawScreenPresenter(DebugDrawScreenView& v);
    ~DebugDrawScreenPresenter() {};
    void activate();
    void deactivate();

    void Main(void);

private:
    DebugDrawScreenPresenter();

    DebugDrawScreenView& view;
};

#endif // DEBUGDRAWSCREENPRESENTER_HPP
