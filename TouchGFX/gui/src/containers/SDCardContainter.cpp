#include <gui/containers/SDCardContainter.hpp>

SDCardContainter::SDCardContainter()
{

}

void SDCardContainter::initialize()
{
    SDCardContainterBase::initialize();
}


void SDCardContainter::NotifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace)
{
    Unicode::snprintf(TotalSpaceTextBoxBuffer, TOTALSPACETEXTBOX_SIZE, "%d", newTotalSpace);
    TotalSpaceTextBox.invalidate();
}


void SDCardContainter::NotifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace)
{
    Unicode::snprintf(FreeSpaceTextBoxBuffer, FREESPACETEXTBOX_SIZE, "%d", newFreeSpace);
    FreeSpaceTextBox.invalidate();
}


void SDCardContainter::NotifySignalChanged_sdCardInfo_state(uint8_t newState)
{
    Unicode::UnicodeChar stateText[STATETEXTBOX_SIZE] = {0u};

    switch (newState)
    {
        case 0u:
            Unicode::fromUTF8((const uint8_t*)"UNINITIALIZED", stateText, STATETEXTBOX_SIZE);
            break;
        case 1u:
            Unicode::fromUTF8((const uint8_t*)"INITIALIZED", stateText, STATETEXTBOX_SIZE);
            break;
        default:
            Unicode::fromUTF8((const uint8_t*)"INVALID", stateText, STATETEXTBOX_SIZE);
            break;
    }

    Unicode::snprintf(StateTextBoxBuffer, STATETEXTBOX_SIZE, "%s", stateText);
    StateTextBox.invalidate();
}


void SDCardContainter::NotifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum)
{
    Unicode::snprintf(InputFilesTextBoxBuffer, INPUTFILESTEXTBOX_SIZE, "%d", newInFilesNum);
    InputFilesTextBox.invalidate();
}


void SDCardContainter::NotifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum)
{
    Unicode::snprintf(OutputFilesTextBoxBuffer, OUTPUTFILESTEXTBOX_SIZE, "%d", newOutFilesNum);
    OutputFilesTextBox.invalidate();
}