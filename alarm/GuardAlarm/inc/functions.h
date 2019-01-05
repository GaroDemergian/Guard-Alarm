#ifndef FUNCTIONS_H
#define FUNCTIONS_H

using namespace std;

void guardingOff(bool &guarding);
void guardingOn(bool &guarding);
void userWelcomeMsg(struct User activeUser);
void message_log(int num, string *str);
void systemLog(int num, string id);
void locked(class SerialPort *arduino);
void logging_in(int usersInput, struct User *active, bool &guarding);
void logging_out(int usersInput, struct User *active, bool &guarding);



#endif
