//
// Created by Leonardo Villalobos on 12/16/21.
//

#include <iostream>
#include <cstdlib>
#include <utility>
#include <array>
#include <sstream>
#include "Helpers.hpp"

// The data structure to hold the events.
multimap<string, Event> list;

int main () {
    const string PROJECT_PATH ("YOUR PROJECT PATH");  // NOTE: Make sure to change the project path to the correct one
    string query, command, flag_1, flag_2;
    string user_name, user_date, user_category;
    array<string, USER_DETAILS_SIZE> user_details { query, user_name, user_date };

    external::displayMenu ();
    do {
        cout << ">> ";
        getline (cin, query);

        query = external::trimLeft (query);
        size_t space_index = query.find_first_of (' ');

        if (space_index == nil) {
            command = query;
        }
        else {
            command = query.substr (0, space_index);
            query = query.substr (space_index + 1);
        }

        external::toLower (command);

        if (command == "add") {
            operations::insertIntoList ();
        }
        else if (command == "del") {
            stringstream ss (query);
            ss >> flag_1;

            operations::removeFromList (flag_1);
        }
        else if (command == "view") {
            stringstream ss (query);
            ss >> flag_1;

            operations::displayList (flag_1);
        }
        else if (command == "save") {
            stringstream ss (query);
            ss >> flag_1 >> flag_2;

            try {
                operations::saveToFile (flag_1, flag_2);
            }
            catch (out_of_range &error) {
                external::log ("Error: File name and extension cannot be empty.", 'e');
            }
        }
        else if (command == "open") {
            stringstream ss (query);
            ss >> flag_1 >> flag_2;

            operations::loadFromPath (flag_1, flag_2);
        }
        else if (command == "quit") {
            external::log ("Terminating...", 'w');
            continue;
        }
        else if (command == "help") {
            external::displayHelp ();
        }
        else
            external::log ("Error: Unknown command. Enter <help> for help.", 'e');
    } while (command != "quit");

    return EXIT_SUCCESS;
}
