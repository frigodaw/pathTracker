#ifndef DEBUGDRAWSCREENPRESENTER_HPP
#define DEBUGDRAWSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DebugDrawScreenView;

class DebugDrawScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DebugDrawScreenPresenter(DebugDrawScreenView& v);
    ~DebugDrawScreenPresenter() {};
    void activate();
    void deactivate();

private:
    DebugDrawScreenPresenter();

    DebugDrawScreenView& view;
};

#endif // DEBUGDRAWSCREENPRESENTER_HPP
