#include <gui/appsettingsscreen_screen/AppSettingsScreenView.hpp>

AppSettingsScreenView::AppSettingsScreenView()
{

}

void AppSettingsScreenView::setupScreen()
{
    AppSettingsScreenViewBase::setupScreen();
}

void AppSettingsScreenView::tearDownScreen()
{
    AppSettingsScreenViewBase::tearDownScreen();
}

void AppSettingsScreenView::DisplayPreviousSetting(void)
{
    presenter->DisplayPreviousSetting();
}

void AppSettingsScreenView::DisplayNextSetting(void)
{
    presenter->DisplayNextSetting();
}

void AppSettingsScreenView::IncrementValue(void)
{
    presenter->IncrementValue();
}

void AppSettingsScreenView::DecrementValue(void)
{
    presenter->DecrementValue();
}

void AppSettingsScreenView::UpdateSettingsLabel(char* name, uint8_t len)
{
    SettingsCC.UpdateSettingsLabel(name, len);
}

void AppSettingsScreenView::UpdateSettingsValue(int16_t value)
{
    SettingsCC.UpdateSettingsValue(value);
}
