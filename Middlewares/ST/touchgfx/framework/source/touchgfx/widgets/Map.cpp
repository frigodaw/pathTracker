#include <touchgfx/widgets/Map.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{

Map::Map() :
      Box(),
      meshInfo({0}),
      backgroundColor(0xFFFF),
      trackColor(0),
      lineColor(0x8888),
      size(3)
{
    FlushTrackList();
}


void Map::draw(const touchgfx::Rect& invalidatedArea) const
{
    touchgfx::Rect absolute = getAbsoluteRect();

    uint16_t* framebuffer = touchgfx::HAL::getInstance()->lockFrameBuffer();

    uint16_t xVal = 0u;
    uint16_t yVal = 0u;

    /* Draw background */
    for (int y = invalidatedArea.y; y < invalidatedArea.bottom(); y++)
    {
        for (int x = invalidatedArea.x; x < invalidatedArea.right(); x++)
        {
            framebuffer[absolute.x + x + (absolute.y + y) * touchgfx::HAL::DISPLAY_WIDTH] = backgroundColor;
        }
    }

    /* Draw scale line */
    uint16_t xStart = (absolute.width - MAP_SCALE_LINE_LENGTH_PX) / MAP_SCALE_LINE_DIV_COEFF;
    uint16_t xEnd = xStart + MAP_SCALE_LINE_LENGTH_PX;
    uint16_t yStart = absolute.bottom() - MAP_SCALE_LOCATION_ABOVE_BOTTOM_PX - MAP_SCALE_LINE_HEIGTH_PX;
    uint16_t yEnd = yStart + MAP_SCALE_LINE_HEIGTH_PX;

    for (uint16_t y = yStart; y < yEnd; y++)
    {
        for (uint16_t x = xStart; x < xEnd; x++)
        {
            framebuffer[x + (y) * touchgfx::HAL::DISPLAY_WIDTH] = trackColor;
        }
    }

    xStart = (absolute.width - MAP_SCALE_INNER_LINE_LENGTH_PX) / MAP_SCALE_LINE_DIV_COEFF;
    xEnd = xStart + MAP_SCALE_INNER_LINE_LENGTH_PX;
    yStart = absolute.bottom() - MAP_SCALE_LOCATION_ABOVE_BOTTOM_PX - MAP_SCALE_LINE_HEIGTH_PX;
    yEnd = yStart + MAP_SCALE_INNER_LINE_HEIGTH_PX;

    for (uint16_t y = yStart; y < yEnd; y++)
    {
        for (uint16_t x = xStart; x < xEnd; x++)
        {
            framebuffer[x + (y) * touchgfx::HAL::DISPLAY_WIDTH] = backgroundColor;
        }
    }

    /* Draw dots on a track points */
    for (uint8_t i = 0u; i < trackList.idx; i++)
    {
        for (int y = 0u; y < meshInfo.elementDimension.Y; y++)
        {
            for (int x = 0u; x < meshInfo.elementDimension.X; x++)
            {
                xVal = absolute.x + trackList.coords[i].X * meshInfo.elementDimension.X + x;
                yVal = absolute.y + trackList.coords[i].Y * meshInfo.elementDimension.Y + y;
                framebuffer[xVal + (yVal) * touchgfx::HAL::DISPLAY_WIDTH] = trackColor;
            }
        }
    }

    /* Connect dots and create line path */
    if(trackList.idx > 1u)
    {
        Map_CoordinatesXY_T prev = {0u};
        Map_CoordinatesXY_T curr = {0u};
        Map_CoordinatesXY_T lineCoords = {0u};
        Map_FunctionType_T funcType = MAP_FUNCTION_INVALID;
        float a = 0.f;
        float b = 0.f;

        for (uint8_t i = 1u; i < trackList.idx; i++)
        {
            /* Get points to connect */
            prev.X = trackList.coords[i-1u].X * meshInfo.elementDimension.X;
            prev.Y = trackList.coords[i-1u].Y * meshInfo.elementDimension.Y;
            curr.X = trackList.coords[i].X * meshInfo.elementDimension.X;
            curr.Y = trackList.coords[i].Y * meshInfo.elementDimension.Y;

            uint16_t xStart = (curr.X < prev.X) ? curr.X : prev.X;
            uint16_t xEnd = (curr.X > prev.X) ? curr.X : prev.X;
            uint16_t yStart = (curr.Y < prev.Y) ? curr.Y : prev.Y;
            uint16_t yEnd = (curr.Y > prev.Y) ? curr.Y : prev.Y;

            funcType = MapCoordsToLinearFunction(curr, prev, a, b);

            /* Linear function y=f(x) */
            if(MAP_FUNCTION_Y_FX == funcType)
            {
                for (uint16_t x = xStart; x < xEnd; x++)
                {
                    lineCoords.X = absolute.x + x;
                    for (uint16_t dy = 0u; dy < meshInfo.elementDimension.Y; dy++)
                    {
                        lineCoords.Y = absolute.y + (float)(a*x) + b + dy;
                        framebuffer[lineCoords.X + (lineCoords.Y) * touchgfx::HAL::DISPLAY_WIDTH] = lineColor;
                    }
                }
            }
            /* Linear function x=f(y) */
            else if(MAP_FUNCTION_X_FY == funcType)
            {
                for (uint16_t y = yStart; y < yEnd; y++)
                {
                    lineCoords.Y = absolute.y + y;
                    for (uint16_t dx = 0u; dx < meshInfo.elementDimension.X; dx++)
                    {
                        lineCoords.X = absolute.x + (float)(a*y) + b + dx;
                        framebuffer[lineCoords.X + (lineCoords.Y) * touchgfx::HAL::DISPLAY_WIDTH] = lineColor;
                    }
                }
            }
        }
    }

    touchgfx::HAL::getInstance()->unlockFrameBuffer();
}


