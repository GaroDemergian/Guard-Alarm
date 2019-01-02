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
