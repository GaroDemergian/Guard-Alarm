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
#include "Sensor.h"
#include "arduino_functions.h"
#include "SerialPort.h"

int main()
{
    struct User activeUser;

    const bool workingAlarm = true; //workingAlarm should never be false.
    bool guarding = true;
    int usersInput;

    //this loop will keep the alarm system program running.
    while (workingAlarm == true)
    {
        guardingOn(guarding);
        int tries = 0;
        //this loop is for deactivating and activating the alarm.
        //the alarm is active while guarding is true.
        while (guarding == true)
        {
            //this function is mainly for stopping the user from entering a wrong pincode more than 3 times.
            while (guarding == true && tries < 3)
            {
                cout << "Alarm is active" << endl;
                cout << endl;
                cout << "Enter pincode " << endl;
                usersInput = stoi(getInput());

                //If input == any of pincodes saved in file and user isnt blocked => menu.
                //if not then try again, max 3 tries.
                logging_in(usersInput, &activeUser, guarding);
                tries++;
                if (guarding == true && tries < 3)
                {
                    systemLog(4, "Unknown");
                    cout << "Wrong code, try again!" << endl;
                    break;
                }
            }
            //3 wrong tries and the system get locked.
            if (guarding == true && tries == 3)
            {
                locked();
            }
        }
        cout << "Alarm is deactivated" << endl;
        userWelcomeMsg(activeUser);

        // A menu or choice for the user.
        int x;
        int tries2 = 0;
        cout << endl;
        cout << "To activate alarm press 1 then * " << endl;
        cout << "for configurations press 2 then * " << endl;
        x = stoi(getInput());
        switch (x)
        {
        case 1:
            while (guarding == false)
            {
                while (guarding == false && tries < 3)
                {
                    cout << endl;
                    cout << "Enter pincode " << endl;
                    usersInput = stoi(getInput());

                    //pincode ok = menu. pincode not ok try again. 3 times
                    logging_out(usersInput, &activeUser, guarding);
                    tries2++;
                    if (guarding == false && tries < 3)
                    {
                        systemLog(6, "Unknown");
                        cout << "Wrong code, try again!" << endl;
                        break;
                    }
                }
                if (guarding == false && tries == 3)
                {
                    locked();
                }
            }
            break;

        case 2:
            cout << endl;
            cout << "this function is unavailable for the moment" << endl;
            break;

        default:
            cout << "Not a valid choice" << endl;
            break;
        }
    }
    return 0;
}
