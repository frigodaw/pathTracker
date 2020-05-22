#include <gui/containers/ActivityDataContainter_Map.hpp>

ActivityDataContainter_Map::ActivityDataContainter_Map()
{
    Map.setPosition(0, 0, getWidth(), getHeight());
    Map.setVisible(true);
    Map.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Map.SetMesh();

    add(Map);
}

void ActivityDataContainter_Map::initialize()
{
    ActivityDataContainter_MapBase::initialize();
}


void ActivityDataContainter_Map::FlushTrackList(void)
{
    Map.FlushTrackList();
}


void ActivityDataContainter_Map::AddCoordsToTrackList(uint8_t x, uint8_t y)
{
    Map.AddCoordsToTrackList(x, y);
}


void ActivityDataContainter_Map::TrackRedraw(void)
{
    Map.invalidate();
}