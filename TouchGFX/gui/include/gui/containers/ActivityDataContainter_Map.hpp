#ifndef ACTIVITYDATACONTAINTER_MAP_HPP
#define ACTIVITYDATACONTAINTER_MAP_HPP

#include <gui_generated/containers/ActivityDataContainter_MapBase.hpp>
#include <gui/common/Map.hpp>

#define MAPDATACONTAINER_SCALE_THRESHOLD                    1000u
#define MAPDATACONTAINER_SCALE_THRESHOLD_FULLVIEW_COEEF1    100u
#define MAPDATACONTAINER_SCALE_THRESHOLD_FULLVIEW_COEEF2    10u

#define MAPDATACONTAINER_ARROW_DIV_HALF_COEFF               2u

class ActivityDataContainter_Map : public ActivityDataContainter_MapBase
{
public:
    ActivityDataContainter_Map();
    ~ActivityDataContainter_Map();
    void initialize();

    void FlushRouteList(void);
    void SetRouteScale(uint32_t scaleVal);
    bool AddCoordsToRouteList(uint8_t coordsX, uint8_t coordsY, Map_DrawRoute_T route, bool add);
    void TrackRedraw(void);
    void IncrementRouteIdx(Map_DrawRoute_T route, uint8_t addedPoints);
    void SetArrowVisibilityStatus(bool visibilityStatus);
    void SetArrowLocation(uint8_t X, uint8_t Y);
    void SetArrowAngle(float angle);

private:
    touchgfx::Map Map;
};

#endif // ACTIVITYDATACONTAINTER_MAP_HPP
