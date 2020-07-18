#include <touchgfx/widgets/Map.hpp>
#include <touchgfx/hal/HAL.hpp>
#include "dataCollector.h"


namespace touchgfx
{

Map::Map() :
      Box(),
      meshInfo({0u}),
      backgroundColor(MAP_COLOR_WHITE),
      mapDotColor(MAP_COLOR_GREEN),
      mapLineColor(MAP_COLOR_GREEN),
      trackDotColor(MAP_COLOR_BLACK),
      trackLineColor(MAP_COLOR_BLACK),
      size(3u)
{
    FlushRouteList();
}


void Map::draw(const touchgfx::Rect& invalidatedArea) const
{
    touchgfx::Rect absolute = getAbsoluteRect();

    uint16_t* framebuffer = touchgfx::HAL::getInstance()->lockFrameBuffer();

    Map_CoordinatesXY_U16_T coords = {0u};
    uint8_t invalidatedAreaRight = (uint8_t)invalidatedArea.right();
    uint8_t invalidatedAreaBottom = (uint8_t)invalidatedArea.bottom();

    /* Draw background */
    for (int y = invalidatedArea.y; y < invalidatedAreaBottom; y++)
    {
        for (int x = invalidatedArea.x; x < invalidatedAreaRight; x++)
        {
            framebuffer[absolute.x + x + (absolute.y + y) * touchgfx::HAL::DISPLAY_WIDTH] = backgroundColor;
        }
    }

    for (uint8_t route = MAP_DRAWROUTE_MAP; route <= MAP_DRAWROUTE_TRACK; route++)
    {
        uint16_t dotColor = 0u;
        uint16_t lineColor = 0u;
        uint8_t idxStart = 0u;
        uint8_t idxEnd = 0u;

        switch(route)
        {
            case MAP_DRAWROUTE_MAP:
                dotColor = mapDotColor;
                lineColor = mapLineColor;
                idxStart = 0u;
                idxEnd = routeList.idxMap;
                break;

            case MAP_DRAWROUTE_TRACK:
                dotColor = trackDotColor;
                lineColor = trackLineColor;
                idxStart = routeList.idxMap;
                idxEnd = routeList.idxTrack;
                break;
            default:
                break;
        }

        /* Connect dots and create line path */
        if( (idxEnd >= idxStart) && ((idxEnd - idxStart) > (uint8_t)MAP_MIN_POINTS_TO_DRAW) )
        {
            Map_CoordinatesXY_T prev = {0u};
            Map_CoordinatesXY_T curr = {0u};
            Map_FunctionType_T funcType = MAP_FUNCTION_INVALID;
            float a = 0.f;
            float b = 0.f;

            for (uint8_t i = idxStart + MAP_MIN_POINTS_OFFSET; i < idxEnd; i++)
            {
                /* Get points to connect */
                prev.X = routeList.coords[i-MAP_MIN_POINTS_OFFSET].X * meshInfo.elementDimension.X;
                prev.Y = routeList.coords[i-MAP_MIN_POINTS_OFFSET].Y * meshInfo.elementDimension.Y;
                curr.X = routeList.coords[i].X * meshInfo.elementDimension.X;
                curr.Y = routeList.coords[i].Y * meshInfo.elementDimension.Y;

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
                        coords.X = absolute.x + x;
                        for (uint16_t dy = 0u; dy < meshInfo.elementDimension.Y; dy++)
                        {
                            coords.Y = absolute.y + (float)(a*x) + b + dy;
                            if(true == IsInvalidatedArea(invalidatedAreaRight, invalidatedAreaBottom, coords))
                            {
                                framebuffer[coords.X + (coords.Y) * touchgfx::HAL::DISPLAY_WIDTH] = lineColor;
                            }
                        }
                    }
                }
                /* Linear function x=f(y) */
                else if(MAP_FUNCTION_X_FY == funcType)
                {
                    for (uint16_t y = yStart; y < yEnd; y++)
                    {
                        coords.Y = absolute.y + y;
                        for (uint16_t dx = 0u; dx < meshInfo.elementDimension.X; dx++)
                        {
                            coords.X = absolute.x + (float)(a*y) + b + dx;
                            if(true == IsInvalidatedArea(invalidatedAreaRight, invalidatedAreaBottom, coords))
                            {
                                framebuffer[coords.X + (coords.Y) * touchgfx::HAL::DISPLAY_WIDTH] = lineColor;
                            }
                        }
                    }
                }
            }
        }

