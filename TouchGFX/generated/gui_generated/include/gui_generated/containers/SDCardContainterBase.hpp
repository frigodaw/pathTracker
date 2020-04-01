/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SDCARDCONTAINTERBASE_HPP
#define SDCARDCONTAINTERBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class SDCardContainterBase : public touchgfx::Container
{
public:
    SDCardContainterBase();
    virtual ~SDCardContainterBase() {}
    virtual void initialize();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::TextAreaWithOneWildcard FreeSpaceTextBox;
    touchgfx::TextAreaWithOneWildcard TotalSpaceTextBox;
    touchgfx::TextAreaWithOneWildcard StateTextBox;
    touchgfx::TextAreaWithOneWildcard InputFilesTextBox;
    touchgfx::TextAreaWithOneWildcard OutputFilesTextBox;

    /*
     * Wildcard Buffers
     */
    static const uint16_t FREESPACETEXTBOX_SIZE = 16;
    touchgfx::Unicode::UnicodeChar FreeSpaceTextBoxBuffer[FREESPACETEXTBOX_SIZE];
    static const uint16_t TOTALSPACETEXTBOX_SIZE = 16;
    touchgfx::Unicode::UnicodeChar TotalSpaceTextBoxBuffer[TOTALSPACETEXTBOX_SIZE];
    static const uint16_t STATETEXTBOX_SIZE = 16;
    touchgfx::Unicode::UnicodeChar StateTextBoxBuffer[STATETEXTBOX_SIZE];
    static const uint16_t INPUTFILESTEXTBOX_SIZE = 4;
    touchgfx::Unicode::UnicodeChar InputFilesTextBoxBuffer[INPUTFILESTEXTBOX_SIZE];
    static const uint16_t OUTPUTFILESTEXTBOX_SIZE = 4;
    touchgfx::Unicode::UnicodeChar OutputFilesTextBoxBuffer[OUTPUTFILESTEXTBOX_SIZE];

private:

};

#endif // SDCARDCONTAINTERBASE_HPP