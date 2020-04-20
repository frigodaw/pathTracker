#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include "BitmapDatabase.hpp"


AppActivityScreenView::AppActivityScreenView()
{

}

void AppActivityScreenView::setupScreen()
{
    AppActivityScreenViewBase::setupScreen();
    ShowFixImage(false);
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


void AppActivityScreenView::NotifySignalChanged_activityData_timer(uint32_t newTimer)
{
    ActivityDataCC.NotifySignalChanged_activityData_timer(newTimer);
}

void AppActivityScreenView::ShowFixImage(bool isFix)
{
    if(true == isFix)
    {
        FixImage.setAlpha(APP_ALPHA_FULLVISIBLE);
    }
    else
    {
        FixImage.setAlpha(APP_ALPHA_INVISIBLE);
    }
    FixImage.invalidate();
}