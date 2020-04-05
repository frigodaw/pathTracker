#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "cmsis_os.h"
#include "dataCollector.h"

extern osMessageQId Queue_gpsData_ReadHandle;
extern osMessageQId Queue_gpsData_WriteHandle;


/* Constructor of the class Model */
Model::Model() : modelListener(0), modelTicks(0)
{

}


/* Method called to bind current presenter
   to modelListener */
void Model::bind(ModelListener* listener)
{
    modelListener = listener;
}


/* Method called about 60 times per second,
   its purpose is to be like 'Main' function */
void Model::tick(void)
{
    ReadInputSignals();

    //NotifySignalsChanged();

    RefreshScreens();
}


/* Method called to read all input signals
   from peripherals and save them inside the object */
void Model::ReadInputSignals(void)
{
    /* neoGps */
    gpsData_latitude = DC_get_neoGps_gpsData_latitude();
    gpsData_longitude = DC_get_neoGps_gpsData_longitude();
    gpsData_altitude = DC_get_neoGps_gpsData_altitude();
    gpsData_read = DC_get_neoGps_gpsData_read();
    gpsData_write = DC_get_neoGps_gpsData_write();
    gpsData_timeHr = DC_get_neoGps_gpsData_timeHr();
    gpsData_timeMin = DC_get_neoGps_gpsData_timeMin();
    gpsData_timeSec = DC_get_neoGps_gpsData_timeSec();
    gpsData_dateDay = DC_get_neoGps_gpsData_dateDay();
    gpsData_dateMon = DC_get_neoGps_gpsData_dateMon();
    gpsData_dateYear = DC_get_neoGps_gpsData_dateYear();
    gpsData_fixQuality = DC_get_neoGps_gpsData_fixQuality();
    gpsData_satellitesNum = DC_get_neoGps_gpsData_satellitesNum();
    gpsData_lonDir = DC_get_neoGps_gpsData_lonDir();
    gpsData_latDir = DC_get_neoGps_gpsData_latDir();

    /* fileSystem */
    sdCardInfo_totalSpace = DC_get_fileSystem_sdCardInfo_totalSpace();
    sdCardInfo_freeSpace = DC_get_fileSystem_sdCardInfo_freeSpace();
    sdCardInfo_state = DC_get_fileSystem_sdCardInfo_state();
    dirInfo_in_filesNum = DC_get_fileSystem_dirInfo_in_filesNum();
    dirInfo_out_filesNum = DC_get_fileSystem_dirInfo_out_filesNum();
}


/* Method called to call periodically other 
   functions which can refresh data on the screen */
void Model::RefreshScreens(void)
{
    modelTicks++;
    if(modelTicks >= MODEL_GPSDATA_TIME_INTERVAL)
    {
        NotifySignalsChanged();
        modelTicks = 0u;
    }
    else
    {
        /* wait */
    }
}


/* Method called to inform active presenter that some
   signals have changes and give it a new value */
void Model::NotifySignalsChanged(void)
{
    if(modelListener != NULL)
    {
        /* neoGps */
        modelListener->notifySignalChanged_gpsData_latitude(gpsData_latitude);
        modelListener->notifySignalChanged_gpsData_longitude(gpsData_longitude);
        modelListener->notifySignalChanged_gpsData_altitude(gpsData_altitude);
        modelListener->notifySignalChanged_gpsData_read(gpsData_read);
        modelListener->notifySignalChanged_gpsData_write(gpsData_write);
        modelListener->notifySignalChanged_gpsData_time(gpsData_timeHr, gpsData_timeMin, gpsData_timeSec);
        modelListener->notifySignalChanged_gpsData_date(gpsData_dateDay, gpsData_dateMon, gpsData_dateYear);
        modelListener->notifySignalChanged_gpsData_fixQuality(gpsData_fixQuality);
        modelListener->notifySignalChanged_gpsData_satellitesNum(gpsData_satellitesNum);
        modelListener->notifySignalChanged_gpsData_lonDir(gpsData_lonDir);
        modelListener->notifySignalChanged_gpsData_latDir(gpsData_latDir);

        /* fileSystem */
        modelListener->notifySignalChanged_sdCardInfo_totalSpace(sdCardInfo_totalSpace);
        modelListener->notifySignalChanged_sdCardInfo_freeSpace(sdCardInfo_freeSpace);
        modelListener->notifySignalChanged_sdCardInfo_state(sdCardInfo_state);
        modelListener->notifySignalChanged_dirInfo_in_filesNum(dirInfo_in_filesNum);
        modelListener->notifySignalChanged_dirInfo_out_filesNum(dirInfo_out_filesNum);
    }
}
