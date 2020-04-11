#ifndef STARTSCREENPRESENTER_HPP
#define STARTSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class StartScreenView;

class StartScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    StartScreenPresenter(StartScreenView& v);
    ~StartScreenPresenter() {};
    void activate();
    void deactivate();

private:
    StartScreenPresenter();

    StartScreenView& view;
};

#endif // STARTSCREENPRESENTER_HPP
