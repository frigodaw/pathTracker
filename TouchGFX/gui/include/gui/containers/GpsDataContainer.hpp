#ifndef GPSDATACONTAINER_HPP
#define GPSDATACONTAINER_HPP

#include <gui_generated/containers/GpsDataContainerBase.hpp>

class GpsDataContainer : public GpsDataContainerBase
{
public:
    GpsDataContainer();
    virtual ~GpsDataContainer() {}

    virtual void initialize();

    virtual void updateGpsDataContainer();
protected:
};

#endif // GPSDATACONTAINER_HPP
