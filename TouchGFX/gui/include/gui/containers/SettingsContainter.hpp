#ifndef SETTINGSCONTAINTER_HPP
#define SETTINGSCONTAINTER_HPP

#include <gui_generated/containers/SettingsContainterBase.hpp>

class SettingsContainter : public SettingsContainterBase
{
public:
    SettingsContainter();
    ~SettingsContainter() {}
    void initialize();

    void UpdateSettingsLabel(char* name, uint8_t len);
    void UpdateSettingsValue(int8_t value);
protected:
};

#endif // SETTINGSCONTAINTER_HPP
