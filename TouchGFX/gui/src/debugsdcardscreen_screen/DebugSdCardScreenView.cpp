#include <gui/debugsdcardscreen_screen/DebugSdCardScreenView.hpp>

DebugSdCardScreenView::DebugSdCardScreenView()
{

}

void DebugSdCardScreenView::setupScreen()
{
    DebugSdCardScreenViewBase::setupScreen();
}

void DebugSdCardScreenView::tearDownScreen()
{
    DebugSdCardScreenViewBase::tearDownScreen();
}

void DebugSdCardScreenView::NotifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace)
{
    SDCardDataCC.NotifySignalChanged_sdCardInfo_totalSpace(newTotalSpace);
}

void DebugSdCardScreenView::NotifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace)
{
    SDCardDataCC.NotifySignalChanged_sdCardInfo_freeSpace(newFreeSpace);
}

void DebugSdCardScreenView::NotifySignalChanged_sdCardInfo_state(uint8_t newState)
{
    SDCardDataCC.NotifySignalChanged_sdCardInfo_state(newState);
}

void DebugSdCardScreenView::NotifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum)
{
    SDCardDataCC.NotifySignalChanged_dirInfo_in_filesNum(newInFilesNum);
}

void DebugSdCardScreenView::NotifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum)
{
    SDCardDataCC.NotifySignalChanged_dirInfo_out_filesNum(newOutFilesNum);
}