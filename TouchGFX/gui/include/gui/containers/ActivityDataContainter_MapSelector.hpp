#ifndef ACTIVITYDATACONTAINTER_MAPSELECTOR_HPP
#define ACTIVITYDATACONTAINTER_MAPSELECTOR_HPP

#include <gui_generated/containers/ActivityDataContainter_MapSelectorBase.hpp>

class ActivityDataContainter_MapSelector : public ActivityDataContainter_MapSelectorBase
{
public:
    ActivityDataContainter_MapSelector();
    ~ActivityDataContainter_MapSelector() {}
    void initialize();

    void ChangeMapDistance(float distance);
protected:
};

#endif // ACTIVITYDATACONTAINTER_MAPSELECTOR_HPP
