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
    const std::string YOUR_PROJECT_PATH ("/Users/lvill/dev/Todoplusplus/");
    std::string selection, user_name, user_date, user_category;
    std::multimap<std::string, Event> list;

    displayMenu ();
    do {
        std::cout << "** Enter selection.\n>> ";
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
        else if (selection == "vw") { // View events
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
        else if (selection == "sv") { // Save events
            if (list.empty ()) {
                std::cout << "\n* Nothing to save.\n";
                continue;
            }
            std::string path, filename;
            std::cin.ignore ();
            std::cout << "Enter the path to where to save the file to,";
            std::cout << " or leave blank to save to project directory.\n>> ";
            std::getline (std::cin, path);

            if (path.empty ())
                path = YOUR_PROJECT_PATH;
            else if (!validPathname (path)) {
                std::cout << "\n*Error: Filename contains illegal characters.\n";
                continue;
            }

            std::cout << "Enter name for the file, or leave blank to save as \"todo.txt\".\n>> ";
            std::getline (std::cin, filename);

            if (filename.empty ())
                filename = "todo.txt";
            else if (!validFilename (filename)) {
                std::cout << "\n*Error: Filename contains illegal characters.\n";
                continue;
            }

            // Check if path ends correctly
            if (path.back () != '/' || path.back () != '\\') {
                if (std::find (path.begin (), path.end (), '/') != path.end ())
                    path += '/';
                else
                    path += '\\';
            }

            // Check if file extension is correct
            auto extension = std::find (filename.begin (), filename.end (), '.');
            if (extension == filename.end ())
                filename += ".txt";
            else {
                // Change extension
                std::string newFilename;
                for (auto i = filename.begin (); i != extension; i++)
                    newFilename += *i;
                filename = newFilename + ".txt";
            }

            // Create output file
            std::ofstream outputFile (path + filename);
            if (outputFile.fail ()) {
                std::cout << "\n*Error: Something went wrong.\n";
                continue;
            }

            // Write the events from the list
            for (const auto&[category, event]: list) {
                outputFile << event.getName () << " (" << event.getDate () << ") : " << category << '\n';
            }
            outputFile.close ();
        }
        else if (selection == "q") { // Quit program
            continue;
        }
        else if (selection == "h") { // Display help
            displayHelp ();
        }
        else { // Unknown action
            std::cout << "Error: Unknown command. Press \'h\' for help.\n";
        }
    } while (selection != "q");

    return EXIT_SUCCESS;
}
