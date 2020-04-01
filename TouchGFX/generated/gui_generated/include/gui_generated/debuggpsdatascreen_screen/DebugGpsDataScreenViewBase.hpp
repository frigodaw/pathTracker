/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef DEBUGGPSDATASCREENVIEWBASE_HPP
#define DEBUGGPSDATASCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenPresenter.hpp>
#include <gui/containers/ScreenBackgroundContainer.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <gui/containers/GpsDataContainer.hpp>
#include <touchgfx/widgets/TextArea.hpp>

class DebugGpsDataScreenViewBase : public touchgfx::View<DebugGpsDataScreenPresenter>
{
public:
    DebugGpsDataScreenViewBase();
    virtual ~DebugGpsDataScreenViewBase() {}
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
    GpsDataContainer GpsDataCC;
    touchgfx::TextArea NameLabel;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<DebugGpsDataScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // DEBUGGPSDATASCREENVIEWBASE_HPP
