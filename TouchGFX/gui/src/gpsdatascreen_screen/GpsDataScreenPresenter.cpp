#include <gui/gpsdatascreen_screen/GpsDataScreenView.hpp>
#include <gui/gpsdatascreen_screen/GpsDataScreenPresenter.hpp>

GpsDataScreenPresenter::GpsDataScreenPresenter(GpsDataScreenView& v)
    : view(v)
{

}

void GpsDataScreenPresenter::activate()
{

}

void GpsDataScreenPresenter::deactivate()
{

}

void GpsDataScreenPresenter::updateGpsDataContainer()
{
    view.updateGpsDataContainer();
}

