#ifndef ARDUINO_FUNCTIONS_H
#define ARDUINO_FUNCTIONS_H

using namespace std;

string getInput(class SerialPort *arduino, char incomingData[]);
void send(class SerialPort *arduino, string msg);
void sensorlocked(class SerialPort *arduino, char incomingData[]);
void checkSensor(class SerialPort *arduino, char incomingData[]);
void systemLog(string sensor, int status);
string getSensorInfo(class SerialPort *arduino, char incomingData[]);


#endif
