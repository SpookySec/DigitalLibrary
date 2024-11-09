#include "includes.h"
#include "cli-2.2.0/include/cli/cli.h"
#include "cli-2.2.0/include/cli/loopscheduler.h"
#include "cli-2.2.0/include/cli/clilocalsession.h"
#include "cli-2.2.0/include/cli/scheduler.h"
#include "color.hpp"
#include "cmds.h"

using namespace cli;

// Global variable representing the current logged-in user
extern shared_ptr<LibraryUser> current;



int main() {
    // Display initial messages to welcome the user and provide guidance
    CMT(cout, "Welcome to the Digital Library System!\n");
    CMT(cout, "Use 'help' to display available commands.\n");
    CMT(cout, "Press TAB to auto-complete and CTRL+L to clear the screen.\n");

    // Set up the CLI root menu
    auto root_menu = make_unique<Menu>("library");
    SetColor(); // Enable terminal color output

    // Adding commands to the CLI
    root_menu->Insert("login", login,
        "Usage: login <role>\n"
        "\tDescription: Login as either a 'librarian' or a 'member'.\n"
        "\tExample: login librarian or login member\n");

    root_menu->Insert("adduser", addUser,
        "Usage: adduser <role> <name>\n"
        "\tDescription: Add a new 'librarian' or a 'member' user.\n"
        "\tExample: adduser John librarian or adduser Alice member\n");

    root_menu->Insert("displayusers", displayUsers,
        "Usage: displayusers\n"
        "\tDescription: Display all registered users.\n"
        "\tExample: displayusers\n");

    root_menu->Insert("displaybooks", displayBooks,
        "Usage: displaybooks\n"
        "\tDescription: Display all books in the library.\n"
        "\tExample: displaybooks\n");

    root_menu->Insert("addbook", addBook,
        "Usage: addbook <title> <author> <isbn>\n"
        "\tDescription: Add a new book to the library.\n"
        "\tExample: addbook 'C++ Programming' 'Bjarne Stroustrup' '978-0321563842'\n");

    root_menu->Insert("removebook", removeBook,
        "Usage: removebook <isbn>\n"
        "\tDescription: Remove a book from the library by its ISBN.\n"
        "\tExample: removebook '978-0321563842'\n");

    root_menu->Insert("bookcount", countBooks,
        "Usage: bookcount\n"
        "\tDescription: Display the total number of books in the library.\n"
        "\tExample: bookcount\n");

    root_menu->Insert("search", searchBook,
        "Usage: search <title>\n"
        "\tDescription: Search for a book by its title.\n"
        "\tExample: search 'C++ Programming'\n");

    root_menu->Insert("borrow", borrowBook,
        "Usage: borrow <isbn>\n"
        "\tDescription: Borrow a book from the library using its ISBN.\n"
        "\tExample: borrow '978-0321563842'\n");

    root_menu->Insert("return", returnBook,
        "Usage: return <isbn>\n"
        "\tDescription: Return a borrowed book by its ISBN.\n"
        "\tExample: return '978-0321563842'\n");

    root_menu->Insert("listborrowed", listBorrowed,
        "Usage: listborrowed\n"
        "\tDescription: List books currently borrowed by the logged-in member.\n"
        "\tExample: listborrowed\n");

    root_menu->Insert("current", showCurrentUser,
        "Usage: current\n"
        "\tDescription: Display the currently logged-in user.\n"
        "\tExample: current\n");

    root_menu->Insert("logout", logout,
        "Usage: logout\n"
        "\tDescription: Logout the current user.\n"
        "\tExample: logout\n");



    // Initialize the CLI with the root menu
    Cli cli(move(root_menu));
    LoopScheduler scheduler;
    CliLocalTerminalSession local_session(cli, scheduler, cout);

    // Define an action for when the user exits the CLI session
    local_session.ExitAction(
        [&scheduler](auto& out) {
            LOG(out, dye::green("Thank you for using the Digital Library System. Goodbye!\n"));
            scheduler.Stop();
        }
    );

    // Set a handler for unrecognized or incorrect commands
    cli.WrongCommandHandler(
        [](ostream& out, const string& cmd) {
            ERR(out, "Unknown command or incorrect parameters: " << dye::purple(cmd) << "\n");
            LOG(out, "Use 'help' to see a list of available commands.\n");
        }
    );

    // Start the CLI session loop
    scheduler.Run();

    return 0;
}
