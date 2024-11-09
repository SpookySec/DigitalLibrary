#pragma once

#include "LibraryUser.h"
#include "LibraryBook.h"
#include <vector>

using namespace std;

class Member : public LibraryUser
{
    vector<shared_ptr<LibraryBook>> borrowed_books;

public:
    // Abstract methods
    void displayUserInfo() const override;

    Member(string name, int user_id);
    bool borrowBook(shared_ptr<LibraryBook> book);
    bool returnBook(shared_ptr<LibraryBook> book);
    vector<shared_ptr<LibraryBook>> getBorrowedBooks();
};