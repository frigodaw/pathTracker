#ifndef DRAWSCREENVIEW_HPP
#define DRAWSCREENVIEW_HPP

#include <gui_generated/drawscreen_screen/DrawScreenViewBase.hpp>
#include <gui/drawscreen_screen/DrawScreenPresenter.hpp>

class DrawScreenView : public DrawScreenViewBase
{
public:
    DrawScreenView();
    virtual ~DrawScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // DRAWSCREENVIEW_HPP
