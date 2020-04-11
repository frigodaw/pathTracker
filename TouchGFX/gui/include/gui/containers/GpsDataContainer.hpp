#ifndef GPSDATACONTAINER_HPP
#define GPSDATACONTAINER_HPP

#include <gui_generated/containers/GpsDataContainerBase.hpp>

#define GPSDATACONTAINER_TIME_COEFF1             100u
#define GPSDATACONTAINER_TIME_COEFF2             10000u
#define GPSDATACONTAINER_DATE_COEFF1             100u
#define GPSDATACONTAINER_DATE_COEFF2             10000u

class GpsDataContainer : public GpsDataContainerBase
{
public:
    GpsDataContainer();
    ~GpsDataContainer() {}
    void initialize();

    void NotifySignalChanged_gpsData_latitude(float newLatitude);
    void NotifySignalChanged_gpsData_longitude(float newLongitude);
    void NotifySignalChanged_gpsData_altitude(float newAltitude);
    void NotifySignalChanged_gpsData_time(uint32_t newTime);
    void NotifySignalChanged_gpsData_date(uint32_t newDate);
    void NotifySignalChanged_gpsData_read(uint16_t newRead);
    void NotifySignalChanged_gpsData_write(uint16_t newWrite);
    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum);
    void NotifySignalChanged_gpsData_lonDir(char newLonDir);
    void NotifySignalChanged_gpsData_latDir(char newLatDir);
protected:
};

#endif // GPSDATACONTAINER_HPP
