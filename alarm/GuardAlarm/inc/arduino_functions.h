#ifndef ARDUINO_FUNCTIONS_H
#define ARDUINO_FUNCTIONS_H

using namespace std;

string getInput();
void send(string msg);
void sensorlocked();
void checkSensor();
void systemLog(string sensor, int status);
string getSensorInfo();


#endif
