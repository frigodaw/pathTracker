/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/debuggpsdatascreen_screen/DebugGpsDataScreenViewBase.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

DebugGpsDataScreenViewBase::DebugGpsDataScreenViewBase() :
    buttonCallback(this, &DebugGpsDataScreenViewBase::buttonCallbackHandler)
{

    BackgroundCC.setXY(0, 0);

    NextButton.setXY(180, 260);
    NextButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_NEXT_ARROW_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_NEXT_ARROW_32_ID));
    NextButton.setIconXY(22, 15);
    NextButton.setAction(buttonCallback);

    GpsDataCC.setXY(0, 50);

    NameLabel.setXY(38, 15);
    NameLabel.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    NameLabel.setLinespacing(0);
    NameLabel.setTypedText(touchgfx::TypedText(T_SINGLEUSEID58));

    add(BackgroundCC);
    add(NextButton);
    add(GpsDataCC);
    add(NameLabel);
}

void DebugGpsDataScreenViewBase::setupScreen()
{
    BackgroundCC.initialize();
    GpsDataCC.initialize();
}

void DebugGpsDataScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &NextButton)
    {
        //GoToDebugSdCardScreen
        //When NextButton clicked change screen to DebugSdCardScreen
        //Go to DebugSdCardScreen with no screen transition
        application().gotoDebugSdCardScreenScreenNoTransition();
    }
}
