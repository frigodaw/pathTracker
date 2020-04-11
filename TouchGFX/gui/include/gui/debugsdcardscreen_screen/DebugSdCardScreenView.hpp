#ifndef DEBUGSDCARDSCREENVIEW_HPP
#define DEBUGSDCARDSCREENVIEW_HPP

#include <gui_generated/debugsdcardscreen_screen/DebugSdCardScreenViewBase.hpp>
#include <gui/debugsdcardscreen_screen/DebugSdCardScreenPresenter.hpp>

class DebugSdCardScreenView : public DebugSdCardScreenViewBase
{
public:
    DebugSdCardScreenView();
    ~DebugSdCardScreenView() {}
    void setupScreen();
    void tearDownScreen();

    void NotifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace);
    void NotifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace);
    void NotifySignalChanged_sdCardInfo_state(uint8_t newState);
    void NotifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum);
    void NotifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum);
protected:
};

#endif // DEBUGSDCARDSCREENVIEW_HPP
