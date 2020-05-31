#ifndef MAP_HPP
# define MAP_HPP

/* START OF THE INCLUDE AREA */
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>
/* END OF THE DEFINE AREA */


/* START OF THE DEFINE AREA */
#define MAP_ELEMENT_SIZE_COMMON             3u
#define MAP_ELEMENT_SIZE_X                  MAP_ELEMENT_SIZE_COMMON
#define MAP_ELEMENT_SIZE_Y                  MAP_ELEMENT_SIZE_COMMON
#define MAP_TRACK_DRAWABLE_ELEMENTS         128u
#define MAP_EMPTYSLOT_VALUE                 255u
#define MAP_TRACK_A_COEFF_LIMIT             1.f

#define MAP_SCALE_LINE_LENGTH_PX            100u
#define MAP_SCALE_LINE_HEIGTH_PX            5u
#define MAP_SCALE_INNER_LINE_LENGTH_PX      96u
#define MAP_SCALE_INNER_LINE_HEIGTH_PX      3u
#define MAP_SCALE_LINE_DIV_COEFF            2u
#define MAP_SCALE_LOCATION_ABOVE_BOTTOM_PX  10u
/* END OF THE DEFINE AREA */


/* START OF THE ENUM AREA */
typedef enum
{
    MAP_FUNCTION_INVALID,
    MAP_FUNCTION_Y_FX,
    MAP_FUNCTION_X_FY
}Map_FunctionType_T;
/* END OF THE ENUM AREA */


/* START OF THE STRUCT AREA */
typedef struct
{
    uint8_t X;
    uint8_t Y;
}Map_CoordinatesXY_T;

typedef struct
{
    Map_CoordinatesXY_T coords[MAP_TRACK_DRAWABLE_ELEMENTS];
    uint8_t idx;
}Map_TrackList_T;

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

    void FlushTrackList(void);
    void AddCoordsToTrackList(uint8_t x, uint8_t y);
    void SetTrackScale(uint16_t scaleVal);

    void SetMesh(void);
    Map_CoordinatesXY_T MapCoordsToMesh(uint8_t x, uint8_t y);
    Map_FunctionType_T MapCoordsToLinearFunction(Map_CoordinatesXY_T curr, Map_CoordinatesXY_T prev, float &a, float &b) const;


private:
    Map_TrackList_T trackList;
    Map_MeshInfo_T meshInfo;
    uint16_t scale;
    uint16_t backgroundColor;
    uint16_t trackColor;
    uint16_t lineColor;
    uint8_t size;

};
} // namespace touchgfx

#endif // MAP_HPP
