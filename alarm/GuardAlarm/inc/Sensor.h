
#ifndef SENSOR_H
#define SENSOR_H

using namespace std;

enum SensorStatus
{
    Open = 1,
    Close = 0,
    Fail = -1
};
/*for future developing
struct Node
{
    struct Sensor;
    struct Node *nextSensor;
};*/

struct Sensor
{
    SensorStatus status;
};


#endif
