/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef DEBUGENVSENSORSSCREENVIEWBASE_HPP
#define DEBUGENVSENSORSSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/debugenvsensorsscreen_screen/DebugEnvSensorsScreenPresenter.hpp>
#include <gui/containers/ScreenBackgroundContainer.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <gui/containers/EnvSensorsContainter.hpp>

class DebugEnvSensorsScreenViewBase : public touchgfx::View<DebugEnvSensorsScreenPresenter>
{
public:
    DebugEnvSensorsScreenViewBase();
    virtual ~DebugEnvSensorsScreenViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    ScreenBackgroundContainer BackgroundCC;
    touchgfx::ButtonWithIcon NextButton;
    touchgfx::TextArea NameLabel;
    EnvSensorsContainter EnvSensorsDataCC;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<DebugEnvSensorsScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // DEBUGENVSENSORSSCREENVIEWBASE_HPP
