#ifndef APPMENUSCREENPRESENTER_HPP
#define APPMENUSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class AppMenuScreenView;

class AppMenuScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    AppMenuScreenPresenter(AppMenuScreenView& v);

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

    virtual ~AppMenuScreenPresenter() {};

private:
    AppMenuScreenPresenter();

    AppMenuScreenView& view;
};

#endif // APPMENUSCREENPRESENTER_HPP
