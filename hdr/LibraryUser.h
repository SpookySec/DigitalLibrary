#pragma once

#include <iostream>
#include <string.h>

using namespace std;

class LibraryUser
{
protected:
    string name;
    int user_id;

public:
    LibraryUser(string name, int user_id) : name(name), user_id(user_id) {}
    virtual void displayUserInfo() const = 0;

    string getName();
    int getUserId();
};