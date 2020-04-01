#ifndef DEBUGGPSDATASCREENPRESENTER_HPP
#define DEBUGGPSDATASCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DebugGpsDataScreenView;

class DebugGpsDataScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DebugGpsDataScreenPresenter(DebugGpsDataScreenView& v);

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

    virtual void tickDebugScreen();

    virtual ~DebugGpsDataScreenPresenter() {};

private:
    DebugGpsDataScreenPresenter();

    DebugGpsDataScreenView& view;
};

#endif // DEBUGGPSDATASCREENPRESENTER_HPP