        /* Draw dots on a route points */
        for (uint8_t i = idxStart; i < idxEnd; i++)
        {
            for (int y = 0u; y < meshInfo.elementDimension.Y; y++)
            {
                for (int x = 0u; x < meshInfo.elementDimension.X; x++)
                {
                    coords.X = absolute.x + (uint16_t)(routeList.coords[i].X * meshInfo.elementDimension.X) + x;
                    coords.Y = absolute.y + (uint16_t)(routeList.coords[i].Y * meshInfo.elementDimension.Y) + y;
                    if(true == IsInvalidatedArea(invalidatedAreaRight, invalidatedAreaBottom, coords))
                    {
                        framebuffer[coords.X + (coords.Y) * touchgfx::HAL::DISPLAY_WIDTH] = dotColor;
                    }
                }
            }
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
            coords.X = x;
            coords.Y = y;
            //if(true == IsInvalidatedArea(invalidatedAreaRight, invalidatedAreaBottom, coords))
            {
                framebuffer[coords.X + (coords.Y) * touchgfx::HAL::DISPLAY_WIDTH] = trackDotColor;
            }
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
            coords.X = x;
            coords.Y = y;
            //if(true == IsInvalidatedArea(invalidatedAreaRight, invalidatedAreaBottom, coords))
            {
                framebuffer[coords.X + (coords.Y) * touchgfx::HAL::DISPLAY_WIDTH] = backgroundColor;
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
void Map::FlushRouteList(void)
{
    memset(&routeList, 255u, sizeof(routeList));
    routeList.idxMap = 0u;
    routeList.idxTrack = 0u;
    routeList.prevRoute = MAP_DRAWROUTE_MAP;
}


/* Method called to set scale on a map */
void Map::SetRouteScale(uint32_t scaleVal)
{
    scale = scaleVal;
}


/* Method called to add coordinates to track list. */
bool Map::AddCoordsToRouteList(uint8_t x, uint8_t y, Map_DrawRoute_T route)
{
    Map_CoordinatesXY_T meshCoords = MapCoordsToMesh(x,y);
    bool newPoint = true;
    uint8_t idxStart = 0u;
    uint8_t idxEnd = 0u;
    uint8_t *idxCurr = NULL;

    if((MAP_DRAWROUTE_MAP == routeList.prevRoute) && (MAP_DRAWROUTE_TRACK == route))
    {
        /* Change from MAP to TRACK, set idxTrack */
        routeList.idxTrack = routeList.idxMap;
        routeList.prevRoute = MAP_DRAWROUTE_TRACK;
    }

    switch(route)
    {
        case MAP_DRAWROUTE_MAP:
            idxStart = 0u;
            idxEnd = routeList.idxMap;
            idxCurr = &routeList.idxMap;
            break;

        case MAP_DRAWROUTE_TRACK:
            idxStart = routeList.idxMap;
            idxEnd = routeList.idxTrack;
            idxCurr = &routeList.idxTrack;
            break;

        default:
            break;
    }

    for(uint8_t i = idxStart; i < idxEnd; i++)
    {
        if((meshCoords.X == routeList.coords[i].X) && (meshCoords.Y == routeList.coords[i].Y))
        {
            newPoint = false;
            break;
        }
    }

    if(true == newPoint)
    {
        if( (routeList.idxMap < (MAP_ROUTE_DRAWABLE_ELEMENTS - 1u)) && (routeList.idxTrack < (MAP_ROUTE_DRAWABLE_ELEMENTS - 1u)) )
        {
            routeList.coords[*idxCurr].X = meshCoords.X;
            routeList.coords[*idxCurr].Y = meshCoords.Y;
            (*idxCurr)++;
        }
    }

    return newPoint;
}


/* Method called to split widget display into smaller elements. */
void Map::SetMesh(void)
{
    meshInfo.elementDimension.X = DC_get_appSettings_settingsData_value_meshsize();
    meshInfo.elementDimension.Y = DC_get_appSettings_settingsData_value_meshsize();

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
        type = (abs(a) <= MAP_ROUTE_A_COEFF_LIMIT) ? MAP_FUNCTION_Y_FX : MAP_FUNCTION_X_FY;
    }
    else if(curr.Y != prev.Y)
    {
        a = ((float)(curr.X-prev.X)) / ((float)(curr.Y-prev.Y));
        type = (abs(a) <= MAP_ROUTE_A_COEFF_LIMIT) ? MAP_FUNCTION_X_FY : MAP_FUNCTION_Y_FX;
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


/* Method called to set map or track index to
   route structure to distunguish which points
   belong to whom. */
void Map::IncrementRouteIdx(Map_DrawRoute_T route, uint8_t addedPoints)
{
    if(MAP_DRAWROUTE_MAP == route)
    {
        routeList.idxMap = addedPoints;
    }
    else if(MAP_DRAWROUTE_TRACK == route)
    {
        routeList.idxTrack = routeList.idxMap + addedPoints;
    }
}


/* Method called to check whether given coords are in
   in invalidated area. */
bool Map::IsInvalidatedArea(uint8_t right, uint8_t bottom, Map_CoordinatesXY_U16_T coordsCurr) const
{
    bool retVal = false;

    if( (coordsCurr.X < right) && (coordsCurr.Y < bottom) )
    {
        retVal = true;
    }
    else
    {
        retVal = false;
    }

    return retVal;
}

} // namespace touchgfx
