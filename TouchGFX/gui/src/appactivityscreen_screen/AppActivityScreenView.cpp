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

void AppActivityScreenView::ZoomIn(void)
{
    presenter->ZoomIn();
}

void AppActivityScreenView::ZoomOut(void)
{
    presenter->ZoomOut();
}

void AppActivityScreenView::ChangeActivityDataCC(void)
{
    presenter->ChangeActivityDataCC();
}

void AppActivityScreenView::SetActivityDataScreen(AppActivity_activeScreen_T screen)
{
    ActivityDataCC_MapSelector.setVisible(false);
    ActivityDataCC_Main.setVisible(false);
    ActivityDataCC_Alti.setVisible(false);
    ActivityDataCC_Map.setVisible(false);
    PrevMapButton.setVisible(false);
    ConfirmMapButton.setVisible(false);
    NextMapButton.setVisible(false);
    StartStopButton.setVisible(false);
    FinishButton.setVisible(false);
    ZoomInButton.setVisible(false);
    ZoomOutButton.setVisible(false);

    switch (screen)
    {
        case APP_SCREEN_NONE:
            break;
        case APP_SCREEN_SELECTMAP:
            ActivityDataCC_MapSelector.setVisible(true);
            ActivityDataCC_Map.setVisible(true);
            PrevMapButton.setVisible(true);
            ConfirmMapButton.setVisible(true);
            NextMapButton.setVisible(true);
            break;
        case APP_SCREEN_MAIN:
            ActivityDataCC_Main.setVisible(true);
            StartStopButton.setVisible(true);
            FinishButton.setVisible(true);
            break;
        case APP_SCREEN_ALTI:
            ActivityDataCC_Alti.setVisible(true);
            StartStopButton.setVisible(true);
            FinishButton.setVisible(true);
            break;
        case APP_SCREEN_MAP:
            ActivityDataCC_Map.setVisible(true);
            ZoomInButton.setVisible(true);
            ZoomOutButton.setVisible(true);
            presenter->DrawRoute(APP_DRAWROUTE_MAP);
            presenter->DrawRoute(APP_DRAWROUTE_TRACK);
            break;
        default:
            ActivityDataCC_Main.setVisible(true);
            StartStopButton.setVisible(true);
            FinishButton.setVisible(true);
            break;
    }

    ActivityDataCC_MapSelector.invalidate();
    ActivityDataCC_Main.invalidate();
    ActivityDataCC_Alti.invalidate();
    ActivityDataCC_Map.invalidate();
    PrevMapButton.invalidate();
    ConfirmMapButton.invalidate();
    NextMapButton.invalidate();
    StartStopButton.invalidate();
    FinishButton.invalidate();
    ZoomInButton.invalidate();
    ZoomOutButton.invalidate();
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


void AppActivityScreenView::ShowSdCard(bool isSdCard)
{
    if(true == isSdCard)
    {
        SdCardImage.setAlpha(APP_ALPHA_FULLVISIBLE);
    }
    else
    {
        SdCardImage.setAlpha(APP_ALPHA_INVISIBLE);
    }
    SdCardImage.invalidate();
}


void AppActivityScreenView::FlushRouteList(void)
{
    ActivityDataCC_Map.FlushRouteList();
}


void AppActivityScreenView::SetRouteScale(uint32_t scaleVal)
{
    ActivityDataCC_Map.SetRouteScale(scaleVal);
}


bool AppActivityScreenView::AddCoordsToRouteList(AppActivity_coordinatesXY_T coordsXY, AppActivity_drawRoute_T route)
{
    bool newPoint = ActivityDataCC_Map.AddCoordsToRouteList(coordsXY.X, coordsXY.Y, (Map_DrawRoute_T)route);
    ActivityDataCC_Map.invalidate();
    return newPoint;
}

void AppActivityScreenView::TrackRedraw(void)
{
    ActivityDataCC_Map.TrackRedraw();
}


void AppActivityScreenView::IncrementRouteIdx(AppActivity_drawRoute_T route, uint8_t addedPoints)
{
    ActivityDataCC_Map.IncrementRouteIdx((Map_DrawRoute_T)route, addedPoints);
}

void AppActivityScreenView::ConfirmMapSelection(void)
{
    presenter->ConfirmMapSelection();
}

void AppActivityScreenView::DisplayPreviousMap(void)
{
    presenter->DisplayPreviousMap();
}

void AppActivityScreenView::DisplayNextMap(void)
{
    presenter->DisplayNextMap();
}

void AppActivityScreenView::ChangeMapDescription(uint8_t *fileName, uint8_t nameLen, float distance)
{
    Unicode::UnicodeChar fileNameUnicode[NAMETEXTBOX_SIZE] = {0u};
    Unicode::fromUTF8((const uint8_t*)fileName, fileNameUnicode, NAMETEXTBOX_SIZE);
    Unicode::snprintf(NameTextBoxBuffer, NAMETEXTBOX_SIZE, "%s", fileNameUnicode);
    //NameTextBox.resizeToCurrentText();
    NameTextBox.invalidate();

    ActivityDataCC_MapSelector.ChangeMapDistance(distance);
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
