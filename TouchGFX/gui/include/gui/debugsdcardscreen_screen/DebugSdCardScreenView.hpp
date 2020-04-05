#ifndef DEBUGSDCARDSCREENVIEW_HPP
#define DEBUGSDCARDSCREENVIEW_HPP

#include <gui_generated/debugsdcardscreen_screen/DebugSdCardScreenViewBase.hpp>
#include <gui/debugsdcardscreen_screen/DebugSdCardScreenPresenter.hpp>

class DebugSdCardScreenView : public DebugSdCardScreenViewBase
{
public:
    DebugSdCardScreenView();
    virtual ~DebugSdCardScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void notifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace);
    virtual void notifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace);
    virtual void notifySignalChanged_sdCardInfo_state(uint8_t newState);
    virtual void notifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum);
    virtual void notifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum);
protected:
};

#endif // DEBUGSDCARDSCREENVIEW_HPP
