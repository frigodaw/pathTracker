#ifndef ACTIVITYDATACONTAINTER_MAP_HPP
#define ACTIVITYDATACONTAINTER_MAP_HPP

#include <gui_generated/containers/ActivityDataContainter_MapBase.hpp>
#include <touchgfx/widgets/Map.hpp>

class ActivityDataContainter_Map : public ActivityDataContainter_MapBase
{
public:
    ActivityDataContainter_Map();
    ~ActivityDataContainter_Map() {}
    void initialize();

    void FlushTrackList(void);
    void AddCoordsToTrackList(uint8_t coordsX, uint8_t coordsY);
    void TrackRedraw(void);

    touchgfx::Map Map;
protected:
};

#endif // ACTIVITYDATACONTAINTER_MAP_HPP
