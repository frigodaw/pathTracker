#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <string.h>
#include "cmsis_os.h"
#include "dataCollector.h"



/* Constructor of the class Model. */
Model::Model() : modelListener(0), modelTicks(0)
{
    dataNotifier = {0u};
}


/* Method called to bind current presenter
   to modelListener. */
void Model::bind(ModelListener* listener)
{
    modelListener = listener;
}


/* Method called about 60 times per second,
   its purpose is to be like 'Main' function. */
void Model::tick(void)
{
    ReadInputSignals();

    NotifyScreens();

    TryCallPresenterMainFunction();
}


/* Method called to read all input signals
   from peripherals and save them inside the object. */
void Model::ReadInputSignals(void)
{
    /* neoGps */
    UpdateElement<float>(DC_get_neoGps_gpsData_latitude, gpsData_latitude, dataNotifier.gpsData_latitude);
    UpdateElement<float>(DC_get_neoGps_gpsData_longitude, gpsData_longitude, dataNotifier.gpsData_longitude);
    UpdateElement<float>(DC_get_neoGps_gpsData_altitude, gpsData_altitude, dataNotifier.gpsData_altitude);
    UpdateElement<uint32_t>(DC_get_neoGps_gpsData_time, gpsData_time, dataNotifier.gpsData_time);
    UpdateElement<uint32_t>(DC_get_neoGps_gpsData_date, gpsData_date, dataNotifier.gpsData_date);
    UpdateElement<uint16_t>(DC_get_neoGps_gpsData_read, gpsData_read, dataNotifier.gpsData_read);
    UpdateElement<uint16_t>(DC_get_neoGps_gpsData_write, gpsData_write, dataNotifier.gpsData_write);
    UpdateElement<uint8_t>(DC_get_neoGps_gpsData_fixQuality, gpsData_fixQuality, dataNotifier.gpsData_fixQuality);
    UpdateElement<uint8_t>(DC_get_neoGps_gpsData_satellitesNum, gpsData_satellitesNum, dataNotifier.gpsData_satellitesNum);
    UpdateElement<char>(DC_get_neoGps_gpsData_lonDir, gpsData_lonDir, dataNotifier.gpsData_lonDir);
    UpdateElement<char>(DC_get_neoGps_gpsData_latDir, gpsData_latDir, dataNotifier.gpsData_latDir);

    /* fileSystem */
    UpdateElement<uint32_t>(DC_get_fileSystem_sdCardInfo_totalSpace, sdCardInfo_totalSpace, dataNotifier.sdCardInfo_totalSpace);
    UpdateElement<uint32_t>(DC_get_fileSystem_sdCardInfo_freeSpace, sdCardInfo_freeSpace, dataNotifier.sdCardInfo_freeSpace);
    UpdateElement<uint8_t>(DC_get_fileSystem_sdCardInfo_state, sdCardInfo_state, dataNotifier.sdCardInfo_state);
    UpdateElement<uint8_t>(DC_get_fileSystem_dirInfo_in_filesNum, dirInfo_in_filesNum, dataNotifier.dirInfo_in_filesNum);
    UpdateElement<uint8_t>(DC_get_fileSystem_dirInfo_out_filesNum, dirInfo_out_filesNum, dataNotifier.dirInfo_out_filesNum);

    /* envSensors */
    UpdateElement<float>(DC_get_sensorData_altitude, sensorData_altitude, dataNotifier.sensorData_altitude);
    UpdateElement<float>(DC_get_sensorData_pressure, sensorData_pressure, dataNotifier.sensorData_pressure);
    UpdateElement<float>(DC_get_sensorData_temperature, sensorData_temperature, dataNotifier.sensorData_temperature);
}


/* Method called to call periodically other 
   functions which can refresh data on the screen. */
void Model::TryCallPresenterMainFunction(void)
{
    uint16_t timePeriodInTicks = (mainTimePeriod * MODEL_TICKS_PER_SEC) / MODEL_MS_IN_SEC;
    modelTicks++;

    if(modelTicks >= timePeriodInTicks)
    {
        modelTicks = 0u;

        if(modelListener != NULL)
        {
            modelListener->Main();
        }
    }
    else
    {
        /* wait */
    }
}


/* Method called to inform active presenter that some
   signals have changed and give it a new value. */
