#ifndef APPSETTINGSSCREENPRESENTER_HPP
#define APPSETTINGSSCREENPRESENTER_HPP

/* START OF THE INCLUDE AREA */
#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include "settings.h"
/* END OF THE INCLUDE AREA */

using namespace touchgfx;

extern "C" Settings_SettingsData_T settingsData;
extern "C" const Settings_SettingsConstData_T settingsConstData[SETTINGS_NUMBER_OF_SETTINGS];
extern "C" const int8_t defaultSettingsValues[SETTINGS_NUMBER_OF_SETTINGS];

/* START OF THE DEFINE AREA */
#define SETTINGS_MAINPERIOD_MS                          200u
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
/* END OF THE ENUM AREA */


/* START OF THE STRUCT AREA */
/* END OF THE STRUCT AREA */

class AppSettingsScreenView;

class AppSettingsScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    AppSettingsScreenPresenter(AppSettingsScreenView& v);
    void activate();
    void deactivate();
    ~AppSettingsScreenPresenter() {};

    void Init(void);
    void Main(void);

    void DisplayPreviousSetting(void);
    void DisplayNextSetting(void);
    void IncrementValue(void);
    void DecrementValue(void);
    void UpdateSettingsView(void);
    void UpdateVariableValues(void);

private:
    AppSettingsScreenPresenter();
    AppSettingsScreenView& view;

    //AppSettings_SettingsData_T *settingsData;
};

#endif // APPSETTINGSSCREENPRESENTER_HPP