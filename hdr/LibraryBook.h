#pragma once
#include <iostream>
#include <string.h>

using namespace std;

class LibraryBook
{
    string title;
    string author;
    string ISBN;
    bool available;
    static int total_books;

public:
    // Getter methods
    string getTitle();
    string getAuthor();
    string getISBN();
    bool getAvailable();

    LibraryBook(string title, string author, string ISBN, bool available);
    ~LibraryBook();
    void displayInfo();
    void setAvailability(bool);
    bool isAvailable();
    static int getTotalBooks();
};