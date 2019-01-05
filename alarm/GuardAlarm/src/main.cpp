/**********************************
* Filename: main.cpp
* 
* author: Garo
* created: 2018-10-16
* notes: 
* 
* desc: 
* 
* ver: 2018-10-16 first version
* 
* 
* **********************************/
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "functions.h"
#include "User.h"
#include "arduino_functions.h"
#include "SerialPort.h"

char portNo[] = "\\\\.\\COM5";
char *port_name = portNo;
//string for incoming data
char incomingData[MAX_DATA_LENGTH];
char output[MAX_DATA_LENGTH];
//Serial object
SerialPort arduino(port_name);

int main()
{
    while (arduino.isConnected())
    {
        struct User activeUser;

        const bool workingAlarm = true; //workingAlarm should never be false.
        bool guarding = true;
        string input;
        int usersInput;

        //this loop will keep the alarm system program running.
        while (workingAlarm == true)
        {
            int tries = 0;
            //this loop is for deactivating and activating the alarm.
            //the alarm is active while guarding is true.
            while (guarding == true)
            {
                cout << endl;
                cout << "Alarm activated " << endl;
                cout << "Enter pincode " << endl;
                input = "";

                //this function is mainly for stopping the user from entering a wrong pincode more than 3 times.
                while (guarding == true && tries < 3)
                {

                    input = getInput(&arduino, incomingData);
                    checkSensor(&arduino, incomingData);
                    if (input.length() > 0)
                    {
                        usersInput = stoi(input);
                        //If input == any of pincodes saved in file and user isnt blocked => menu.
                        //if not then try again, max 3 tries.
                        logging_in(&arduino, usersInput, &activeUser, guarding);
                        tries++;
                        if (guarding == true && tries < 3)
                        {
                            systemLog(4, "Unknown");
                            cout << "Try again!" << endl;
                            break;
                        }
                    }
                    Sleep(1000);
                }
                //3 wrong tries and the system get locked.
                if (guarding == true && tries == 3)
                    locked(&arduino, incomingData);
            }
            cout << "Alarm is deactivated" << endl;
            userWelcomeMsg(activeUser);
            // A menu or choice for the user.
            int tries2 = 0;
            cout << endl;

            //x = stoi(getInput());
            while (guarding == false)
            {
                while (guarding == false && tries < 3)
                {
                    cout << endl;
                    cout << "To activate outside and inside alarms press A then your pin code " << endl;
                    cout << "To activate only outside alarm press B then your pin code " << endl;
                    while (guarding == false)
                    {
                        input = getInput(&arduino, incomingData);
                        if (input.length() > 0)
                        {
                            usersInput = stoi(input);
                            //If input == any of pincodes saved in file and user isnt blocked => menu.
                            //if not then try again, max 3 tries.
                            logging_in(&arduino, usersInput, &activeUser, guarding);
                        }
                        Sleep(1000);
                    }
                    logging_out(&arduino, usersInput, &activeUser, guarding);
                    tries2++;
                    if (guarding == false && tries < 3)
                    {
                        systemLog(6, "Unknown");
                        cout << "Try again!" << endl;
                        break;
                    }
                }
                if (guarding == false && tries2 == 3)
                {
                    locked(&arduino, incomingData);
                }
            }
            break;
        }
    }
    return 0;
}
