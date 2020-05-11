#include <gui/debugdrawscreen_screen/DebugDrawScreenView.hpp>
#include <gui/debugdrawscreen_screen/DebugDrawScreenPresenter.hpp>

DebugDrawScreenPresenter::DebugDrawScreenPresenter(DebugDrawScreenView& v)
    : view(v)
{

}

void DebugDrawScreenPresenter::activate()
{
    model->MainPeriodFromPresenter(DEBUGDRAWSCREEN_MAIN_INTERVAL);
}

void DebugDrawScreenPresenter::deactivate()
{

}

void DebugDrawScreenPresenter::Main(void)
{
    view.ResizeLine();
}