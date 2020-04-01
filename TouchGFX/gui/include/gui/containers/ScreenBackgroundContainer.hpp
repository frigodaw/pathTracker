#ifndef SCREENBACKGROUNDCONTAINER_HPP
#define SCREENBACKGROUNDCONTAINER_HPP

#include <gui_generated/containers/ScreenBackgroundContainerBase.hpp>

class ScreenBackgroundContainer : public ScreenBackgroundContainerBase
{
public:
    ScreenBackgroundContainer();
    virtual ~ScreenBackgroundContainer() {}

    virtual void initialize();
protected:
};

#endif // SCREENBACKGROUNDCONTAINER_HPP
