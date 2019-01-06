#ifndef ARDUINO_FUNCTIONS_H
#define ARDUINO_FUNCTIONS_H

using namespace std;


//recieves data from arduino and decides what to do with it.
string getInput(class SerialPort *arduino);
//function used to send data (string) to arduino using SerialPort 
void send(class SerialPort *arduino, string msg);
//function for locking the system if sensor signal was detected.
void sensorlocked(class SerialPort *arduino);
//calls other functions to read sensor data incoming from arduino and decides what to do with it.
void checkSensor(class SerialPort *arduino, string sensor);
//funtion for writing text about sensor info in log file.
void systemLog(string sensor, int status);
void check(string sensor);
//this function is used by checkSensor(). it reads data incoming from arduino
string getSensorInfo(class SerialPort *arduino);

#endif
