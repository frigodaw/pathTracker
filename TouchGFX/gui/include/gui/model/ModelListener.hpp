#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>
#include <touchgfx/hal/Types.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    /* neoGps */
    virtual void NotifySignalChanged_gpsData_latitude(float newLatitude){}
    virtual void NotifySignalChanged_gpsData_longitude(float newLongitude){}
    virtual void NotifySignalChanged_gpsData_altitude(float newAltitude){}
    virtual void NotifySignalChanged_gpsData_time(uint32_t newTime){}
    virtual void NotifySignalChanged_gpsData_date(uint32_t newDate){}
    virtual void NotifySignalChanged_gpsData_read(uint16_t newRead){}
    virtual void NotifySignalChanged_gpsData_write(uint16_t newWrite){}
    virtual void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality){}
    virtual void NotifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum){}
    virtual void NotifySignalChanged_gpsData_lonDir(char newLonDir){}
    virtual void NotifySignalChanged_gpsData_latDir(char newLatDir){}
    /* fileSystem */
    virtual void NotifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace){}
    virtual void NotifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace){}
    virtual void NotifySignalChanged_sdCardInfo_state(uint8_t newState){}
    virtual void NotifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum){}
    virtual void NotifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum){}
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
