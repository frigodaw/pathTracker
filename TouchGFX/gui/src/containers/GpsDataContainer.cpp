#include <gui/containers/GpsDataContainer.hpp>

GpsDataContainer::GpsDataContainer()
{

}

void GpsDataContainer::initialize()
{
    GpsDataContainerBase::initialize();
}


void GpsDataContainer::NotifySignalChanged_gpsData_latitude(float newLatitude)
{
    Unicode::snprintfFloats(LatTextBoxBuffer1, LATTEXTBOXBUFFER1_SIZE, "%#.5f", &newLatitude);
    LatTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_longitude(float newLongitude)
{
    Unicode::snprintfFloats(LonTextBoxBuffer1, LONTEXTBOXBUFFER1_SIZE, "%#.5f", &newLongitude);
    LonTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_altitude(float newAltitude)
{
    Unicode::snprintfFloats(AltiTextBoxBuffer, ALTITEXTBOX_SIZE, "%#.1f", &newAltitude);
    AltiTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_time(uint32_t newTime)
{
    uint8_t timeSec  = newTime % GPSDATACONTAINER_TIME_COEFF1;
    uint8_t timeMin = (newTime / GPSDATACONTAINER_TIME_COEFF1) % GPSDATACONTAINER_TIME_COEFF1;
    uint8_t timeHr = ((newTime / GPSDATACONTAINER_TIME_COEFF2) % GPSDATACONTAINER_TIME_COEFF1);
    Unicode::snprintf(TimeTextBoxBuffer, TIMETEXTBOX_SIZE, "%.2d:%.2d:%.2d", timeHr, timeMin, timeSec);
    TimeTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_date(uint32_t newDate)
{
    uint8_t dateYear = newDate % GPSDATACONTAINER_DATE_COEFF1;
    uint8_t dateMon  = (newDate / GPSDATACONTAINER_DATE_COEFF1) % GPSDATACONTAINER_DATE_COEFF1;
    uint8_t dateDay  = ((newDate / GPSDATACONTAINER_DATE_COEFF2) % GPSDATACONTAINER_DATE_COEFF1);
    Unicode::snprintf(DateTextBoxBuffer, DATETEXTBOX_SIZE, "%.2d.%.2d.%.2d", dateDay, dateMon, dateYear);
    DateTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_read(uint16_t newRead)
{
    Unicode::snprintf(ReadTextBoxBuffer, READTEXTBOX_SIZE, "%d", newRead);
    ReadTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_write(uint16_t newWrite)
{
    Unicode::snprintf(WriteTextBoxBuffer, WRITETEXTBOX_SIZE, "%d", newWrite);
    WriteTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality)
{
    Unicode::snprintf(FixTextBoxBuffer, FIXTEXTBOX_SIZE, "%d", newFixQuality);
    FixTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum)
{
    Unicode::snprintf(SatTextBoxBuffer, SATTEXTBOX_SIZE, "%d", newSatellitesNum);
    SatTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_lonDir(char newLonDir)
{
    Unicode::snprintf(LonTextBoxBuffer2, LONTEXTBOXBUFFER2_SIZE, "%c", newLonDir);
    LonTextBox.invalidate();
}


void GpsDataContainer::NotifySignalChanged_gpsData_latDir(char newLatDir)
{
    Unicode::snprintf(LatTextBoxBuffer2, LATTEXTBOXBUFFER2_SIZE, "%c", newLatDir);
    LatTextBox.invalidate();
}