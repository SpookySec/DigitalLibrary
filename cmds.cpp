#include <random>
#include <memory>
#include "cmds.h"
#include "Member.h"
#include "Librarian.h"
#include "LibraryUser.h"
#include "LibrarySystem.h"

shared_ptr<LibraryUser> current;

namespace global {
    extern LibrarySystem library;
}

// Generates a unique ID of size 8 characters
int generateUniqueID() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(10000000, 99999999);
    return dist(gen);
}

// Displays all users currently in the library system
void displayUsers(ostream& out) {
    global::library.displayAllUsers();
}

// Logs in the user by setting the `current` global variable based on user name
void login(ostream& out, string user_name) {
    auto users = global::library.getUsers();
    for (auto user : users) {
        if (user->getName() == user_name) {
            current = user;
            SCS(out, "Logged in as: " << dye::aqua(current->getName()) << "[" << dye::aqua(current->getUserId()) << "]!") << "\n";
            return;
        }
    }
    ERR(out, "User: " << dye::aqua(user_name) << " not found!\n");
}

// Creates a new user with a unique ID and specified type ("member" or "librarian")
void addUser(ostream& out, string user_name, string type) {
    int id = generateUniqueID();

    if (type == "member") {
        shared_ptr<Member> user = make_shared<Member>(user_name, id);
        if (global::library.addUser(user))
            LOG(out, "User: " << dye::aqua(user_name) << " created with ID: " << dye::aqua(id) << "\n");
        else
            ERR(out, "User: " << dye::aqua(user_name) << " already exists!\n");
    }
    else if (type == "librarian") {
        shared_ptr<Librarian> user = make_shared<Librarian>(user_name, id);
        if (global::library.addUser(user))
            LOG(out, "User: " << dye::aqua(user_name) << " created with ID: " << dye::aqua(id) << "\n");
        else
            ERR(out, "User: " << dye::aqua(user_name) << " already exists!\n");
    }
    else {
        ERR(out, "You can only add a '" << dye::yellow("member") << "' or a '" << dye::yellow("librarian") << "'!\n");
        return;
    }
}

// Displays all books currently in the library system
void displayBooks(ostream& out) {
    global::library.displayAllBooks();
}

// Adds a new book if the current user is a librarian
void addBook(ostream& out, string title, string author, string ISBN) {
    if (!current) {
        ERR(out, "No user is logged in!\n");
        return;
    }

    Librarian* librarian = dynamic_cast<Librarian*>(current.get());
    if (!librarian) {
        ERR(out, "Only librarians are allowed to add books!\n");
        return;
    }

    shared_ptr<LibraryBook> book = make_shared<LibraryBook>(title, author, ISBN, true);

    if (librarian->addBook(book)) {
        SCS(out, "Book added successfully!\n");
    }
    else {
        ERR(out, "Book already exists!\n");
    }
}

// Removes a book by ISBN if the current user is a librarian
void removeBook(ostream& out, const string& ISBN) {
    if (!current) {
        ERR(out, "No user is logged in!\n");
        return;
    }

    Librarian* librarian = dynamic_cast<Librarian*>(current.get());
    if (!librarian) {
        ERR(out, "Only librarians are allowed to remove books!\n");
        return;
    }

    if (librarian->removeBook(ISBN)) {
        SCS(out, "Book with ISBN " << dye::yellow(ISBN) << " removed successfully!\n");
    }
    else {
        ERR(out, "Book with ISBN " << dye::yellow(ISBN) << " not found in the library!\n");
    }
}

// Counts the total number of books in the library by checking the first book's total
void countBooks(ostream& out) {
    auto books = global::library.getBooks();
    if (books->size() > 0)
        LOG(out, "There are " << dye::aqua(books->at(0)->getTotalBooks()) << " books\n");
    else
        ERR(out, "There are no books in the library\n");
}

// Searches for a book by title
void searchBook(ostream& out, string title) {
    auto book = global::library.searchBook(title);

    if (book) {
        SCS(out, "Found book with matching title!\n");
        book->displayInfo();
    }
    else
        LOG(out, "No books found with provided title\n");
}

// Allows a member to borrow a book if it is available
void borrowBook(ostream& out, string ISBN) {
    Member* member = dynamic_cast<Member*>(current.get());
    if (!member) {
        ERR(out, "Only members are allowed to borrow books!\n");
        return;
    }

    auto& books = *global::library.getBooks();
    for (const auto& book : books) {
        if (book->getISBN() == ISBN) {
            if (!book->isAvailable()) {
                ERR(out, "Book with ISBN " << dye::yellow(ISBN) << " is currently not available for borrowing!\n");
                return;
            }

            if (member->borrowBook(book)) {
                book->setAvailability(false);
                SCS(out, "Book with ISBN " << dye::yellow(ISBN) << " borrowed successfully!\n");
                return;
            }
            else {
                ERR(out, "Failed to borrow book with ISBN " << dye::yellow(ISBN) << "!\n");
                return;
            }
        }
    }
    ERR(out, "Book with ISBN " << dye::yellow(ISBN) << " not found in the library!\n");
}

// Allows a member to return a borrowed book
void returnBook(ostream& out, const string ISBN) {
    Member* member = dynamic_cast<Member*>(current.get());
    if (!member) {
        ERR(out, "Only members are allowed to return books!\n");
        return;
    }

    const auto& borrowedBooks = member->getBorrowedBooks();
    for (const auto& borrowedBook : borrowedBooks) {
        if (borrowedBook->getISBN() == ISBN) {
            borrowedBook->setAvailability(true);
            member->returnBook(borrowedBook);

            SCS(out, "Book with ISBN " << dye::yellow(ISBN) << " returned successfully!\n");
            return;
        }
    }
    ERR(out, "Book with ISBN " << dye::yellow(ISBN) << " is not currently borrowed by you!\n");
}

// Lists all books borrowed by the current user if they are a member
void listBorrowed(ostream& out) {
    Member* member = dynamic_cast<Member*>(current.get());
    if (!member) {
        ERR(out, "Only members are allowed to borrow books!\n");
        return;
    }

    LOG(out, "Displaying borrowed books\n");
    for (auto book : member->getBorrowedBooks()) {
        book->displayInfo();
        out << endl;
    }
}

// Displays the currently logged-in user's information, or a message if no user is logged in
void showCurrentUser(ostream& out) {
    if (!current) {
        LOG(out, "No user is currently logged in.\n");
    }
    else {
        string userType = dynamic_cast<Member*>(current.get()) ? "Member" : "Librarian";
        LOG(out, "Logged in as " << dye::yellow(userType) << ": " << dye::aqua(current->getName()) << "\n");
    }
}

// Logout function that resets the current user and displays a message
void logout(ostream& out) {
    if (current) {
        LOG(out, "User " << dye::aqua(current->getName()) << " has logged out.\n");
        current.reset();
    }
    else {
        ERR(out, "No user is currently logged in!\n");
    }
}