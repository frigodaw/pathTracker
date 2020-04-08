#ifndef SDCARDCONTAINTER_HPP
#define SDCARDCONTAINTER_HPP

#include <gui_generated/containers/SDCardContainterBase.hpp>

class SDCardContainter : public SDCardContainterBase
{
public:
    SDCardContainter();
    virtual ~SDCardContainter() {}

    virtual void initialize();

    virtual void NotifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace);
    virtual void NotifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace);
    virtual void NotifySignalChanged_sdCardInfo_state(uint8_t newState);
    virtual void NotifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum);
    virtual void NotifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum);

protected:
};

#endif // SDCARDCONTAINTER_HPP
