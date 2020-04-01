#ifndef SDCARDCONTAINTER_HPP
#define SDCARDCONTAINTER_HPP

#include <gui_generated/containers/SDCardContainterBase.hpp>

class SDCardContainter : public SDCardContainterBase
{
public:
    SDCardContainter();
    virtual ~SDCardContainter() {}

    virtual void initialize();

    virtual void updateSDCardContainer();
protected:
};

#endif // SDCARDCONTAINTER_HPP
