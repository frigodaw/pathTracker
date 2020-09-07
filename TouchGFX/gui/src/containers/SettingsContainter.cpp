#include <gui/containers/SettingsContainter.hpp>

SettingsContainter::SettingsContainter()
{

}

void SettingsContainter::initialize()
{
    SettingsContainterBase::initialize();
}

void SettingsContainter::UpdateSettingsLabel(char* name, uint8_t len)
{
    Unicode::UnicodeChar nameText[VARNAMELABEL_SIZE] = {0u};
    Unicode::fromUTF8((const uint8_t*)name, nameText, VARNAMELABEL_SIZE);
    Unicode::snprintf(VarNameLabelBuffer, VARNAMELABEL_SIZE, "%s", nameText);
    VarNameLabel.invalidate();
}

void SettingsContainter::UpdateSettingsValue(int16_t value)
{
    Unicode::snprintf(VarTextBoxBuffer, VARTEXTBOX_SIZE, "%d", value);
    VarTextBox.invalidate();
}
