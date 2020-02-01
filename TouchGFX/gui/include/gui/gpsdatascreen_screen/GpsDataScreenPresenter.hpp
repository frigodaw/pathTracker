#ifndef GPSDATASCREENPRESENTER_HPP
#define GPSDATASCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class GpsDataScreenView;

class GpsDataScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    GpsDataScreenPresenter(GpsDataScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual void updateGpsDataContainer();

    virtual ~GpsDataScreenPresenter() {};

private:
    GpsDataScreenPresenter();

    GpsDataScreenView& view;
};

#endif // GPSDATASCREENPRESENTER_HPP
