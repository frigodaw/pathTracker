#ifndef MAP_HPP
# define MAP_HPP

#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>

#define MAP_ELEMENT_SIZE_X                5u
#define MAP_ELEMENT_SIZE_Y                5u
#define MAP_TRACK_DRAWABLE_ELEMENTS     128u
#define MAP_EMPTYSLOT_VALUE             255u

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

    void SetMesh(void);
    Map_CoordinatesXY_T MapCoordsToMesh(uint8_t x, uint8_t y);


private:
    Map_TrackList_T trackList;
    Map_MeshInfo_T meshInfo;
    uint16_t backgroundColor;
    uint16_t trackColor;
    uint8_t size;

};
} // namespace touchgfx

#endif // MAP_HPP
