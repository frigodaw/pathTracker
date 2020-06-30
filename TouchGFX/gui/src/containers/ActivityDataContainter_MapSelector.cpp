#include <gui/containers/ActivityDataContainter_MapSelector.hpp>

ActivityDataContainter_MapSelector::ActivityDataContainter_MapSelector()
{

}

void ActivityDataContainter_MapSelector::initialize()
{
    ActivityDataContainter_MapSelectorBase::initialize();
}

void ActivityDataContainter_MapSelector::ChangeMapDistance(float distance)
{
    Unicode::snprintfFloats(DistanceTextBoxBuffer, DISTANCETEXTBOX_SIZE, "%#.1f", &distance);
    DistanceTextBox.invalidate();
}