#include <gui/containers/ActivityDataContainter_Map.hpp>

ActivityDataContainter_Map::ActivityDataContainter_Map()
{
    Map.setPosition(0, 0, getWidth(), getHeight());
    Map.setVisible(true);
    Map.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Map.SetMesh();

    ArrowIcon.setVisible(false);

    /* Map widget have to be placed as a first element in Drawable list to prevent overwrite scale text */
    insert(0, Map);
}

void ActivityDataContainter_Map::initialize()
{
    ActivityDataContainter_MapBase::initialize();
}


void ActivityDataContainter_Map::FlushRouteList(void)
{
    Map.FlushRouteList();
}


void ActivityDataContainter_Map::SetRouteScale(uint32_t scaleVal)
{
    Map.SetRouteScale(scaleVal);

    Unicode::UnicodeChar unit[SCALETEXTBOXBUFFER2_SIZE] = {0u};

    if(scaleVal >= MAPDATACONTAINER_SCALE_THRESHOLD)
    {
        Unicode::fromUTF8((const uint8_t*)"km", unit, SCALETEXTBOXBUFFER2_SIZE);

        /* Detect thousands */
        if((scaleVal % MAPDATACONTAINER_SCALE_THRESHOLD) == 0)
        {
            scaleVal /= MAPDATACONTAINER_SCALE_THRESHOLD;
            Unicode::snprintf(ScaleTextBoxBuffer1, SCALETEXTBOXBUFFER1_SIZE, "%d", scaleVal);
        }
        /* Detect custom scale and round to hundreds */
        else
        {
            float floatScaleVal = ((float)(scaleVal / MAPDATACONTAINER_SCALE_THRESHOLD_FULLVIEW_COEEF1)) / ((float)MAPDATACONTAINER_SCALE_THRESHOLD_FULLVIEW_COEEF2);
            Unicode::snprintfFloats(ScaleTextBoxBuffer1, SCALETEXTBOXBUFFER1_SIZE, "%#.1f", &floatScaleVal);
        }
    }
    else
    {
        Unicode::fromUTF8((const uint8_t*)"m", unit, SCALETEXTBOXBUFFER2_SIZE);
        Unicode::snprintf(ScaleTextBoxBuffer1, SCALETEXTBOXBUFFER1_SIZE, "%d", scaleVal);
    }

    Unicode::snprintf(ScaleTextBoxBuffer2, SCALETEXTBOXBUFFER2_SIZE, "%s", unit);

    TrackRedraw();
}


bool ActivityDataContainter_Map::AddCoordsToRouteList(uint8_t x, uint8_t y, Map_DrawRoute_T route)
{
    bool newPoint = Map.AddCoordsToRouteList(x, y, route);
    return newPoint;
}


void ActivityDataContainter_Map::TrackRedraw(void)
{
    Map.invalidate();
    ScaleTextBox.invalidate();
    ArrowIcon.invalidate();
}

void ActivityDataContainter_Map::IncrementRouteIdx(Map_DrawRoute_T route, uint8_t addedPoints)
{
    Map.IncrementRouteIdx(route, addedPoints);
}

void ActivityDataContainter_Map::SetArrowVisibilityStatus(bool visibilityStatus)
{
    ArrowIcon.setVisible(visibilityStatus);
    ArrowIcon.invalidate();
}

void ActivityDataContainter_Map::SetArrowAngle(float angle)
{
    ArrowIcon.updateZAngle(angle);
    ArrowIcon.invalidate();
}