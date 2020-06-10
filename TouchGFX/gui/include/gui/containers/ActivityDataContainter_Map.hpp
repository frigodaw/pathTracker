#ifndef ACTIVITYDATACONTAINTER_MAP_HPP
#define ACTIVITYDATACONTAINTER_MAP_HPP

#include <gui_generated/containers/ActivityDataContainter_MapBase.hpp>
#include <touchgfx/widgets/Map.hpp>

#define MAPDATACONTAINER_SCALE_THRESHOLD                    1000u
#define MAPDATACONTAINER_SCALE_THRESHOLD_FULLVIEW_COEEF1    100u
#define MAPDATACONTAINER_SCALE_THRESHOLD_FULLVIEW_COEEF2    10u

class ActivityDataContainter_Map : public ActivityDataContainter_MapBase
{
public:
    ActivityDataContainter_Map();
    ~ActivityDataContainter_Map() {}
    void initialize();

    void FlushTrackList(void);
    void SetTrackScale(uint32_t scaleVal);
    bool AddCoordsToTrackList(uint8_t coordsX, uint8_t coordsY);
    void TrackRedraw(void);

private:
    touchgfx::Map Map;
};

#endif // ACTIVITYDATACONTAINTER_MAP_HPP
