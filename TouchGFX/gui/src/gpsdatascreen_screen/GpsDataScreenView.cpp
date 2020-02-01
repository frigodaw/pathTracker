#include <gui/gpsdatascreen_screen/GpsDataScreenView.hpp>
#include <../../Core/Inc/neoGps.h>

GpsDataScreenView::GpsDataScreenView()
{

}

void GpsDataScreenView::setupScreen()
{
    GpsDataScreenViewBase::setupScreen();
}

void GpsDataScreenView::tearDownScreen()
{
    GpsDataScreenViewBase::tearDownScreen();
}

void GpsDataScreenView::updateGpsDataContainer()
{
    Unicode::snprintf(TimeTextBoxBuffer, TIMETEXTBOX_SIZE, "%.2d:%.2d:%.2d", gpsData.timeHr, gpsData.timeMin, gpsData.timeSec);
    TimeTextBox.invalidate();
    Unicode::snprintfFloats(LatTextBoxBuffer1, LATTEXTBOXBUFFER1_SIZE, "%#.5f", &gpsData.latitude);
    Unicode::snprintf(LatTextBoxBuffer2, LATTEXTBOXBUFFER2_SIZE, "%c", gpsData.latDir);
    LatTextBox.invalidate();
    Unicode::snprintfFloats(LonTextBoxBuffer1, LONTEXTBOXBUFFER1_SIZE, "%#.5f", &gpsData.longitude);
    Unicode::snprintf(LonTextBoxBuffer2, LONTEXTBOXBUFFER2_SIZE, "%c", gpsData.lonDir);
    LonTextBox.invalidate();
    Unicode::snprintfFloats(AltiTextBoxBuffer, ALTITEXTBOX_SIZE, "%#.1f", &gpsData.altitude);
    AltiTextBox.invalidate();
    Unicode::snprintf(FixTextBoxBuffer, FIXTEXTBOX_SIZE, "%d", gpsData.fixQuality);
    FixTextBox.invalidate();
    Unicode::snprintf(SatTextBoxBuffer, SATTEXTBOX_SIZE, "%d", gpsData.satelitesNum);
    SatTextBox.invalidate();
}
