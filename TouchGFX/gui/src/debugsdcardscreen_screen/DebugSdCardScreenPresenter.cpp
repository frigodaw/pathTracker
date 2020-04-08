#include <gui/debugsdcardscreen_screen/DebugSdCardScreenView.hpp>
#include <gui/debugsdcardscreen_screen/DebugSdCardScreenPresenter.hpp>

DebugSdCardScreenPresenter::DebugSdCardScreenPresenter(DebugSdCardScreenView& v)
    : view(v)
{

}

void DebugSdCardScreenPresenter::activate()
{
    model->SignalRequestFromPresenter();
}

void DebugSdCardScreenPresenter::deactivate()
{

}

void DebugSdCardScreenPresenter::NotifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace)
{
    view.NotifySignalChanged_sdCardInfo_totalSpace(newTotalSpace);
}

void DebugSdCardScreenPresenter::NotifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace)
{
    view.NotifySignalChanged_sdCardInfo_freeSpace(newFreeSpace);
}

void DebugSdCardScreenPresenter::NotifySignalChanged_sdCardInfo_state(uint8_t newState)
{
    view.NotifySignalChanged_sdCardInfo_state(newState);
}

void DebugSdCardScreenPresenter::NotifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum)
{
    view.NotifySignalChanged_dirInfo_in_filesNum(newInFilesNum);
}

void DebugSdCardScreenPresenter::NotifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum)
{
    view.NotifySignalChanged_dirInfo_out_filesNum(newOutFilesNum);
}