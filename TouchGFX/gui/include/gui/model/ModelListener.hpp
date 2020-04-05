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
    virtual void notifySignalChanged_gpsData_latitude(float newLatitude){}
    virtual void notifySignalChanged_gpsData_longitude(float newLongitude){}
    virtual void notifySignalChanged_gpsData_altitude(float newAltitude){}
    virtual void notifySignalChanged_gpsData_read(uint16_t newRead){}
    virtual void notifySignalChanged_gpsData_write(uint16_t newWrite){}
    virtual void notifySignalChanged_gpsData_time(uint8_t newTimeHr, uint8_t newTimeMin, uint8_t newTimeSec){}
    virtual void notifySignalChanged_gpsData_date(uint8_t newDateDay, uint8_t newDateMon, uint8_t newDateYear){}
    virtual void notifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality){}
    virtual void notifySignalChanged_gpsData_satellitesNum(uint8_t newSatellitesNum){}
    virtual void notifySignalChanged_gpsData_lonDir(char newLonDir){}
    virtual void notifySignalChanged_gpsData_latDir(char newLatDir){}
    /* fileSystem */
    virtual void notifySignalChanged_sdCardInfo_totalSpace(uint32_t newTotalSpace){}
    virtual void notifySignalChanged_sdCardInfo_freeSpace(uint32_t newFreeSpace){}
    virtual void notifySignalChanged_sdCardInfo_state(uint8_t newState){}
    virtual void notifySignalChanged_dirInfo_in_filesNum(uint8_t newInFilesNum){}
    virtual void notifySignalChanged_dirInfo_out_filesNum(uint8_t newOutFilesNum){}
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
