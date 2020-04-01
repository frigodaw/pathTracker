#ifndef DEBUGSDCARDSCREENPRESENTER_HPP
#define DEBUGSDCARDSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DebugSdCardScreenView;

class DebugSdCardScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DebugSdCardScreenPresenter(DebugSdCardScreenView& v);

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

    virtual ~DebugSdCardScreenPresenter() {};

private:
    DebugSdCardScreenPresenter();

    DebugSdCardScreenView& view;
};

#endif // DEBUGSDCARDSCREENPRESENTER_HPP
