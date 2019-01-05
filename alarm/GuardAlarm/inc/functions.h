#ifndef FUNCTIONS_H
#define FUNCTIONS_H

using namespace std;

void guardingOff(class SerialPort *arduino, bool &guarding);
void guardingOn(class SerialPort *arduino, bool &guarding);
void userWelcomeMsg(struct User activeUser);
void message_log(int num, string *str);
void systemLog(int num, string id);
void locked(class SerialPort *arduino, char incomingData[]);
void logging_in(class SerialPort *arduino, int usersInput, struct User *active, bool &guarding);
void logging_out(class SerialPort *arduino, int usersInput, struct User *active, bool &guarding);



#endif
