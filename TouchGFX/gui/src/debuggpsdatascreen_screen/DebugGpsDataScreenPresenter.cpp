#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenView.hpp>
#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenPresenter.hpp>

DebugGpsDataScreenPresenter::DebugGpsDataScreenPresenter(DebugGpsDataScreenView& v)
    : view(v)
{

}

void DebugGpsDataScreenPresenter::activate()
{

}

void DebugGpsDataScreenPresenter::deactivate()
{

}

void DebugGpsDataScreenPresenter::tickDebugScreen()
{
    view.tickDebugScreen();
}