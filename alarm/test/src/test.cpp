#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <time.h>
#include <cstdio>
#include <cstdlib>

#include "SerialPort.h"

using namespace std;

char portNo[] = "\\\\.\\COM5";
char *port_name = portNo;
//string for incoming data
char incomingData[MAX_DATA_LENGTH];
//Serial object
SerialPort arduino(port_name);

string getSensorInfo();
void sensorlocked();
string getInput();
void send(string msg);
void systemLog(string sensor, int status);
void checkSensor();

int main()
{

    string choice;
    int pin;
    string input;
    int pincode = 1234;
    bool onA = true;
    bool onB = false;
    bool off = false;

    while (arduino.isConnected())
    {
        int i = 0;
        while (onA)
        {
            while (i < 1)
            {
                cout << "enter pincode" << endl;
                i++;
            }
            input = getInput();
            checkSensor();
            if (input.length() > 0)
            {
                cout << " input is: " << input << endl;
                pin = stoi(input);
                if (pin == pincode)
                {
                    cout << pin << endl;
                    send("off");
                    onA = false;
                    off = true;
                }
                else
                    cout << "wrong pincode, try again!" << endl;
            }
        }
        i = 0;
        while (onB)
        {
        }
        i = 0;
        while (off)
        {
            while (i < 1)
            {
                cout << "press A then pincode to activate a alarm " << endl;
                cout << "press B then pincode to activate b alarm " << endl;
                i++;
            }
            input = getInput();
            if (input.length() > 0)
            {
                choice = input;
                if (choice == "A")
                {
                    if (input.length() > 0)
                    {
                        pin = stoi(input);
                        if (pin == pincode)
                        {
                            send("on");
                            onA = true;
                            off = false;
                        }
                        else
                            cout << "wrong pincode, try again!" << endl;
                    }
                }
            }
            else if (choice == "B")
            {
                if (input.length() > 0)
                {
                    pin = stoi(input);
                    if (pin == pincode)
                    {
                        send("on");
                        onB = true;
                        off = false;
                    }
                    else
                        cout << "wrong pincode, try again!" << endl;
                }
                else
                    cout << "not a valid choice! " << endl;
            }
        }
    }

    return 0;
}

string getInput()
{
    int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
    if (read_result > 0)
        return incomingData;
    if (incomingData == "r" || incomingData == "b")
        return "";
    return "";
}
void send(string msg)
{

    int msgSent = 1;

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

    system_log.open("../test/bin/sensor.log");

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

    outFile.open("../test/bin/sensor.log", ofstream::app);
    outFile << rows + 1 << ";" << date << ";" << sensor << ";" << status << '\n';

    outFile.close();
}
string getSensorInfo()
{
    int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
    if (read_result > 0)
        return incomingData;
    return "";
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
    }
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
/*
string readInput()
{
    int i = 0;
    while (i < 1)
    {
    string input, choice, pinCode;
    int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
        if (read_result > 0)
            input = incomingData;
        if (input == "A" || input == "B")
        {
            i++;
            choice = input;
            return choice;
        }
        else if (input.length() >= 4)
        {
            i++;
            pinCode = input;
            return pinCode;
        }
    }
}
*/