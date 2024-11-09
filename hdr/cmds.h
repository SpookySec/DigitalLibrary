#pragma once
#include <iostream>
#include "color.hpp"

#define LOG(out, msg) (out << "[" << dye::aqua("*") << "] " << msg)
#define ERR(out, msg) (out << "[" << dye::red("!") << "] " << msg)
#define SCS(out, msg) (out << "[" << dye::green("+") << "] " << msg)
#define CMT(out, msg) (out << dye::grey_on_black("// ") << dye::grey_on_black(msg))

using namespace std;

void login(ostream& out, string user);
void addUser(ostream& out, string user_name, string type);
void displayUsers(ostream& out);
void displayBooks(ostream& out);
void addBook(ostream& out, string title, string author, string ISBN);
void removeBook(ostream& out, const string& ISBN);
void countBooks(ostream& out);
void searchBook(ostream& out, string title);
void borrowBook(ostream& out, string ISBN);
void returnBook(ostream& out, string ISBN);
void listBorrowed(ostream& out);
void showCurrentUser(ostream& out);
void logout(ostream& out);