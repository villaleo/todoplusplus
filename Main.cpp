#include <iostream>
#include <cstdlib>
#include <thread>
#include <map>
#include <utility>
#include "Event.hpp"

void displayMenu ();

bool taskCancelled (const std::string &str, const std::string &task);

std::string &toLower (std::string &str);

int main () {
    std::string selection, user_name, user_date, user_category;
    std::multimap<std::string, Event> list;

    do {
        displayMenu ();
        std::cin >> selection;
        selection = toLower (selection);

        if (selection == "ins") { // Insert event
            std::cout << "[$]   - Cancel\n>> Event name: ";
            std::cin.ignore ();
            std::getline (std::cin, user_name);
            if (taskCancelled (user_name, "Insert")) continue;

            std::cout << ">> Event date: ";
            std::getline (std::cin, user_date);
            if (taskCancelled (user_date, "Insert")) continue;

            std::cout << ">> Event category: ";
            std::getline (std::cin, user_category);
            if (taskCancelled (user_category, "Insert")) continue;

            auto event = Event (user_name, user_date);
            if (!event.null ()) {
                list.insert ({ user_category, event });
                std::cout << "\n* Event insertion successfully.\n";
            }
            else
                std::cout << "\n* Error: Invalid event details specified.\n";
        }
        else if (selection == "rm") { // Remove event
            bool removed = false;

            std::cout << "[$]   - Cancel\n>> Event name: ";
            std::cin.ignore ();
            std::getline (std::cin, user_name);
            if (taskCancelled (user_name, "Remove")) continue;

            std::cout << ">> Event date: ";
            std::getline (std::cin, user_date);
            if (taskCancelled (user_date, "Remove")) continue;

            std::cout << ">> Event category: ";
            std::getline (std::cin, user_category);
            if (taskCancelled (user_category, "Remove")) continue;

            auto event = Event (user_name, user_date);
            if (event.null ()) {
                std::cout << "\n* Error: Invalid event details specified.\n";
                continue;
            }

            auto[begin, end] = list.equal_range (user_category);
            for (auto i = begin; i != end; i++) {
                if (i->second == event) {
                    list.erase (i);
                    removed = true;
                    std::cout << "\n* Event removal successful.\n";
                    break;
                }
            }
            if (!removed) std::cout << "\n* Event not erased. No such event exists.\n";
        }
        else if (selection == "vw") {
            // View events
            std::cout << "[$]   - Cancel\n>> Event category: ";
            std::cin.ignore ();
            std::getline (std::cin, user_category);
            if (taskCancelled (user_category, "View")) continue;

            auto[begin, end] = list.equal_range (user_category);
            if (begin == list.end ()) {
                std::cout << "\n* Nothing to display.\n";
                continue;
            }

            std::cout << "———————————————————————————\n";
            for (auto i = begin; i != end; i++)
                std::cout << i->second
                    .getName () << " (" << i->second
                    .getDate () << ") : " << i->first << '\n';
        }
        else if (selection == "sv") {
            // Save events
            if (!list.empty ()) {
                //
            }
            else
                std::cout << "\n* Nothing to save.\n";
        }
        else if (selection == "q") {
            // Quit program
        }
        else if (selection == "h") {
            // Display help
        }
        else {
            // Unknown action
        }
    } while (selection != "q");

    return EXIT_SUCCESS;
}

void displayMenu () {
    std::cout << "———————————————————————————\n[ins] - Insert an event    \n";
    std::cout << "[rm]  - Remove an event    \n[vw]  - View events        \n";
    std::cout << "[sv]  - Save list to a file\n[q]   - Quit the program   \n";
    std::cout << "[h]   - Help               \n———————————————————————————\n";
    std::cout << "** Enter selection.\n>> ";
}

bool taskCancelled (const std::string &str, const std::string &task) {
    if (str.length () == 1 && str[0] == '$') {
        std::cout << "\n* " << task << " cancelled.\n";
        return true;
    }
    return false;
};

std::string &toLower (std::string &str) {
    for (char &c: str)
        c = static_cast<char>(std::tolower (c));
    return str;
}
