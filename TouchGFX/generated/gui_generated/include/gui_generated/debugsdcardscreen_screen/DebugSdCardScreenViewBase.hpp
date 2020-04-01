/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef DEBUGSDCARDSCREENVIEWBASE_HPP
#define DEBUGSDCARDSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/debugsdcardscreen_screen/DebugSdCardScreenPresenter.hpp>
#include <gui/containers/ScreenBackgroundContainer.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <gui/containers/SDCardContainter.hpp>

class DebugSdCardScreenViewBase : public touchgfx::View<DebugSdCardScreenPresenter>
{
public:
    DebugSdCardScreenViewBase();
    virtual ~DebugSdCardScreenViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    ScreenBackgroundContainer BackgroundCC;
    touchgfx::ButtonWithLabel NextButton;
    touchgfx::TextArea NameLabel;
    SDCardContainter SDCardDataCC;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<DebugSdCardScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // DEBUGSDCARDSCREENVIEWBASE_HPP