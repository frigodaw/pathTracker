/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/appactivityscreen_screen/AppActivityScreenViewBase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"

AppActivityScreenViewBase::AppActivityScreenViewBase() :
    buttonCallback(this, &AppActivityScreenViewBase::buttonCallbackHandler)
{

    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

    BackgroundCC.setXY(0, 0);

    ActivityDataCC_Map.setXY(0, 35);
    ActivityDataCC_Map.setVisible(false);

    ActivityDataCC_Alti.setXY(0, 35);
    ActivityDataCC_Alti.setVisible(false);

    ActivityDataCC_Main.setXY(0, 35);
    ActivityDataCC_Main.setVisible(false);

    ActivityDataCC_MapSelector.setXY(0, 35);

    NameTextBox.setPosition(30, 5, 175, 25);
    NameTextBox.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    NameTextBox.setLinespacing(0);
    Unicode::snprintf(NameTextBoxBuffer, NAMETEXTBOX_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID261).getText());
    NameTextBox.setWildcard(NameTextBoxBuffer);
    NameTextBox.setTypedText(touchgfx::TypedText(T_SINGLEUSEID97));

    ButtonForLabel.setXY(90, 260);
    ButtonForLabel.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_PLAY_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_PLAY_32_ID));
    ButtonForLabel.setIconXY(20, 15);

    NextButton.setXY(180, 260);
    NextButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_NEXT_ARROW_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_NEXT_ARROW_32_ID));
    NextButton.setIconXY(22, 15);
    NextButton.setAction(buttonCallback);

    ZoomOutButton.setXY(90, 260);
    ZoomOutButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_ZOOMOUT_ID), touchgfx::Bitmap(BITMAP_ZOOMOUT_ID));
    ZoomOutButton.setIconXY(16, 16);
    ZoomOutButton.setAction(buttonCallback);

    StartStopButton.setXY(90, 260);
    StartStopButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_PAUSE_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_PAUSE_32_ID));
    StartStopButton.setIconXY(18, 15);
    StartStopButton.setAction(buttonCallback);

    ZoomInButton.setXY(0, 260);
    ZoomInButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_ZOOMIN_ID), touchgfx::Bitmap(BITMAP_ZOOMIN_ID));
    ZoomInButton.setIconXY(16, 16);
    ZoomInButton.setAction(buttonCallback);

    FinishButton.setXY(0, 260);
    FinishButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_STOP_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_STOP_32_ID));
    FinishButton.setIconXY(15, 15);
    FinishButton.setAction(buttonCallback);

    PrevMapButton.setXY(0, 260);
    PrevMapButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_BACK_ARROW_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_BACK_ARROW_32_ID));
    PrevMapButton.setIconXY(22, 15);
    PrevMapButton.setAction(buttonCallback);

    ConfirmMapButton.setXY(90, 260);
    ConfirmMapButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_DONE_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_DONE_32_ID));
    ConfirmMapButton.setIconXY(15, 15);
    ConfirmMapButton.setAction(buttonCallback);

    NextMapButton.setXY(180, 260);
    NextMapButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_NEXT_ARROW_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_NEXT_ARROW_32_ID));
    NextMapButton.setIconXY(22, 15);
    NextMapButton.setAction(buttonCallback);

    FixImage.setBitmap(touchgfx::Bitmap(BITMAP_FIXICON_ID));
    FixImage.setPosition(210, 5, 25, 25);
    FixImage.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);

    SdCardImage.setBitmap(touchgfx::Bitmap(BITMAP_SDCARDICON_ID));
    SdCardImage.setPosition(5, 5, 19, 25);
    SdCardImage.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);

    add(BackgroundCC);
    add(ActivityDataCC_Map);
    add(ActivityDataCC_Alti);
    add(ActivityDataCC_Main);
    add(ActivityDataCC_MapSelector);
    add(NameTextBox);
    add(ButtonForLabel);
    add(NextButton);
    add(ZoomOutButton);
    add(StartStopButton);
    add(ZoomInButton);
    add(FinishButton);
    add(PrevMapButton);
    add(ConfirmMapButton);
    add(NextMapButton);
    add(FixImage);
    add(SdCardImage);
}

void AppActivityScreenViewBase::setupScreen()
{
    BackgroundCC.initialize();
    ActivityDataCC_Map.initialize();
    ActivityDataCC_Alti.initialize();
    ActivityDataCC_Main.initialize();
    ActivityDataCC_MapSelector.initialize();
}

void AppActivityScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &NextButton)
    {
        //ChangeActivityDataCC
        //When NextButton clicked call virtual function
        //Call ChangeActivityDataCC
        ChangeActivityDataCC();
    }
    else if (&src == &ZoomOutButton)
    {
        //ZoomOutMap
        //When ZoomOutButton clicked call virtual function
        //Call ZoomOut
        ZoomOut();
    }
    else if (&src == &StartStopButton)
    {
        //StartStopActivity
        //When StartStopButton clicked call virtual function
        //Call StartStopActivity
        StartStopActivity();
    }
    else if (&src == &ZoomInButton)
    {
        //ZoomInMap
        //When ZoomInButton clicked call virtual function
        //Call ZoomIn
        ZoomIn();
    }
    else if (&src == &FinishButton)
    {
        //FinishActivity
        //When FinishButton clicked call virtual function
        //Call FinishActivity
        FinishActivity();

        //GoToAppMenuScreen
        //When FinishActivity completed change screen to AppMenuScreen
        //Go to AppMenuScreen with no screen transition
        application().gotoAppMenuScreenScreenNoTransition();
    }
    else if (&src == &PrevMapButton)
    {
        //DisplayPreviousMap
        //When PrevMapButton clicked call virtual function
        //Call DisplayPreviousMap
        DisplayPreviousMap();
    }
    else if (&src == &ConfirmMapButton)
    {
        //ConfirmMapSelection
        //When ConfirmMapButton clicked call virtual function
        //Call ConfirmMapSelection
        ConfirmMapSelection();
    }
    else if (&src == &NextMapButton)
    {
        //DisplayNextMap
        //When NextMapButton clicked call virtual function
        //Call DisplayNextMap
        DisplayNextMap();
    }
}
