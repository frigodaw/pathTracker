#ifndef GPSDATASCREENVIEW_HPP
#define GPSDATASCREENVIEW_HPP

#include <gui_generated/gpsdatascreen_screen/GpsDataScreenViewBase.hpp>
#include <gui/gpsdatascreen_screen/GpsDataScreenPresenter.hpp>

class GpsDataScreenView : public GpsDataScreenViewBase
{
public:
    GpsDataScreenView();
    virtual ~GpsDataScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void updateGpsDataContainer();
protected:
};

#endif // GPSDATASCREENVIEW_HPP
