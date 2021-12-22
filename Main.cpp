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
        std::cout << "Enter selection.\n>> ";
        std::cin >> selection;
        std::cin.ignore ();
        selection = toLower (selection);
        trimRight (selection);

        if (selection == "ins") { // insert
            std::cout << "[$]   - Cancel\n>> Event name: ";
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
            std::string input_buffer;

            std::cout << "———————————————————————————\n";
            std::cout << "[a]  - Remove by specific details\n";
            std::cout << "[n]  - Remove by name\n";
            std::cout << "[k]  - Clear list\n";
            std::cout << "[c]  - Cancel\n";
            std::cout << "———————————————————————————\n>> ";

            std::getline (std::cin, input_buffer);

            if (taskCancelled (input_buffer, "Remove")) continue;

            if (input_buffer == "a") { // Remove by specific details
                std::cout << ">> Event name: ";
                std::getline (std::cin, user_name);
                std::cout << ">> Event date: ";
                std::getline (std::cin, user_date);
                std::cout << ">> Event category: ";
                std::getline (std::cin, user_category);

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
            }
            else if (input_buffer == "n") { // Remove by name
                std::string name;
                std::cout << ">> Event name: ";

                std::getline (std::cin, name);

                if (name.empty ()) {
                    log ("Error: Invalid event details specified.", 'e');
                    continue;
                }

                for (auto it = list.begin (); it != list.end (); it++) {
                    if (it->second
                            .getName () == name) {
                        list.erase (it);
                        removed = true;
                        log ("Event removal successful.", 's');
                        break;
                    }
                }
            }
            else if (input_buffer == "k") { // Clear list
                list.clear ();
                removed = true;
                log ("List cleared.", 's');
            }
            else if (input_buffer == "c") { // Cancel
                log ("Remove cancelled.", 'w');
                continue;
            }

            if (!removed) log ("Error: No such event exists.", 'e');
        }
        else if (selection == "vw") { // View events
            std::cout << "———————————————————————————\n";
            std::cout << "[a]  - View all events\n";
            std::cout << "[c]  - View by category\n";
            std::cout << "[$]   - Cancel\n";
            std::cout << "———————————————————————————\n";

            std::string choice;
            std::getline (std::cin, choice);

            if (taskCancelled (user_category, "View")) continue;

            else if (choice == "a") {
                if (list.empty ()) {
                    log ("List is empty.", 'w');
                    continue;
                }

                std::cout << "———————————————————————————\n";
                for (const auto &i: list) {
                    std::cout << i.second
                        .getName () << " (" << i.second
                        .getDate () << ")\n";
                }
                std::cout << "———————————————————————————\n";
            }
            else if (choice == "c") {
                std::cout << ">> Category: ";
                std::getline (std::cin, user_category);

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

        }
        else if (selection == "sv") { // Save events
            std::string choice, pathname, filename;

            std::cout << "———————————————————————————\n";
            std::cout << "[$]   - Cancel\n";
            std::cout << "———————————————————————————\n>> ";
            std::getline (std::cin, choice);

            if (taskCancelled (choice, "Save")) continue;

            std::cout << "Enter pathname. If project directory, enter '.':\n>> ";
            std::getline (std::cin, pathname);

            if (pathname.size () == 1 && pathname[0] == '.')
                pathname = PROJECT_PATH;
            else if (!validPathname (pathname)) {
                log ("Error: Filename contains illegal characters.", 'e');
                continue;
            }

            std::cout << "Enter filename or leave empty for default name, \"todo.txt\".\n>> ";
            std::getline (std::cin, filename);

            if (filename.empty ())
                filename = "todo.txt";
            else if (!validFilename (filename)) {
                log ("Error: Filename contains illegal characters.", 'e');
                continue;
            }

            formatDirectory (filename, pathname);

            std::ofstream outputFile (pathname + filename);
            if (outputFile.fail ()) {
                log ("Error: Something went wrong.", 'e');
                continue;
            }

            for (const auto&[category, event]: list) {
                outputFile << "cat=" << category << " => event=" << event.getName ();
                outputFile << " (date=" << event.getDate () << ")\n";
            }

            outputFile.close ();
            log ("File insertion successful.", 's');
        }
        else if (selection == "op") { // Open existing file
            std::string filepath, filename;
            bool format_error { false };

            std::cout << "———————————————————————————\n";
            std::cout << "[$]   - Cancel\n";
            std::cout << "———————————————————————————\n";
            std::cout << "Enter file path. Enter '.' if using project directory.\n>> ";
            std::getline (std::cin, filepath);

            if (taskCancelled (filepath, "Open file")) continue;

            if (filepath.size () == 1 && filepath[0] == '.')
                filepath = PROJECT_PATH;
            else if (!validPathname (filepath)) {
                log ("Error: Filename contains illegal characters.", 'e');
                continue;
            }

            std::cout << "Enter file name. Leave blank if using default name \"todo.txt\".\n>> ";
            std::getline (std::cin, filename);

            if (filename.empty ())
                filename = "todo.txt";
            else if (!validFilename (filename)) {
                log ("Error: Filename contains illegal characters.", 'e');
                continue;
            }

            formatDirectory (filename, filepath);
            std::ifstream inputFile (filepath + filename);
            list.clear ();

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

            std::string
                event = "event=",
                date = "date=",
                category = "cat=";
            size_t
                cat_start,
                event_start,
                date_start,
                size_of_arrow = 4;

            for (const auto &entry: entries) {
                cat_start = entry.find (category);
                event_start = entry.find (event);
                date_start = entry.find (date);

                if (cat_start == nil || event_start == nil || date_start == nil) {
                    format_error = true;
                    break;
                }

                user_category = entry.substr (
                    category.length (),
                    event_start - category.length () - size_of_arrow
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
