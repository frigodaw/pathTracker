#ifndef DEBUGGPSDATASCREENVIEW_HPP
#define DEBUGGPSDATASCREENVIEW_HPP

#include <gui_generated/debuggpsdatascreen_screen/DebugGpsDataScreenViewBase.hpp>
#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenPresenter.hpp>

class DebugGpsDataScreenView : public DebugGpsDataScreenViewBase
{
public:
    DebugGpsDataScreenView();
    virtual ~DebugGpsDataScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void updateGpsDataContainer();
protected:
};

#endif // DEBUGGPSDATASCREENVIEW_HPP
