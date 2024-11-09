#include <iostream>
#include "../hdr/Librarian.h"
#include "../hdr/LibrarySystem.h"
#include "../hdr/color.hpp"

using namespace std;

// global::library defined in includes.h
namespace global {
    extern LibrarySystem library;
}

// Constructor initializing a Librarian with name and user ID
Librarian::Librarian(string name, int user_id) : LibraryUser(name, user_id) {}

// Displays information specific to the Librarian user type
void Librarian::displayUserInfo() const {
    cout << "[" << dye::aqua("*") << "] Displaying " << dye::yellow("Librarian") << " Info: " << "\n  - Username: " << dye::aqua(name) << "\n  - User ID : " << dye::aqua(user_id) << endl;
}

// Adds a new book to the library if it doesn’t already exist (checked by ISBN)
// Returns true if the book is added, false if a book with the same ISBN already exists
bool Librarian::addBook(shared_ptr<LibraryBook> book) {
    string ISBN = book->getISBN();
    for (auto book : *global::library.getBooks())
        if (book->getISBN() == ISBN) {
            return false;
        }

    global::library.getBooks()->push_back(book);
    return true;
}

// Removes a book from the library by ISBN if it exists
// Returns true if the book is successfully removed, false if the book is not found
bool Librarian::removeBook(string ISBN) {
    auto& books = *global::library.getBooks();

    for (auto it = books.begin(); it != books.end(); ++it) {
        if ((*it)->getISBN() == ISBN) {
            books.erase(it);
            return true;
        }
    }

    return false;
}
