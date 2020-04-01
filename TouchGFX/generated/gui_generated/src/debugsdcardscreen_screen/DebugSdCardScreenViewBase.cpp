/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/debugsdcardscreen_screen/DebugSdCardScreenViewBase.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

DebugSdCardScreenViewBase::DebugSdCardScreenViewBase() :
    buttonCallback(this, &DebugSdCardScreenViewBase::buttonCallbackHandler)
{

    BackgroundCC.setXY(0, 0);

    NextButton.setXY(180, 260);
    NextButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    NextButton.setLabelText(touchgfx::TypedText(T_SINGLEUSEID68));
    NextButton.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    NextButton.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    NextButton.setAction(buttonCallback);

    NameLabel.setXY(45, 15);
    NameLabel.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    NameLabel.setLinespacing(0);
    NameLabel.setTypedText(touchgfx::TypedText(T_SINGLEUSEID69));

    SDCardDataCC.setXY(0, 50);

    add(BackgroundCC);
    add(NextButton);
    add(NameLabel);
    add(SDCardDataCC);
}

void DebugSdCardScreenViewBase::setupScreen()
{
    BackgroundCC.initialize();
    SDCardDataCC.initialize();
}

void DebugSdCardScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &NextButton)
    {
        //GoToDebugDrawScreen
        //When NextButton clicked change screen to DebugDrawScreen
        //Go to DebugDrawScreen with no screen transition
        application().gotoDebugDrawScreenScreenNoTransition();
    }
}
