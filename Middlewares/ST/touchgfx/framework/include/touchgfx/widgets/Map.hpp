#ifndef MAP_HPP
# define MAP_HPP

/* START OF THE INCLUDE AREA */
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
#define MAP_ROUTE_DRAWABLE_ELEMENTS         200u
#define MAP_EMPTYSLOT_VALUE                 255u
#define MAP_ROUTE_A_COEFF_LIMIT             1.f
#define MAP_ROUTE_COLOR_CHANGE_INTERVAL     128u
#define MAP_MIN_POINTS_TO_DRAW              1u
#define MAP_MIN_POINTS_OFFSET               MAP_MIN_POINTS_TO_DRAW

#define MAP_SCALE_LINE_LENGTH_PX            100u
#define MAP_SCALE_LINE_HEIGTH_PX            5u
#define MAP_SCALE_INNER_LINE_LENGTH_PX      96u
#define MAP_SCALE_INNER_LINE_HEIGTH_PX      3u
#define MAP_SCALE_LINE_DIV_COEFF            2u
#define MAP_SCALE_LOCATION_ABOVE_BOTTOM_PX  0u

#define MAP_COLOR_BLACK                     0x0000u
#define MAP_COLOR_GREEN                     0x6666u
#define MAP_COLOR_WHITE                     0xFFFFu
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
typedef enum
{
    MAP_FUNCTION_INVALID,
    MAP_FUNCTION_Y_FX,
    MAP_FUNCTION_X_FY
}Map_FunctionType_T;

typedef enum
{
    MAP_DRAWROUTE_MAP,
    MAP_DRAWROUTE_TRACK
}Map_DrawRoute_T;
/* END OF THE ENUM AREA */


/* START OF THE STRUCT AREA */
typedef struct
{
    uint8_t X;
    uint8_t Y;
}Map_CoordinatesXY_T;

typedef struct
{
    uint16_t X;
    uint16_t Y;
}Map_CoordinatesXY_U16_T;

typedef struct
{
    Map_CoordinatesXY_T coords[MAP_ROUTE_DRAWABLE_ELEMENTS];
    uint8_t idxMap;
    uint8_t idxTrack;
    Map_DrawRoute_T prevRoute;
}Map_RouteList_T;

typedef struct
{
    Map_CoordinatesXY_T elementDimension;
    Map_CoordinatesXY_T elementsPerCoord;
}Map_MeshInfo_T;
/* END OF THE STRUCT AREA */

namespace touchgfx
{

class Map : public Box
{
public:
    Map();

    virtual void draw(const touchgfx::Rect& invalidatedArea) const;
    virtual touchgfx::Rect getSolidRect() const;

    bool IsInvalidatedArea(uint8_t right, uint8_t bottom, Map_CoordinatesXY_U16_T coordsCurr) const;

    void FlushRouteList(void);
    bool AddCoordsToRouteList(uint8_t x, uint8_t y, Map_DrawRoute_T route);
    void SetRouteScale(uint32_t scaleVal);
    void IncrementRouteIdx(Map_DrawRoute_T route, uint8_t addedPoints);

    void SetMesh(void);
    Map_CoordinatesXY_T MapCoordsToMesh(uint8_t x, uint8_t y);
    Map_FunctionType_T MapCoordsToLinearFunction(Map_CoordinatesXY_T curr, Map_CoordinatesXY_T prev, float &a, float &b) const;


private:
    Map_RouteList_T routeList;
    Map_MeshInfo_T meshInfo;
    uint32_t scale;
    uint16_t backgroundColor;
    uint16_t mapDotColor;
    uint16_t mapLineColor;
    uint16_t trackDotColor;
    uint16_t trackLineColor;
    uint8_t size;

};
} // namespace touchgfx

#endif // MAP_HPP
