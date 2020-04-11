#ifndef ACTIVITYDATACONTAINTER_HPP
#define ACTIVITYDATACONTAINTER_HPP

#include <gui_generated/containers/ActivityDataContainterBase.hpp>

class ActivityDataContainter : public ActivityDataContainterBase
{
public:
    ActivityDataContainter();
    virtual ~ActivityDataContainter() {}

    virtual void initialize();
protected:
};

#endif // ACTIVITYDATACONTAINTER_HPP
