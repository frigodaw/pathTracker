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
    ~DebugSdCardScreenPresenter() {};
    void activate();
    void deactivate();

    void NotifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace);
    void NotifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace);
    void NotifySignalChanged_sdCardInfo_state(uint8_t newState);
    void NotifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum);
    void NotifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum);

private:
    DebugSdCardScreenPresenter();

    DebugSdCardScreenView& view;
};

#endif // DEBUGSDCARDSCREENPRESENTER_HPP
