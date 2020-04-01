#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenView.hpp>

DebugGpsDataScreenView::DebugGpsDataScreenView()
{

}

void DebugGpsDataScreenView::setupScreen()
{
    DebugGpsDataScreenViewBase::setupScreen();
}

void DebugGpsDataScreenView::tearDownScreen()
{
    DebugGpsDataScreenViewBase::tearDownScreen();
}

void DebugGpsDataScreenView::tickDebugScreen()
{
    GpsDataCC.updateGpsDataContainer();
}