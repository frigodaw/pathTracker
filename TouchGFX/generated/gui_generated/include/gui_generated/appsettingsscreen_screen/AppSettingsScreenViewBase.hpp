/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef APPSETTINGSSCREENVIEWBASE_HPP
#define APPSETTINGSSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/appsettingsscreen_screen/AppSettingsScreenPresenter.hpp>
#include <gui/containers/ScreenBackgroundContainer.hpp>
#include <gui/containers/SettingsContainter.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <touchgfx/widgets/TextArea.hpp>

class AppSettingsScreenViewBase : public touchgfx::View<AppSettingsScreenPresenter>
{
public:
    AppSettingsScreenViewBase();
    virtual ~AppSettingsScreenViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void DisplayPreviousSetting()
    {
        // Override and implement this function in AppSettingsScreen
    }

    virtual void DisplayNextSetting()
    {
        // Override and implement this function in AppSettingsScreen
    }

    virtual void IncrementValue()
    {
        // Override and implement this function in AppSettingsScreen
    }

    virtual void DecrementValue()
    {
        // Override and implement this function in AppSettingsScreen
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    ScreenBackgroundContainer BackgroundCC;
    SettingsContainter SettingsCC;
    touchgfx::ButtonWithIcon ExitButton;
    touchgfx::ButtonWithIcon NextButton;
    touchgfx::ButtonWithIcon BackButton;
    touchgfx::ButtonWithIcon DownButton;
    touchgfx::ButtonWithIcon UpButton;
    touchgfx::TextArea NameLabel;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<AppSettingsScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // APPSETTINGSSCREENVIEWBASE_HPP