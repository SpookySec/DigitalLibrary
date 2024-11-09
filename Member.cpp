#include "Member.h"
#include <iostream>
#include "color.hpp"

using namespace std;

// Constructor initializes a Member with a name and user ID
Member::Member(string name, int user_id) : LibraryUser(name, user_id) {}

// Displays information about the Member, including their name and user ID
void Member::displayUserInfo() const {
    cout << "[" << dye::aqua("*") << "] Displaying " << dye::yellow("Member") << " Info: "
        << "\n  - Username: " << dye::aqua(name)
        << "\n  - User ID : " << dye::aqua(user_id) << endl;
}

// Attempts to borrow a book for the Member
// Adds the book to the Member's borrowed_books list if it is available
bool Member::borrowBook(shared_ptr<LibraryBook> book) {
    if (book->isAvailable()) {
        borrowed_books.push_back(book);  // Add book to the borrowed list
        book->setAvailability(false);    // Mark the book as unavailable
        return true;
    }

    return false;  // Return false if the book is not available
}

// Attempts to return a book for the Member
// Removes the book from borrowed_books if found
bool Member::returnBook(shared_ptr<LibraryBook> book) {
    // Locate the book in the borrowed_books list
    auto index = find(borrowed_books.begin(), borrowed_books.end(), book);

    if (index != borrowed_books.end()) {
        borrowed_books.erase(index);  // Remove the book from the list
        return true;
    }

    return false;  // Return false if the book was not found in the borrowed list
}

// Retrieves the list of books currently borrowed by the Member
vector<shared_ptr<LibraryBook>> Member::getBorrowedBooks() {
    return borrowed_books;
}
