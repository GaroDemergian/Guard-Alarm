#ifndef ARDUINO_FUNCTIONS_H
#define ARDUINO_FUNCTIONS_H

using namespace std;

string getInput(class SerialPort *arduino);
void send(class SerialPort *arduino, string msg);
void sensorlocked(class SerialPort *arduino);
void checkSensor(class SerialPort *arduino, string sensor);
void systemLog(string sensor, int status);
void check(string sensor);
string getSensorInfo(class SerialPort *arduino);

#endif
