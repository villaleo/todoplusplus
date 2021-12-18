//
// Created by Leonardo Villalobos on 12/16/21.
//

#include <iostream>
#include <cstdlib>
#include <map>
#include <utility>
#include <fstream>
#include "Event.hpp"
#include "Helpers.hpp"

int main () {
    const std::string PROJECT_PATH ("/Users/lvill/dev/Todoplusplus/");
    std::string selection, user_name, user_date, user_category;
    std::multimap<std::string, Event> list;

    displayMenu ();
    do {
        std::cout << "** Enter selection.\n>> ";
        std::cin >> selection;
        selection = toLower (selection);
        trimRight (selection);

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
                log ("Event insertion successfully.", 's');
            }
            else
                log ("Error: Invalid event details specified.", 'e');
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
                log ("Error: Invalid event details specified.", 'e');
                continue;
            }

            auto[begin, end] = list.equal_range (user_category);
            for (auto i = begin; i != end; i++) {
                if (i->second == event) {
                    list.erase (i);
                    removed = true;
                    log ("Event removal successful.", 's');
                    break;
                }
            }
            if (!removed) log ("Error: No such event exists.", 'e');
        }
        else if (selection == "vw") { // View events
            std::cout << "[$]   - Cancel\n>> Event category: ";
            std::cin.ignore ();
            std::getline (std::cin, user_category);
            if (taskCancelled (user_category, "View")) continue;

            auto[begin, end] = list.equal_range (user_category);
            if (begin == list.end ()) {
                log ("Error: List is empty, nothing to display.", 'e');
                continue;
            }

            std::cout << "———————————————————————————\n";
            std::string buffer;
            for (auto i = begin; i != end; i++) {
                std::cout << i->second
                                 .getName () + " (";
                std::cout << i->second
                                 .getDate () + ")\n";
            }
            std::cout << "———————————————————————————\n";
        }
        else if (selection == "sv") { // Save events
            if (list.empty ()) {
                log ("Error: List is empty, nothing to save.", 'e');
                continue;
            }
            std::string pathname, filename;
            std::cin.ignore ();
            std::cout << "Enter the pathname to where to save the file to,";
            std::cout << " or leave blank to save to project directory.\n>> ";
            std::getline (std::cin, pathname);

            if (pathname.empty ())
                pathname = PROJECT_PATH;
            else if (!validPathname (pathname)) {
                log ("Error: Filename contains illegal characters.", 'e');
                continue;
            }

            std::cout << "Enter name for the file, or leave blank to save as \"todo.txt\".\n>> ";
            std::getline (std::cin, filename);

            if (filename.empty ())
                filename = "todo.txt";
            else if (!validFilename (filename)) {
                log ("Error: Filename contains illegal characters.", 'e');
                continue;
            }

            // Make sure file name and pathname are in the correct format
            formatDirectory (filename, pathname);

            // Create output file
            std::ofstream outputFile (pathname + filename);
            if (outputFile.fail ()) {
                log ("Error: Something went wrong.", 'e');
                continue;
            }

            // Write the events from the list
            for (const auto&[category, event]: list)
                outputFile << event.getName () << " (" << event.getDate () << ") : " << category << '\n';

            outputFile.close ();
            log ("File insertion successful.", 's');
        }
        else if (selection == "op") { // Open existing file
            std::string filepath, filename;
            std::cin.ignore ();
            std::cout << "Enter the path of the file. If using project directory, leave blank.\n";
            std::getline (std::cin, filepath);

            // Handle path name
            if (filepath.empty ())
                filepath = PROJECT_PATH;
            else if (!validPathname (filepath)) {
                log ("Error: Filename contains illegal characters.", 'e');
                continue;
            }

            std::cout << "Enter the name of the file. If using default name \"todo.txt\", leave blank.\n";
            std::getline (std::cin, filename);

            // Handle file name
            if (filename.empty ())
                filename = "todo.txt";
            else if (!validFilename (filename)) {
                log ("Error: Filename contains illegal characters.", 'e');
                continue;
            }

            // Make sure file name and path are in the correct format
            formatDirectory (filename, filepath);

            // Read from file
            std::ifstream inputFile (filepath + filename);

            // TODO: Clear current list
            // TODO: Manage contents in file
            // TODO: Add file contents to current list

            inputFile.close ();
            log ("Contents loaded from file successfully!", 's');
        }
        else if (selection == "q") { // Quit program
            log ("Terminating...", 'w');
            continue;
        }
        else if (selection == "h") { // Display help
            displayHelp ();
        }
        else { // Unknown action
            log ("Error: Unknown command. Press \'h\' for help.", 'e');
        }
    } while (selection != "q");

    return EXIT_SUCCESS;
}
