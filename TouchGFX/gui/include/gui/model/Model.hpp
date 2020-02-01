#ifndef MODEL_HPP
#define MODEL_HPP

#define MODEL_TICKS_PER_SECOND          60u
#define MODEL_GPSDATA_TIME_INTERVAL     15u     //15:60 = 0.25s = 250ms

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
protected:
    ModelListener* modelListener;
    unsigned short modelTicks;
};

#endif // MODEL_HPP
