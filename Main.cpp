#include <iostream>
#include <cstdlib>
#include <string>
#include <array>
#include <thread>
#include <map>
#include <utility>

/*
 * [[name, date, category], [name, date, category], [name, date, category]]
 * multimap?
 * [category] -> [name, date]
 */

class InvalidEvent {
public:
    [[nodiscard]] virtual bool null () const = 0;

protected:
    bool isNull { false };
};

class Event : virtual public InvalidEvent {
public:
    Event () { isNull = true; }

    Event (std::string name, std::string date):
        name (std::move (name)), date (std::move (date)) {}

    [[nodiscard]] bool null () const override { return isNull; }

private:
    std::string name, date;
};

void displayMenu ();

int main () {
    std::string selection;

    do {
        displayMenu ();
        std::cin >> selection;
        // Handle selection

        if (selection == "ins") {
            // Insert event
        }
        else if (selection == "rm") {
            // Remove event
        }
        else if (selection == "vw") {
            // View events
        }
        else if (selection == "sv") {
            // Save events
        }
        else if (selection == "q") {
            // Quit program
            break;
        }
        else if (selection == "h") {
            // Display help
        }
        else {
            // Unknown action
        }
    } while (std::tolower (selection[0]) != 'q');

    return EXIT_SUCCESS;
}

void displayMenu () {
    std::cout << "———————————————————————————\n          Actions          \n";
    std::cout << "———————————————————————————\n[ins] - Insert an event    \n";
    std::cout << "[rm]  - Remove an event    \n[vw]  - View events        \n";
    std::cout << "[sv]  - Save list to a file\n[q]   - Quit the program   \n";
    std::cout << "[h]   - Help               \n———————————————————————————\n";
    std::cout << "Enter selection.\n";
}
