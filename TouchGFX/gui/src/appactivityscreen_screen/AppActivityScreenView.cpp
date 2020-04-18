#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include "BitmapDatabase.hpp"


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

void AppActivityScreenView::SetBitmapButton(const uint16_t bitmapId)
{
    StartStopButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(bitmapId), touchgfx::Bitmap(bitmapId));
}

void AppActivityScreenView::NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    ActivityDataCC.NotifySignalChanged_gpsData_fixQuality(newFixQuality);
}

void AppActivityScreenView::NotifySignalChanged_activityData_timer(uint32_t newTimer)
{
    ActivityDataCC.NotifySignalChanged_activityData_timer(newTimer);
}