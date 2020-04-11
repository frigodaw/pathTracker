#ifndef APPMENUSCREENVIEW_HPP
#define APPMENUSCREENVIEW_HPP

#include <gui_generated/appmenuscreen_screen/AppMenuScreenViewBase.hpp>
#include <gui/appmenuscreen_screen/AppMenuScreenPresenter.hpp>

class AppMenuScreenView : public AppMenuScreenViewBase
{
public:
    AppMenuScreenView();
    ~AppMenuScreenView() {}
    void setupScreen();
    void tearDownScreen();
protected:
};

#endif // APPMENUSCREENVIEW_HPP
