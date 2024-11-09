#include <vector>
#include <algorithm>
#include <cctype>
#include "../hdr/LibrarySystem.h"

using namespace std;

// Helper function to compare two strings in a case-insensitive manner
bool compareCaseInsensitive(const std::string& str1, const std::string& str2) {
    std::string lowerStr1 = str1;
    std::string lowerStr2 = str2;

    // Convert both strings to lowercase
    std::transform(lowerStr1.begin(), lowerStr1.end(), lowerStr1.begin(), ::tolower);
    std::transform(lowerStr2.begin(), lowerStr2.end(), lowerStr2.begin(), ::tolower);

    return lowerStr1 == lowerStr2;
}

// Adds a new user to the library if they do not already exist
// Returns true if the user is added, false if the user already exists
bool LibrarySystem::addUser(shared_ptr<LibraryUser> user) {
    // Check if user already exists
    if (find(users.begin(), users.end(), user) != users.end())
        return false; // User exists

    users.push_back(user);
    return true;
}

// Recursive function to search for a book by title within the collection
// Returns a shared pointer to the book if found, or NULL if not found
shared_ptr<LibraryBook> searchBookRecursive(const vector<shared_ptr<LibraryBook>>& books, string title, int index = 0) {
    // Base case: If the index is out of bounds (we've checked all books)
    if (index >= books.size()) {
        return NULL;  // Book not found
    }

    // Compare the current book's title with the given title
    if (compareCaseInsensitive(books[index]->getTitle(), title)) {
        return books[index];  // Return the found book
    }

    // Recursive case: Check the next book in the collection
    return searchBookRecursive(books, title, index + 1);
}

// Public method to search for a book by title, using the recursive helper
shared_ptr<LibraryBook> LibrarySystem::searchBook(string title) {
    return searchBookRecursive(books, title);
}

// Displays information for all books in the library
void LibrarySystem::displayAllBooks() {
    for (auto book : books) {
        book->displayInfo();
        cout << endl;
    }
}

// Displays information for all users in the library
void LibrarySystem::displayAllUsers() {
    for (auto user : users) {
        user->displayUserInfo();
        cout << endl;
    }
}

// Returns a pointer to the vector of books
vector<shared_ptr<LibraryBook>>* LibrarySystem::getBooks() {
    return &books;
}

// Returns a copy of the vector of users
vector<shared_ptr<LibraryUser>> LibrarySystem::getUsers() {
    return users;
}
