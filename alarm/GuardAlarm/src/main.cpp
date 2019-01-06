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

int main()
{

    char portNo[] = "\\\\.\\COM5";
    char *port_name = portNo;
    
    //Serial object
    SerialPort arduino(port_name);
    struct User activeUser;

    const bool workingAlarm = true; //workingAlarm should never be false.
    bool guarding = true;
    string input;
    unsigned int usersInput;

    cout << endl;
    cout << " * * * Loading System * * * " << endl;
    cout << endl;
    Sleep(500);
    cout << "         *   *   *   " << endl;

    while (arduino.isConnected())
    {
        //this loop will keep the alarm system program running.
        while (workingAlarm == true)
        {
            send(&arduino, "on");
            int tries = 0;
            //this loop is for deactivating and activating the alarm.
            //the alarm is active while guarding is true.
            while (guarding == true)
            {
                cout << endl;
                cout << "         *   *   *   " << endl;
                cout << "Alarm activated " << endl;
                cout << "Enter pincode " << endl;

                //this loop is mainly for stopping the user from entering a wrong pincode more than 3 times.
                while (guarding == true && tries < 3)
                {
                    input = getInput(&arduino); //imports a string incoming from arduino and saves it into input.
                    if (input.length() == 4) // if input contains a string of 4 chars
                    {  
                        usersInput = stoi(input); //string to int and saves it into usersInput.
                        input = "0";

                        //If input == any of pincodes saved in file and user isnt blocked => menu.
                        //if not then try again, max 3 tries.
                        logging_in(usersInput, &activeUser, guarding);
                        tries++;
                        if (guarding == true && tries < 3)
                        {
                            systemLog(4, "Unknown");
                            cout << endl;
                            cout << "Wrong pincode! Try again!" << endl;
                            break;
                        }
                    }
                    Sleep(1000);
                    
                }
                //3 wrong tries and the system get locked.
                if (guarding == true && tries == 3)
                    locked(&arduino);
            }
            cout <<  endl;
            cout << "         *   *   *" << endl;
            cout << "Alarm is deactivated" << endl;
            userWelcomeMsg(activeUser); //gets users info from activeUser and prints out a message

            // A menu or choice for the user.
            int tries2 = 0;
            string input2;
            cout << endl;
            while (guarding == false)
            {
                send(&arduino, "off");
                cout << endl;
                cout << "To activate outside and inside alarms press A then your pin code then #" << endl;
                cout << "To activate only outside alarm press B then your pin code then #" << endl;
                while (guarding == false && tries2 < 3)
                {
                    input2 = getInput(&arduino);//imports a string incoming from arduino and saves it into input2.
                    if (input2.length() == 4) // if input contains a string of 4 chars
                    {
                        usersInput = stoi(input2); //string to int and saves it into usersInput.
                        input2 = "0";
                        //If input == any of pincodes saved in file and user isnt blocked => menu.
                        //if not then try again, max 3 tries.
                        logging_out(usersInput, &activeUser, guarding);
                        tries2++;
                        if (guarding == false && tries < 3)
                        {
                            systemLog(6, "Unknown");
                            cout << endl;
                            cout << "Wrong pincode! Try again!" << endl;
                            break;
                        }
                    }
                    Sleep(1000);
                }
                if (guarding == false && tries2 == 3)
                {
                    locked(&arduino);
                }
            }
        }
    }
    return 0;
}
