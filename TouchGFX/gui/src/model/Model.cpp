#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0), modelTicks(0)
{

}

void Model::tick()
{
    modelTicks++;
    if(modelTicks >= MODEL_GPSDATA_TIME_INTERVAL)
    {
        modelListener->tickDebugScreen();
        modelTicks = 0u;
    }
    else
    {
        /* wait */
    }
    
}
