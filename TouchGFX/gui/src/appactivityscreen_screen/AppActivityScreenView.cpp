#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include "BitmapDatabase.hpp"


AppActivityScreenView::AppActivityScreenView()
{
    Map.setXY(0, 35);
    Map.setVisible(false);
    Map.setWidth(240);
    Map.setHeight(225);
    Map.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Map.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    Map.setBorderSize(2);

    add(Map);
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

void AppActivityScreenView::ChangeActivityDataCC(void)
{
    presenter->ChangeActivityDataCC();
}

void AppActivityScreenView::SetActivityDataScreen(AppActivity_activeScreen_T screen)
{
    ActivityDataCC_Main.setVisible(false);
    ActivityDataCC_Alti.setVisible(false);
    ActivityDataCC_Map.setVisible(false);
    Map.setVisible(false);

    switch (screen)
    {
        case APP_SCREEN_MAIN:
            ActivityDataCC_Main.setVisible(true);
            break;
        case APP_SCREEN_ALTI:
            ActivityDataCC_Alti.setVisible(true);
            break;
        case APP_SCREEN_MAP:
            //ActivityDataCC_Map.setVisible(true);
            Map.setVisible(true);
            break;
        default:
            ActivityDataCC_Main.setVisible(true);
            break;
    }

    ActivityDataCC_Main.invalidate();
    ActivityDataCC_Alti.invalidate();
    ActivityDataCC_Map.invalidate();
    Map.invalidate();
}

void AppActivityScreenView::SetBitmapButton(const uint16_t bitmapId)
{
    StartStopButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(bitmapId), touchgfx::Bitmap(bitmapId));
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


void AppActivityScreenView::DrawLine(AppActivity_coordinatesXY_T coordsXY)
{
    Map.drawPixel(coordsXY.X, coordsXY.Y);
    Map.invalidate();
}


void AppActivityScreenView::NotifySignalChanged_activityData_timer(uint32_t newTimer)
{
    ActivityDataCC_Main.NotifySignalChanged_activityData_timer(newTimer);
}
void AppActivityScreenView::NotifySignalChanged_activityData_distance(float newDistance)
{
    ActivityDataCC_Main.NotifySignalChanged_activityData_distance(newDistance);
}
void AppActivityScreenView::NotifySignalChanged_activityData_time(uint32_t newTime)
{
    ActivityDataCC_Main.NotifySignalChanged_activityData_time(newTime);
}
void AppActivityScreenView::NotifySignalChanged_activityData_speed(float newSpeed)
{
    ActivityDataCC_Main.NotifySignalChanged_activityData_speed(newSpeed);
}
void AppActivityScreenView::NotifySignalChanged_activityData_avgSpeed(float newAvgSpeed)
{
    ActivityDataCC_Main.NotifySignalChanged_activityData_avgSpeed(newAvgSpeed);
}
void AppActivityScreenView::NotifySignalChanged_activityData_maxSpeed(float newMaxSpeed)
{
    ActivityDataCC_Main.NotifySignalChanged_activityData_maxSpeed(newMaxSpeed);
}
void AppActivityScreenView::NotifySignalChanged_activityData_altitude(float newAltitude)
{
    ActivityDataCC_Alti.NotifySignalChanged_activityData_altitude(newAltitude);
}
void AppActivityScreenView::NotifySignalChanged_activityData_slope(float newSlope)
{
    ActivityDataCC_Alti.NotifySignalChanged_activityData_slope(newSlope);
}
void AppActivityScreenView::NotifySignalChanged_activityData_altiUp(int32_t newAltiUp)
{
    ActivityDataCC_Alti.NotifySignalChanged_activityData_altiUp(newAltiUp);
}
void AppActivityScreenView::NotifySignalChanged_activityData_altiDown(int32_t newAltiDown)
{
    ActivityDataCC_Alti.NotifySignalChanged_activityData_altiDown(newAltiDown);
}
void AppActivityScreenView::NotifySignalChanged_activityData_altiMax(int32_t newAltiMax)
{
    ActivityDataCC_Alti.NotifySignalChanged_activityData_altiMax(newAltiMax);
}
void AppActivityScreenView::NotifySignalChanged_activityData_slopeMax(float newSlopeMax)
{
    ActivityDataCC_Alti.NotifySignalChanged_activityData_slopeMax(newSlopeMax);
}
void AppActivityScreenView::NotifySignalChanged_sensorData_altitude(float newAltitude)
{
    ActivityDataCC_Alti.NotifySignalChanged_sensorData_altitude(newAltitude);
}
