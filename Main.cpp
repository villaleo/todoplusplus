//
// Created by Leonardo Villalobos on 12/16/21.
//

#include <iostream>
#include <cstdlib>
#include <map>
#include <utility>
#include <fstream>
#include <vector>
#include <array>
#include "Event.hpp"
#include "Helpers.hpp"

int main () {
    const std::string PROJECT_PATH ("/Users/lvill/dev/Todoplusplus/");
    std::string selection, user_name, user_date, user_category;
    std::array<std::string, 3> user_details {selection, user_name, user_date};
    std::multimap<std::string, Event> list;

    external::displayMenu ();
    do {
        std::cout << "Enter selection.\n>> ";
        std::cin >> selection;
        std::cin.ignore ();
        selection = external::toLower (selection);
        external::trimRight (selection);

        if (selection == "ins") {
            list_ops::insertIntoList (user_details, list);
        }
        else if (selection == "rm") {
            list_ops::removeFromList (user_details, list);
        }
        else if (selection == "vw") {
            list_ops::displayList (list);
        }
        else if (selection == "sv") {
            list_ops::saveListToFile (list, PROJECT_PATH);
        }
        else if (selection == "op") {
            list_ops::loadListFromFile (list, PROJECT_PATH);
        }
        else if (selection == "q") {
            external::log ("Terminating...", 'w');
            continue;
        }
        else if (selection == "h") {
            external::displayHelp ();
        }
        else { // Unknown action
            external::log ("Error: Unknown command. Press \'h\' for help.", 'e');
        }
    } while (selection != "q");

    return EXIT_SUCCESS;
}
