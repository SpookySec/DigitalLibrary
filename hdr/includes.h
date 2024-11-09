#pragma once
#include "LibrarySystem.h"
#include "LibraryBook.h"
#include "LibraryUser.h"
#include "Librarian.h"
#include "Member.h"

// Shared global LibrarySystem() object to manage the library
namespace global {
    LibrarySystem library{};
}