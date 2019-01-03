#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <time.h>

#include "functions.h"
#include "User.h"
#include "arduino_functions.h"

using namespace std;

//function for diactivating the alarm system.
void guardingOff(bool &guarding)
{
    guarding = false;
    send("off");
}

//function for activating the alarm system.
void guardingOn(bool &guarding)
{
    guarding = true;
    checkSensor();
    send("on");
}

//this function accepts user input and checks if it matches with any saved pincode from users.dat
//allows to user to deactivate the alarm system
void logging_in(int usersInput, struct User *active, bool &guarding)
{
    int tries = 2;
    ifstream userInfo;
    string id, pin, name, tag, status, reserved;

    userInfo.open("../GuardAlarm/bin/users.dat");

    if (userInfo.is_open() && tries >= 0)
    {
        while (!userInfo.eof())
        {
            //separates values and stores them in variables.
            getline(userInfo, id, ';');
            getline(userInfo, pin, ';');
            getline(userInfo, name, ';');
            getline(userInfo, tag, ';');
            getline(userInfo, status, ';');
            getline(userInfo, reserved, '\n');

            //checks for matching pincode and user status. or..
            //checks for matching pincode+ 1 and user status
            if (usersInput == stoi(pin) && status != "3"||
                usersInput == stoi(pin) + 1 && status != "3")
            {
                userInfo.close();

                //saves active users info to User
                (*active).userName = name;
                (*active).userID = stoi(id);
                (*active).pincode = stoi(pin);
                (*active).status = Active;
                guardingOff(guarding);
                if (usersInput == stoi(pin))
                {
                    systemLog(1, id);
                }
                else if (usersInput == (stoi(pin) + 1))
                {
                    systemLog(7, id);
                    cout << "Calling security" << endl;
                }
                break;
            }

            //doesnt allow blocked users to enter the system.
            else if (usersInput == stoi(pin) && status == "3" ||
                     usersInput == stoi(pin) + 1 && status == "3")
            {
                cout << "User is blocked" << endl;
                systemLog(9, id);
            }
        }
    }
    userInfo.close();
}

//function for welcoming the inlogged user.
void userWelcomeMsg(struct User activeUser)
{
    cout << "Welcome " << activeUser.userName << endl;
}

//different messages to be logged into system.log
void message_log(int num, string *str)
{
    switch (num)
    {
    case 1:
        *str = "Alarm deactivated";
        break;
    case 2:
        *str = "Alarm activated";
        break;
    case 3:
        *str = "Attempt to deactivate alarm";
        break;
    case 4:
        *str = "Failed to deactivate alarm";
        break;
    case 5:
        *str = "Attemt to activate alarm";
        break;
    case 6:
        *str = "Failed to activate alarm";
        break;
    case 7:
        *str = "Alarm deactivated/police notified";
        break;
    case 8:
        *str = "3 failed attempts/System locked!";
        break;
    case 9:
        *str = "Blocked user attempted alarm deactivation";
        break;
    case 10:
        *str = "Alarm activated/Police notified";
        break;
    case 11:
        *str = "Blocked user attempted alarm activation";
        break;
    }
}

//function for logging activities into a text file named system.log
void systemLog(int num, string id)
{
    ofstream outFile;
    ifstream system_log;
    string reserved;
    char c;
    int rows = 0;

    //the next 6 lines with get the date from system to program
    time_t now = time(0);
    struct tm tstruct;
    char date[80];
    tstruct = *localtime(&now);
    strftime(date, sizeof(date), "%Y%m%d %X", &tstruct);
    string date_now = date;
    string message;

    message_log(num, &message);

    system_log.open("../GuardAlarm/bin/system.log");

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

    outFile.open("../GuardAlarm/bin/system.log", ofstream::app);
    if (num == 12 || num == 13)
    outFile << rows + 1 << ";" << date << ";" << message << '\n';
    else
    outFile << rows + 1 << ";" << date << ";" << id << ";" << reserved << ";" << message << '\n';
    outFile.close();
}

//function for locking the system after 3 wrong pincode inputs.
void locked()
{
    send("siren");
    cout << "****ALARM****" << endl;
    cout << "Calling Security" << endl;
    systemLog(8, "Unknown");
    for (int i = 0; i < 1;)
    {
        // The system stays in the loop.
        checkSensor();
    } 
}

//this function accepts user input and checks if it matches with any saved pincode from users.dat
//allows to user to activate the alarm system
void logging_out(int usersInput, struct User *active, bool &guarding)
{
    int tries = 2;
    ifstream userInfo;
    string id, pin, name, tag, status, reserved;

    userInfo.open("../GuardAlarm/bin/users.dat");

    if (userInfo.is_open() && tries >= 0)
    {
        while (!userInfo.eof())
        {
            //separates values and stores them in variables.
            getline(userInfo, id, ';');
            getline(userInfo, pin, ';');
            getline(userInfo, name, ';');
            getline(userInfo, tag, ';');
            getline(userInfo, status, ';');
            getline(userInfo, reserved, '\n');

            //checks for matching pincode.
            if (usersInput == stoi(pin) && (status != "3"))
            {
                userInfo.close();
                guardingOn(guarding);
                systemLog(2, id);
                break;
            }
            //checks for matching pincode+ 1 and user status
            else if (usersInput == (stoi(pin) + 1) && (status != "3"))
            {
                userInfo.close();
                guardingOn(guarding);
                systemLog(10, id);
                cout << "Calling security" << endl;
                break;
            }
            //doesnt allow blocked users to enter the system.
            else if (usersInput == stoi(pin) ||
                     usersInput == stoi(pin) + 1 &&
                         status == "3")
            {
                cout << "User is blocked" << endl;
                systemLog(11, id);
            }
        }
    }
    userInfo.close();
}

