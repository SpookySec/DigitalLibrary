#pragma once

#include <vector>
#include "LibraryBook.h"
#include "LibraryUser.h"

using namespace std;

class LibrarySystem
{
    vector<shared_ptr<LibraryBook>> books;
    vector<shared_ptr<LibraryUser>> users;

public:
    bool addUser(shared_ptr<LibraryUser> user);
    shared_ptr<LibraryBook> searchBook(string title);
    void displayAllBooks();
    void displayAllUsers();

    vector<shared_ptr<LibraryBook>>* getBooks();
    vector<shared_ptr<LibraryUser>> getUsers();
};

