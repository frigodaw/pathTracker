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

void DebugSdCardScreenView::notifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace)
{
    SDCardDataCC.notifySignalChanged_sdCardInfo_totalSpace(newTotalSpace);
}

void DebugSdCardScreenView::notifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace)
{
    SDCardDataCC.notifySignalChanged_sdCardInfo_freeSpace(newFreeSpace);
}

void DebugSdCardScreenView::notifySignalChanged_sdCardInfo_state(uint8_t newState)
{
    SDCardDataCC.notifySignalChanged_sdCardInfo_state(newState);
}

void DebugSdCardScreenView::notifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum)
{
    SDCardDataCC.notifySignalChanged_dirInfo_in_filesNum(newInFilesNum);
}

void DebugSdCardScreenView::notifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum)
{
    SDCardDataCC.notifySignalChanged_dirInfo_out_filesNum(newOutFilesNum);
}