#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>

AppActivityScreenView::AppActivityScreenView()
{

}

void AppActivityScreenView::setupScreen()
{
    AppActivityScreenViewBase::setupScreen();
}

void AppActivityScreenView::tearDownScreen()
{
    AppActivityScreenViewBase::tearDownScreen();
}

void AppActivityScreenView::StartStopActivity(void)
{
    presenter->StartStopActivity();
}

void AppActivityScreenView::FinishActivity(void)
{
    presenter->FinishActivity();
}