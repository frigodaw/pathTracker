/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/SDCardContainterBase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

SDCardContainterBase::SDCardContainterBase()
{
    setWidth(240);
    setHeight(210);
    FreeSpaceTextBox.setPosition(20, 55, 200, 20);
    FreeSpaceTextBox.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    FreeSpaceTextBox.setLinespacing(0);
    Unicode::snprintf(FreeSpaceTextBoxBuffer, FREESPACETEXTBOX_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID75).getText());
    FreeSpaceTextBox.setWildcard(FreeSpaceTextBoxBuffer);
    FreeSpaceTextBox.setTypedText(touchgfx::TypedText(T_SINGLEUSEID74));

    TotalSpaceTextBox.setPosition(20, 35, 200, 20);
    TotalSpaceTextBox.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    TotalSpaceTextBox.setLinespacing(0);
    Unicode::snprintf(TotalSpaceTextBoxBuffer, TOTALSPACETEXTBOX_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID73).getText());
    TotalSpaceTextBox.setWildcard(TotalSpaceTextBoxBuffer);
    TotalSpaceTextBox.setTypedText(touchgfx::TypedText(T_SINGLEUSEID72));

    StateTextBox.setPosition(20, 15, 200, 20);
    StateTextBox.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    StateTextBox.setLinespacing(0);
    Unicode::snprintf(StateTextBoxBuffer, STATETEXTBOX_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID71).getText());
    StateTextBox.setWildcard(StateTextBoxBuffer);
    StateTextBox.setTypedText(touchgfx::TypedText(T_SINGLEUSEID70));

    InputFilesTextBox.setPosition(20, 75, 200, 20);
    InputFilesTextBox.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    InputFilesTextBox.setLinespacing(0);
    Unicode::snprintf(InputFilesTextBoxBuffer, INPUTFILESTEXTBOX_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID77).getText());
    InputFilesTextBox.setWildcard(InputFilesTextBoxBuffer);
    InputFilesTextBox.setTypedText(touchgfx::TypedText(T_SINGLEUSEID76));

    OutputFilesTextBox.setPosition(20, 95, 200, 20);
    OutputFilesTextBox.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    OutputFilesTextBox.setLinespacing(0);
    Unicode::snprintf(OutputFilesTextBoxBuffer, OUTPUTFILESTEXTBOX_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID79).getText());
    OutputFilesTextBox.setWildcard(OutputFilesTextBoxBuffer);
    OutputFilesTextBox.setTypedText(touchgfx::TypedText(T_SINGLEUSEID78));

    add(FreeSpaceTextBox);
    add(TotalSpaceTextBox);
    add(StateTextBox);
    add(InputFilesTextBox);
    add(OutputFilesTextBox);
}

void SDCardContainterBase::initialize()
{

}