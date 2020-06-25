#include <gui/appsettingsscreen_screen/AppSettingsScreenView.hpp>
#include <gui/appsettingsscreen_screen/AppSettingsScreenPresenter.hpp>
#include "dataCollector.h"



AppSettingsScreenPresenter::AppSettingsScreenPresenter(AppSettingsScreenView& v)
    : view(v)
{
}

void AppSettingsScreenPresenter::activate()
{
    settingsData.ID = SETTINGS_ID_TIMEZONE;
    settingsData.ptr = &settingsConstData[SETTINGS_ID_TIMEZONE];

    UpdateSettingsView();
    UpdateVariableValues();

    model->SignalRequestFromPresenter();
    model->MainPeriodFromPresenter(SETTINGS_MAINPERIOD_MS);
}

void AppSettingsScreenPresenter::deactivate()
{

}


void AppSettingsScreenPresenter::Init(void)
{

}


/* Main function for Settings component called from Model. */
void AppSettingsScreenPresenter::Main(void)
{

}


/* Method called after BackButton is pressed to change current 
   setting to the previous one. */
void AppSettingsScreenPresenter::DisplayPreviousSetting(void)
{
    if(settingsData.ID > SETTINGS_ID_TIMEZONE)
    {
        settingsData.ID = (Settings_Elements_ID_T)(settingsData.ID - 1u);
    }
    else
    {
        settingsData.ID = (Settings_Elements_ID_T)(SETTINGS_ID_AMOUT_OF_ELEMENTS - 1u);
    }

    settingsData.ptr = &settingsConstData[settingsData.ID];
    UpdateSettingsView();
}


/* Method called after NextButton is pressed to change current 
   setting to the next one. */
void AppSettingsScreenPresenter::DisplayNextSetting(void)
{
    if(settingsData.ID < ((Settings_Elements_ID_T)(SETTINGS_ID_AMOUT_OF_ELEMENTS - 1u)))
    {
        settingsData.ID = (Settings_Elements_ID_T)(settingsData.ID + 1u);
    }
    else
    {
        settingsData.ID = SETTINGS_ID_TIMEZONE;
    }

    settingsData.ptr = &settingsConstData[settingsData.ID];
    UpdateSettingsView();
}


/* Method called to increment current setting. */
void AppSettingsScreenPresenter::IncrementValue(void)
{
    if(settingsData.value[settingsData.ID] < settingsData.ptr->max)
    {
        settingsData.value[settingsData.ID] += 1u;
    }
    else
    {
        settingsData.value[settingsData.ID] = settingsData.ptr->min;
    }

    UpdateSettingsView();
    UpdateVariableValues();
}


/* Method called to decrement current setting. */
void AppSettingsScreenPresenter::DecrementValue(void)
{
    if(settingsData.value[settingsData.ID] > settingsData.ptr->min)
    {
        settingsData.value[settingsData.ID] -= 1u;
    }
    else
    {
        settingsData.value[settingsData.ID] = settingsData.ptr->max;
    }

    UpdateSettingsView();
    UpdateVariableValues();
}


/* Method called to update current values on a display. */
void AppSettingsScreenPresenter::UpdateSettingsView(void)
{
    view.UpdateSettingsLabel((char*)settingsData.ptr->name, SETTINGS_NAME_LEN);
    view.UpdateSettingsValue(settingsData.value[settingsData.ID]);
}


/* Method called to call DataCollector to set proper
   variables to given value. */
void AppSettingsScreenPresenter::UpdateVariableValues(void)
{
    switch(settingsData.ID)
    {
        case SETTINGS_ID_TIMEZONE:
            break;
        case SETTINGS_ID_MESH_SIZE:
            break;
        case SETTINGS_ID_SENSORS:
            DC_set_sensorActivationFlags_enabled(settingsData.value[settingsData.ID]);
            break;

        default:
            break;
    }
}