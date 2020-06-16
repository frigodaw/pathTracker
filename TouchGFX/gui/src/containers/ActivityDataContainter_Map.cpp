#include <gui/containers/ActivityDataContainter_Map.hpp>

ActivityDataContainter_Map::ActivityDataContainter_Map()
{
    Map.setPosition(0, 0, getWidth(), getHeight());
    Map.setVisible(true);
    Map.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Map.SetMesh();

    /* Map widget have to be placed before ScaleTextBox to prevent overwrite */
    insert(0, Map);
}

void ActivityDataContainter_Map::initialize()
{
    ActivityDataContainter_MapBase::initialize();
}


void ActivityDataContainter_Map::FlushTrackList(void)
{
    Map.FlushTrackList();
}


void ActivityDataContainter_Map::SetTrackScale(uint32_t scaleVal)
{
    Map.SetTrackScale(scaleVal);

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


bool ActivityDataContainter_Map::AddCoordsToTrackList(uint8_t x, uint8_t y)
{
    bool newPoint = Map.AddCoordsToTrackList(x, y);
    return newPoint;
}


void ActivityDataContainter_Map::TrackRedraw(void)
{
    Map.invalidate();
    ScaleTextBox.invalidate();
}