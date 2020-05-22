#include <touchgfx/widgets/Map.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{

Map::Map() :
      Box(),
      meshInfo({0}),
      backgroundColor(0xFFFF),
      trackColor(0),
      size(3)
{
    FlushTrackList();
}


void Map::draw(const touchgfx::Rect& invalidatedArea) const
{
    touchgfx::Rect absolute = getAbsoluteRect();

    uint16_t* framebuffer = touchgfx::HAL::getInstance()->lockFrameBuffer();

    /* Draw background */
    for (int y = invalidatedArea.y; y < invalidatedArea.bottom(); y++)
    {
        for (int x = invalidatedArea.x; x < invalidatedArea.right(); x++)
        {
            framebuffer[absolute.x + x + (absolute.y + y) * touchgfx::HAL::DISPLAY_WIDTH] = backgroundColor;
        }
    }

    /* Draw track */
    uint8_t xVal = 0u;
    uint8_t yVal = 0u;

    for (uint8_t i = 0u; i < trackList.idx; i++)
    {
        for (int y = 0u; y < meshInfo.elementDimension.Y; y++)
        {
            for (int x = 0u; x < meshInfo.elementDimension.X; x++)
            {
                xVal = absolute.x + trackList.coords[i].X*meshInfo.elementDimension.X + x;
                yVal = absolute.y + trackList.coords[i].Y*meshInfo.elementDimension.Y + y;
                framebuffer[xVal + (yVal) * touchgfx::HAL::DISPLAY_WIDTH] = trackColor;
            }
        }
    }

    touchgfx::HAL::getInstance()->unlockFrameBuffer();
}


Rect Map::getSolidRect() const
{
    return touchgfx::Rect(0, 0, getWidth(), getHeight());
}


/* Method called to clear list with track coordinates. */
void Map::FlushTrackList(void)
{
    memset(&trackList, 255u, sizeof(trackList));
    trackList.idx = 0u;
}


/* Method called to add coordinates to track list. */
void Map::AddCoordsToTrackList(uint8_t x, uint8_t y)
{
    /* Basic working version.
    if(trackList.idx < (MAP_TRACK_DRAWABLE_ELEMENTS - 1u))
    {
        trackList.coords[trackList.idx].X = x;
        trackList.coords[trackList.idx].Y = y;
        trackList.idx++;
    }*/

    Map_CoordinatesXY_T meshCoords = MapCoordsToMesh(x,y);
    bool duplicate = false;

    for(uint8_t i=0u; i<trackList.idx; i++)
    {
        if((meshCoords.X == trackList.coords[i].X) && (meshCoords.Y == trackList.coords[i].Y))
        {
            duplicate = true;
            break;
        }
    }

    if(false == duplicate)
    {
        if(trackList.idx < (MAP_TRACK_DRAWABLE_ELEMENTS - 1u))
        {
            trackList.coords[trackList.idx].X = meshCoords.X;
            trackList.coords[trackList.idx].Y = meshCoords.Y;
            trackList.idx++;
        }
    }
}


/* Method called to split widget display into smaller elements. */
void Map::SetMesh(void)
{
    meshInfo.elementDimension.X = MAP_ELEMENT_SIZE_X;
    meshInfo.elementDimension.Y = MAP_ELEMENT_SIZE_Y;

    meshInfo.elementsPerCoord.X = getWidth()/meshInfo.elementDimension.X;
    meshInfo.elementsPerCoord.Y = getHeight()/meshInfo.elementDimension.Y;
}


Map_CoordinatesXY_T Map::MapCoordsToMesh(uint8_t x, uint8_t y)
{
    Map_CoordinatesXY_T meshCoords = {0u};

    meshCoords.X = (uint8_t)(x/meshInfo.elementDimension.X);
    meshCoords.Y = (uint8_t)(y/meshInfo.elementDimension.Y);

    return meshCoords;
}


} // namespace touchgfx
