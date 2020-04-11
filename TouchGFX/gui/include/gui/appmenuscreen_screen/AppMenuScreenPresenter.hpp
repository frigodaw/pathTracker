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

    virtual void activate();
    virtual void deactivate();

    virtual ~AppMenuScreenPresenter() {};

private:
    AppMenuScreenPresenter();

    AppMenuScreenView& view;
};

#endif // APPMENUSCREENPRESENTER_HPP
