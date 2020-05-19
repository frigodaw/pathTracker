#ifndef ACTIVITYDATACONTAINTER_MAP_HPP
#define ACTIVITYDATACONTAINTER_MAP_HPP

#include <gui_generated/containers/ActivityDataContainter_MapBase.hpp>

class ActivityDataContainter_Map : public ActivityDataContainter_MapBase
{
public:
    ActivityDataContainter_Map();
    ~ActivityDataContainter_Map() {}
    void initialize();

    void DrawLine(float coordsX, float coordsY);
protected:
};

#endif // ACTIVITYDATACONTAINTER_MAP_HPP
