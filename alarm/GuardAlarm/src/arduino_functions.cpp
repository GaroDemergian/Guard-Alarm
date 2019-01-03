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

char portNo[] = "\\\\.\\COM5";
char *port_name = portNo;
//string for incoming data
char incomingData[MAX_DATA_LENGTH];
char output[MAX_DATA_LENGTH];
//Serial object
SerialPort arduino(port_name);

string getInput()
{
    //while connected
    while (arduino.isConnected())
    {
        int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
        if (read_result > 0)
        {
            //print out data
            return incomingData;
        }
        else
            Sleep(1000);
    }
    return 0;
}

void send(string msg)
{

    int msgSent = 1;

    while (arduino.isConnected() && msgSent == 1)
    {
        //Creating a c string
        char *c_string = new char[msg.size() + 1];
        //copying the string to c string
        copy(msg.begin(), msg.end(), c_string);
        //Adding the delimiter
        c_string[msg.size()] = '\n';
        //Writing string to arduino
        arduino.writeSerialPort(c_string, MAX_DATA_LENGTH);
        //freeing c_string memory
        delete[] c_string;
        msgSent++;
    }
}

void sensorlocked()
{
    cout << "****ALARM****" << endl;
    cout << "Calling Security" << endl;
    for (int i = 0; i < 1;)
    {
        string sensor = getSensorInfo();
        if (sensor == "b")
            systemLog("Blue", 0);
        else if (sensor == "r")
            systemLog("Red", 0);
        Sleep(1000);
    } // The system stays in the loop.
}

void checkSensor()
{
    string sensor = getSensorInfo();
    if (sensor == "b")
    {
        systemLog("Blue", 0);
        sensorlocked();
    }
    else if (sensor == "r")
    {
        systemLog("Red", 0);
        sensorlocked();
    }
}

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
    outFile << rows + 1 << ";" << date << ";" << sensor << ";" << status << '\n';

    outFile.close();
}

string getSensorInfo()
{
    int i = 0;
    while (arduino.isConnected() && i < 1)
    {
        int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
        if (read_result > 0)
            return incomingData;
        else
            Sleep(1000);
        i++;
    }
    return 0;
}