void Model::NotifyScreens(void)
{
    if(modelListener != NULL)
    {
        /* neoGps */
        NotifyElement<float>(&ModelListener::NotifySignalChanged_gpsData_longitude, gpsData_longitude, dataNotifier.gpsData_longitude);
        NotifyElement<float>(&ModelListener::NotifySignalChanged_gpsData_latitude, gpsData_latitude, dataNotifier.gpsData_latitude);
        NotifyElement<float>(&ModelListener::NotifySignalChanged_gpsData_altitude, gpsData_altitude, dataNotifier.gpsData_altitude);
        NotifyElement<uint32_t>(&ModelListener::NotifySignalChanged_gpsData_time, gpsData_time, dataNotifier.gpsData_time);
        NotifyElement<uint32_t>(&ModelListener::NotifySignalChanged_gpsData_date, gpsData_date, dataNotifier.gpsData_date);
        NotifyElement<uint16_t>(&ModelListener::NotifySignalChanged_gpsData_read, gpsData_read, dataNotifier.gpsData_read);
        NotifyElement<uint16_t>(&ModelListener::NotifySignalChanged_gpsData_write, gpsData_write, dataNotifier.gpsData_write);
        NotifyElement<uint8_t>(&ModelListener::NotifySignalChanged_gpsData_fixQuality, gpsData_fixQuality, dataNotifier.gpsData_fixQuality);
        NotifyElement<uint8_t>(&ModelListener::NotifySignalChanged_gpsData_satellitesNum, gpsData_satellitesNum, dataNotifier.gpsData_satellitesNum);
        NotifyElement<char>(&ModelListener::NotifySignalChanged_gpsData_lonDir, gpsData_lonDir, dataNotifier.gpsData_lonDir);
        NotifyElement<char>(&ModelListener::NotifySignalChanged_gpsData_latDir, gpsData_latDir, dataNotifier.gpsData_latDir);

        /* fileSystem */
        NotifyElement<uint32_t>(&ModelListener::NotifySignalChanged_sdCardInfo_totalSpace, sdCardInfo_totalSpace, dataNotifier.sdCardInfo_totalSpace);
        NotifyElement<uint32_t>(&ModelListener::NotifySignalChanged_sdCardInfo_freeSpace, sdCardInfo_freeSpace, dataNotifier.sdCardInfo_freeSpace);
        NotifyElement<uint8_t>(&ModelListener::NotifySignalChanged_sdCardInfo_state, sdCardInfo_state, dataNotifier.sdCardInfo_state);
        NotifyElement<uint8_t>(&ModelListener::NotifySignalChanged_dirInfo_in_filesNum, dirInfo_in_filesNum, dataNotifier.dirInfo_in_filesNum);
        NotifyElement<uint8_t>(&ModelListener::NotifySignalChanged_dirInfo_out_filesNum, dirInfo_out_filesNum, dataNotifier.dirInfo_out_filesNum);

        /* envSensors */
        NotifyElement<float>(&ModelListener::NotifySignalChanged_sensorData_altitude, sensorData_altitude, dataNotifier.sensorData_altitude);
        NotifyElement<float>(&ModelListener::NotifySignalChanged_sensorData_pressure, sensorData_pressure, dataNotifier.sensorData_pressure);
        NotifyElement<float>(&ModelListener::NotifySignalChanged_sensorData_temperature, sensorData_temperature, dataNotifier.sensorData_temperature);
    }
}


/* Method called to acquire new data via variable
   getter from dataCollector. If new value if different
   than previous one, dataNotifier flag will be set
   to inform NotifyElement function. */
template <typename T>
void Model::UpdateElement(T (*getDataPtr)(void), T &currentData, uint8_t &dataNotifier)
{
    T newData = getDataPtr();
    if(newData != currentData)
    {
        currentData = newData;
        dataNotifier = true;
    }
    else
    {
        dataNotifier = false;
    }
}


/* Method called to notify active presenter
   if given variable has changed since
   last call. */
template <typename T>
void Model::NotifyElement(void (ModelListener::*notifySignalChangedElement)(T), T currentData, uint8_t &dataNotifier)
{
    if (true == dataNotifier)
    {
        (modelListener->*notifySignalChangedElement)(currentData);
        dataNotifier = false;
    }
}


/* Method called to set all dataNotifier structure
   to true to force Model to update current
   presenter with proper signal value. */
void Model::SignalRequestFromPresenter(void)
{
    memset(&dataNotifier, true, sizeof(dataNotifier));
    NotifyScreens();
}


/* Method called to get time interval needed
   to call presenter's main function. */
void Model::MainPeriodFromPresenter(uint16_t timePeriod)
{
    mainTimePeriod = timePeriod;
    modelTicks = 0u;
}
