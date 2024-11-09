#pragma once

#include <string.h>
#include "LibraryUser.h"
#include "LibraryBook.h"

using namespace std;

class Librarian : public LibraryUser
{
public:
    // Abstract methods
    void displayUserInfo() const override;

    Librarian(string name, int user_id);
    bool addBook(shared_ptr<LibraryBook> book);
    bool removeBook(string ISBN);
};