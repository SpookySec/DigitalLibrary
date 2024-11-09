#include "LibraryBook.h"
#include "color.hpp"

// Static variable to keep track of the total number of books
int LibraryBook::total_books = 0;

// Constructor to initialize a LibraryBook with title, author, ISBN, and availability status
// Increments the static `total_books` counter each time a new book is created
LibraryBook::LibraryBook(string title, string author, string ISBN, bool available) {
    this->title = title;
    this->author = author;
    this->ISBN = ISBN;
    this->available = available;
    total_books++;
}

// Destructor for the LibraryBook that decrements the total_books static variable by 1
LibraryBook::~LibraryBook() {
    total_books--;
}

// Displays information about the book, including title, author, ISBN, and availability status
void LibraryBook::displayInfo() {
    cout << "[" << dye::aqua("*") << "] Displaying " << dye::yellow("Book") << " Info: \n"
        << "  - Title:        " << dye::aqua(title) << "\n"
        << "  - Author:       " << dye::aqua(author) << "\n"
        << "  - ISBN:         " << dye::yellow(ISBN) << "\n"
        << "  - Availability: " << (available ? dye::green("Available!") : dye::red_on_yellow("Not Available."))
        << endl;
}

// Sets the availability status of the book (true if available, false if not)
void LibraryBook::setAvailability(bool newValue) {
    this->available = newValue;
}

// Returns the current availability status of the book
bool LibraryBook::isAvailable() {
    return this->available;
}

// Static method to get the total number of books in the library
int LibraryBook::getTotalBooks() {
    return total_books;
}

// Returns the title of the book
string LibraryBook::getTitle() {
    return title;
}

// Returns the author of the book
string LibraryBook::getAuthor() {
    return author;
}

// Returns the ISBN of the book
string LibraryBook::getISBN() {
    return ISBN;
}

// Alternative getter for the availability status of the book
bool LibraryBook::getAvailable() {
    return available;
}
