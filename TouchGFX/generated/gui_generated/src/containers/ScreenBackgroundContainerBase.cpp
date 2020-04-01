/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/ScreenBackgroundContainerBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

ScreenBackgroundContainerBase::ScreenBackgroundContainerBase() :
    buttonCallback(this, &ScreenBackgroundContainerBase::buttonCallbackHandler)
{
    setWidth(240);
    setHeight(320);
    background.setPosition(0, 0, 240, 320);
    background.setColor(touchgfx::Color::getColorFrom24BitRGB(100, 160, 200));

    ExitButton.setXY(0, 260);
    ExitButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    ExitButton.setLabelText(touchgfx::TypedText(T_SINGLEUSEID67));
    ExitButton.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    ExitButton.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    ExitButton.setAction(buttonCallback);

    add(background);
    add(ExitButton);
}

void ScreenBackgroundContainerBase::initialize()
{

}

void ScreenBackgroundContainerBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &ExitButton)
    {
        //GoToStartScreen
        //When ExitButton clicked change screen to StartScreen
        //Go to StartScreen with no screen transition
        application().gotoStartScreenScreenNoTransition();
    }
}
