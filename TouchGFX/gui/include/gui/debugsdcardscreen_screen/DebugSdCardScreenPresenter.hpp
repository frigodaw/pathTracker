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
    
    virtual void NotifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace);
    virtual void NotifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace);
    virtual void NotifySignalChanged_sdCardInfo_state(uint8_t newState);
    virtual void NotifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum);
    virtual void NotifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum);

    virtual ~DebugSdCardScreenPresenter() {};

private:
    DebugSdCardScreenPresenter();

    DebugSdCardScreenView& view;
};

#endif // DEBUGSDCARDSCREENPRESENTER_HPP
