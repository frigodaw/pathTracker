#ifndef APPSETTINGSSCREENVIEW_HPP
#define APPSETTINGSSCREENVIEW_HPP

#include <gui_generated/appsettingsscreen_screen/AppSettingsScreenViewBase.hpp>
#include <gui/appsettingsscreen_screen/AppSettingsScreenPresenter.hpp>


class AppSettingsScreenView : public AppSettingsScreenViewBase
{
public:
    AppSettingsScreenView();
    ~AppSettingsScreenView() {}
    void setupScreen();
    void tearDownScreen();

    void DisplayPreviousSetting(void);
    void DisplayNextSetting(void);
    void IncrementValue(void);
    void DecrementValue(void);

    void UpdateSettingsLabel(char* name, uint8_t len);
    void UpdateSettingsValue(int8_t value);

protected:
};

#endif // APPSETTINGSSCREENVIEW_HPP
