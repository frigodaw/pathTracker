#include <gui/containers/GpsDataContainer.hpp>

GpsDataContainer::GpsDataContainer()
{

}

void GpsDataContainer::initialize()
{
    GpsDataContainerBase::initialize();
}


void GpsDataContainer::notifySignalChanged_gpsData_latitude(float newLatitude)
{
    Unicode::snprintfFloats(LatTextBoxBuffer1, LATTEXTBOXBUFFER1_SIZE, "%#.5f", &newLatitude);
    LatTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_longitude(float newLongitude)
{
    Unicode::snprintfFloats(LonTextBoxBuffer1, LONTEXTBOXBUFFER1_SIZE, "%#.5f", &newLongitude);
    LonTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_altitude(float newAltitude)
{
    Unicode::snprintfFloats(AltiTextBoxBuffer, ALTITEXTBOX_SIZE, "%#.1f", &newAltitude);
    AltiTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_read(uint16_t newRead)
{
    Unicode::snprintf(ReadTextBoxBuffer, READTEXTBOX_SIZE, "%d", newRead);
    ReadTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_write(uint16_t newWrite)
{
    Unicode::snprintf(WriteTextBoxBuffer, WRITETEXTBOX_SIZE, "%d", newWrite);
    WriteTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_time(uint8_t newTimeHr, uint8_t newTimeMin, uint8_t newTimeSec)
{
    Unicode::snprintf(TimeTextBoxBuffer, TIMETEXTBOX_SIZE, "%.2d:%.2d:%.2d", newTimeHr, newTimeMin, newTimeSec);
    TimeTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_date(uint8_t newDateDay, uint8_t newDateMon, uint8_t newDateYear)
{
    Unicode::snprintf(DateTextBoxBuffer, DATETEXTBOX_SIZE, "%.2d.%.2d.%.2d", newDateDay, newDateMon, newDateYear);
    DateTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    Unicode::snprintf(FixTextBoxBuffer, FIXTEXTBOX_SIZE, "%d", newFixQuality);
    FixTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum)
{
    Unicode::snprintf(SatTextBoxBuffer, SATTEXTBOX_SIZE, "%d", newSatellitesNum);
    SatTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_lonDir(char newLonDir)
{
    Unicode::snprintf(LonTextBoxBuffer2, LONTEXTBOXBUFFER2_SIZE, "%c", newLonDir);
    LonTextBox.invalidate();
}


void GpsDataContainer::notifySignalChanged_gpsData_latDir(char newLatDir)
{
    Unicode::snprintf(LatTextBoxBuffer2, LATTEXTBOXBUFFER2_SIZE, "%c", newLatDir);
    LatTextBox.invalidate();
}