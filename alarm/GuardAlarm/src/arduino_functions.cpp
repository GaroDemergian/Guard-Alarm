#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <time.h>
#include <cstdio>
#include <cstdlib>

#include "SerialPort.h"
#include "functions.h"
#include "User.h"
#include "arduino_functions.h"

using namespace std;


//recieves data from arduino and decides what to do with it.
string getInput(class SerialPort *arduino)
{
    //string for incoming data
    char incomingData[MAX_DATA_LENGTH];
    
    string sensor;
    string empty = "";
    //reads data and data size.
    int read_result = (*arduino).readSerialPort(incomingData, MAX_DATA_LENGTH);
    if  (read_result == 1)//if data size == 1 //one char
    {
        checkSensor(arduino, incomingData); //send data to sensor function.
    }
    else if (read_result == 4) //if data size is 4 chars
        return incomingData; // returna data
    
    return empty; //else returns an empty string
}

//function used to send data (string) to arduino using SerialPort 
void send(class SerialPort *arduino, string msg)
{
    char output[MAX_DATA_LENGTH];

    //Creating a c string
    char *c_string = new char[msg.size() + 1];
    //copying the string to c string
    copy(msg.begin(), msg.end(), c_string);
    //Adding the delimiter
    c_string[msg.size()] = '\n';
    //Writing string to arduino
    (*arduino).writeSerialPort(c_string, MAX_DATA_LENGTH);
    //freeing c_string memory
    delete[] c_string;
}

//function for locking the system if sensor signal was detected.
void sensorlocked(class SerialPort *arduino)
{
    systemLog(12, "Unknown");
    cout << endl; cout << endl;
    cout << "*  *   *   *   ALARM   *   *   *   *" << endl;
    cout << "   *   *  Calling Security *   *   " << endl;
    cout << endl; cout << endl;
    for (int i = 0; i < 1;)
    {
        string sensor = getSensorInfo(arduino); //get sensor data incoming from arduino
        if (sensor == "b")
            systemLog("Blue", 0); //logs the event if blur sensor was pushed
        else if (sensor == "r")
            systemLog("Red", 0); //logs the event if red sensor was pushed
        Sleep(3000);
    } // The system stays in the loop.
}


void checkSensor(class SerialPort *arduino, string sensor)
{
    if (sensor == "b")
    {
        systemLog("Blue", 0);
        sensorlocked(arduino);
    }
    else if (sensor == "r")
    {
        systemLog("Red", 0);
        sensorlocked(arduino);
    }
    else if (sensor == "ok")
        systemLog("All Sensors", 1);
}

//funtion for writing text about sensor info in log file.
void systemLog(string sensor, int status)
{
    ofstream outFile;
    ifstream system_log;
    char c;
    int rows = 0;

    //the next 6 lines with get the date from system to program
    time_t now = time(0);
    struct tm tstruct;
    char date[80];
    tstruct = *localtime(&now);
    strftime(date, sizeof(date), "%Y%m%d %X", &tstruct);
    string date_now = date;

    system_log.open("../GuardAlarm/bin/sensor.log");

    if (system_log.is_open())
    {
        while (system_log.get(c))
        {
            if (c == '\n')
            {
                rows++;
            }
        }
        system_log.close();
    }

    outFile.open("../GuardAlarm/bin/sensor.log", ofstream::app);
    //line example: 1;20190104 20:04:13;Red;0
    outFile << rows + 1 << ";" << date << ";" << sensor << ";" << status << '\n';

    outFile.close();
}

//this function is used by checkSensor(). it reads data incoming from arduino
string getSensorInfo(class SerialPort *arduino)
{
    char incomingData[MAX_DATA_LENGTH];
    int read_result = (*arduino).readSerialPort(incomingData, MAX_DATA_LENGTH);
    if (read_result > 0)
        return incomingData;
    return "";
}