/* Method called to get solid dimensions */
Rect Map::getSolidRect() const
{
    return touchgfx::Rect(0, 0, getWidth(), getHeight());
}


/* Method called to clear list with track coordinates. */
void Map::FlushTrackList(void)
{
    memset(&trackList, 255u, sizeof(trackList));
    trackList.idx = 0u;
    lineColor += 1000u;
}


/* Method called to set scale on a map */
void Map::SetTrackScale(uint16_t scaleVal)
{
    scale = scaleVal;
}


/* Method called to add coordinates to track list. */
void Map::AddCoordsToTrackList(uint8_t x, uint8_t y)
{
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


/* Method called to assign coords to corresponding element in mesh */
Map_CoordinatesXY_T Map::MapCoordsToMesh(uint8_t x, uint8_t y)
{
    Map_CoordinatesXY_T meshCoords = {0u};

    meshCoords.X = (uint8_t)(x/meshInfo.elementDimension.X);
    meshCoords.Y = (uint8_t)(y/meshInfo.elementDimension.Y);

    return meshCoords;
}


/* Method called to select linear function depending on given coords */
Map_FunctionType_T Map::MapCoordsToLinearFunction(Map_CoordinatesXY_T curr, Map_CoordinatesXY_T prev, float &a, float &b) const
{
    Map_FunctionType_T type = MAP_FUNCTION_INVALID;
    a = 0.f;
    b = 0.f;

    if(curr.X != prev.X)
    {
        a = ((float)(curr.Y-prev.Y)) / ((float)(curr.X-prev.X));
        type = (a <= MAP_TRACK_A_COEFF_LIMIT) ? MAP_FUNCTION_Y_FX : MAP_FUNCTION_X_FY;
    }
    else if(curr.Y != prev.Y)
    {
        a = ((float)(curr.X-prev.X)) / ((float)(curr.Y-prev.Y));
        type = (a <= MAP_TRACK_A_COEFF_LIMIT) ? MAP_FUNCTION_X_FY : MAP_FUNCTION_Y_FX;
    }

    switch (type)
    {
        case MAP_FUNCTION_INVALID:
            break;
        case MAP_FUNCTION_Y_FX:
            a = ((float)(curr.Y-prev.Y)) / ((float)(curr.X-prev.X));
            b = ((float)(prev.Y*curr.X - prev.X*curr.Y)) / ((float)(curr.X-prev.X));
            break;
        case MAP_FUNCTION_X_FY:
            a = ((float)(curr.X-prev.X)) / ((float)(curr.Y-prev.Y));
            b = ((float)(prev.X*curr.Y - prev.Y*curr.X)) / ((float)(curr.Y-prev.Y));
            break;
        default:
            break;
    }

    return type;
}


} // namespace touchgfx
