#include <gui/debugsdcardscreen_screen/DebugSdCardScreenView.hpp>
#include <gui/debugsdcardscreen_screen/DebugSdCardScreenPresenter.hpp>

DebugSdCardScreenPresenter::DebugSdCardScreenPresenter(DebugSdCardScreenView& v)
    : view(v)
{

}

void DebugSdCardScreenPresenter::activate()
{

}

void DebugSdCardScreenPresenter::deactivate()
{

}

void DebugSdCardScreenPresenter::notifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace)
{
    view.notifySignalChanged_sdCardInfo_totalSpace(newTotalSpace);
}

void DebugSdCardScreenPresenter::notifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace)
{
    view.notifySignalChanged_sdCardInfo_freeSpace(newFreeSpace);
}

void DebugSdCardScreenPresenter::notifySignalChanged_sdCardInfo_state(uint8_t newState)
{
    view.notifySignalChanged_sdCardInfo_state(newState);
}

void DebugSdCardScreenPresenter::notifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum)
{
    view.notifySignalChanged_dirInfo_in_filesNum(newInFilesNum);
}

void DebugSdCardScreenPresenter::notifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum)
{
    view.notifySignalChanged_dirInfo_out_filesNum(newOutFilesNum);
}