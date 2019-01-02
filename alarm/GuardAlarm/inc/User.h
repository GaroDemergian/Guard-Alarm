
#ifndef USER_H
#define USER_H

using namespace std;

enum Status
{
    Active = 1,
    Inactive = 2,
    Blocked = 3
};

struct User
{
    int userID;
    int pincode;
    string userName;
    string tagID;
    Status status;
    string reserved;
};


#endif
