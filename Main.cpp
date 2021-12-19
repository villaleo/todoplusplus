//
// Created by Leonardo Villalobos on 12/16/21.
//

#include <iostream>
#include <cstdlib>
#include <map>
#include <utility>
#include <fstream>
#include <vector>
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

            if (list.find (user_category) == list.end ()) {
                log ("Error: No matching category found.", 'e');
                continue;
            }

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
            // Format is cat=... => event=... (date=...)
            for (const auto&[category, event]: list) {
                outputFile << "cat=" << category << " => event=" << event.getName ();
                outputFile << " (date=" << event.getDate () << ")\n";
            }

            outputFile.close ();
            log ("File insertion successful.", 's');
        }
        else if (selection == "op") { // Open existing file
            std::cout << "[$]   - Cancel\n>> Event category: ";

            std::string filepath, filename;
            bool format_error { false };

            std::cin.ignore ();
            std::cout << "Enter file path. Leave blank if using project directory.\n>> ";
            std::getline (std::cin, filepath);

            if (taskCancelled (filepath, "Open file")) continue;

            // Handle path name
            if (filepath.empty ())
                filepath = PROJECT_PATH;
            else if (!validPathname (filepath)) {
                log ("Error: Filename contains illegal characters.", 'e');
                continue;
            }

            std::cout << "Enter file name. Leave blank if using default name \"todo.txt\".\n>> ";
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

            // Clear current list
            list.clear ();

            // Manage contents in file
            std::vector<std::string> entries;
            std::string temp;

            if (inputFile.fail ()) {
                log ("Error: Something went wrong.", 'e');
                continue;
            }

            while (std::getline (inputFile, temp))
                entries.push_back (temp);
            inputFile.close ();

            if (entries.empty ()) {
                log ("Error: Nothing to insert.", 'e');
                continue;
            }

            // Add file contents to current list
            for (const auto &entry: entries) {
                // Literals found in the file
                std::string event = "event=", date = "date=", category = "cat=";

                // Locate the literals
                size_t cat_start = entry.find (category);
                size_t event_start = entry.find (event);
                size_t date_start = entry.find (date);

                // If not found
                constexpr auto nil = std::string::npos;
                if (cat_start == nil || event_start == nil || date_start == nil) {
                    format_error = true;
                    break;
                }

                // Extract and store into user variables
                user_category = entry.substr (
                    category.length (),
                    event_start - category.length () - 4
                );
                user_name = entry.substr (
                    event_start + event.length (),
                    date_start - cat_start - event_start - date.length () - 3
                );
                user_date = entry.substr (
                    date_start + date.length (),
                    entry.substr (date_start + date.length ())
                        .length () - 1
                );

                // Insert into list
                list.insert (
                    {
                        user_category,
                        Event (user_name, user_date)
                    }
                );
            }
            if (format_error)
                log ("Error: File format is invalid.", 'e');
            else
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
