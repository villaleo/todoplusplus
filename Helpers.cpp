//
// Created by Leonardo Villalobos on 12/17/21.
//

#include <fstream>
#include <iostream>
#include <vector>
#include "Helpers.hpp"

namespace external {
    void displayMenu () {
        std::cout << "———————————————————————————\n[ins] - Insert an event    \n";
        std::cout << "[rm]  - Remove an event    \n[vw]  - View events        \n";
        std::cout << "[sv]  - Save list to a file\n[op]  - Open existing file \n";
        std::cout << "[q]   - Quit the program   \n[h]   - Help               \n";
        std::cout << "———————————————————————————\n";
    }

    bool validFilename (const std::string &path) {
        std::set<char> illegal_chars {
            '#', '%', '&', '{', '}', '\\',
            '<', '>', '*', '?', '/', '$',
            '!', '\'', '\"', ':', '@', '+',
            '`', '|', '=', ' '
        };
        for (const char c: path) {
            if (illegal_chars.find (c) != illegal_chars.end ())
                return false;
        }
        return true;
    }

    bool validPathname (const std::string &path) {
        std::set<char> illegal_chars {
            '#', '%', '&', '{', '}',
            '<', '>', '*', '?', '$',
            '!', '\'', '\"', ':', '@',
            '`', '|', '=', '+', ' '
        };
        for (const char c: path) {
            if (illegal_chars.find (c) != illegal_chars.end ())
                return false;
        }
        return true;
    }

    void displayHelp () {
        std::cout << "———————————————————————————\n[ins] - Insert an event into the list. Event details cannot";
        std::cout << " be empty.\n[rm]  - Remove an event from the list. Event details cannot be empty.\n";
        std::cout << "[vw]  - View events in the list.\n[sv]  - Save list to a file. Filename and path cannot contain ";
        std::cout << "spaces nor illegal characters";
        std::cout << "\n[op]  - Open existing file. Filename and path cannot contain spaces nor illegal characters";
        std::cout << "\n[q]   - Terminate the program\n" << Color::YELLOW << "Illegal characters include: ";
        std::cout << R"('#', '%', '&', '{', '}', etc.)" << Color::RESET;
        std::cout << "\n———————————————————————————\n";
    }

    bool taskCancelled (const std::string &str, const std::string &task) {
        if (str.length () == 1 && str[0] == '$') {
            std::string message = task + " cancelled.";
            log (message, 'w');
            return true;
        }
        return false;
    };

    std::string &toLower (std::string &str) {
        for (char &c: str)
            c = static_cast<char> (std::tolower (c));
        return str;
    }

    void log (const std::string &msg, char type) {
        type = static_cast<char> (std::tolower (type));
        if (type == 'w')
            std::cout << "\n* " << Color::YELLOW << msg << Color::RESET << '\n';
        else if (type == 'e')
            std::cout << "\n* " << Color::RED << msg << Color::RESET << '\n';
        else if (type == 's')
            std::cout << "\n* " << Color::GREEN << msg << Color::RESET << '\n';
        else
            std::cout << "\n* " << msg << '\n';
    }

    void trimRight (std::string &str) {
        str.erase (
            std::find_if (
                str.rbegin (), str.rend (), [] (unsigned char c) {
                    return !std::isspace (c);
                }
            ).base (), str.end ());
    }

    void formatDirectory (std::string &filename, std::string &pathname) {
        // Check if path ends correctly
        if (pathname.back () != '/' || pathname.back () != '\\')
            pathname += (std::find (pathname.begin (), pathname.end (), '/') != pathname.end ()) ? '/' : '\\';

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
    }
}

namespace list_ops {
    using namespace external;

    void insertIntoList (query_array &query, std::multimap<std::string, Event> &list) {
        std::cout << "[$]   - Cancel\n>> Event name: ";
        std::getline (std::cin, query.at (0));

        if (taskCancelled (query.at (0), "Insert")) return;

        std::cout << ">> Event date: ";
        std::getline (std::cin, query.at (1));

        if (taskCancelled (query.at (1), "Insert")) return;

        std::cout << ">> Event category: ";
        std::getline (std::cin, query.at (2));

        if (taskCancelled (query.at (2), "Insert")) return;

        auto event = Event (query.at (0), query.at (1));
        if (!event.null ()) {
            list.insert ({ query.at (2), event });
            log ("Event insertion successfully.", 's');
        }
        else
            log ("Error: Invalid event details specified.", 'e');
    }

    void removeFromList (query_array &query, std::multimap<std::string, Event> &list) {
        bool removed = false;
        std::string input_buffer;

        std::cout << "———————————————————————————\n";
        std::cout << "[a]  - Remove by specific details\n";
        std::cout << "[n]  - Remove by name\n";
        std::cout << "[k]  - Clear list\n";
        std::cout << "[c]  - Cancel\n";
        std::cout << "———————————————————————————\n>> ";

        std::getline (std::cin, input_buffer);

        if (taskCancelled (input_buffer, "Remove")) return;

        if (input_buffer == "a") { // Remove by specific details
            std::cout << ">> Event name: ";
            std::getline (std::cin, query.at (0));
            std::cout << ">> Event date: ";
            std::getline (std::cin, query.at (1));
            std::cout << ">> Event category: ";
            std::getline (std::cin, query.at (2));

            auto event = Event (query.at (0), query.at (1));
            if (event.null ()) {
                log ("Error: Invalid event details specified.", 'e');
                return;
            }

            auto[begin, end] = list.equal_range (query.at (2));
            for (auto i = begin; i != end; i++) {
                if (i->second == event) {
                    list.erase (i);
                    removed = true;
                    log ("Event removal successful.", 's');
                    break;
                }
            }

            if (!removed) log ("Error: Event not found.", 'e');
        }
        else if (input_buffer == "n") { // Remove by name
            std::string name;
            std::cout << ">> Event name: ";

            std::getline (std::cin, name);

            if (name.empty ()) {
                log ("Error: Invalid event details specified.", 'e');
                return;
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
            return;
        }

        if (!removed) log ("Error: No such event exists.", 'e');
    }

    void displayList (const std::multimap<std::string, Event> &list) {
        std::cout << "———————————————————————————\n";
        std::cout << "[a]  - View all events\n";
        std::cout << "[c]  - View by category\n";
        std::cout << "[$]   - Cancel\n";
        std::cout << "———————————————————————————\n";

        std::string choice, category;
        std::getline (std::cin, choice);

        if (taskCancelled (choice, "View")) return;

        else if (choice == "a") {
            if (list.empty ()) {
                log ("List is empty.", 'w');
                return;
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
            std::getline (std::cin, category);

            if (list.find (category) == list.end ()) {
                log ("Error: No matching category found.", 'e');
                return;
            }

            auto[begin, end] = list.equal_range (category);
            if (begin == list.end ()) {
                log ("Error: List is empty, nothing to display.", 'e');
                return;
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

    void saveListToFile (const std::multimap<std::string, Event> &list, const std::string &PROJECT_DIR) {
        std::string choice, pathname, filename;

        std::cout << "———————————————————————————\n";
        std::cout << "[$]   - Cancel\n";
        std::cout << "———————————————————————————\n>> ";


        std::cout << "Enter pathname. If project directory, enter '.':\n>> ";
        std::getline (std::cin, pathname);

        if (taskCancelled (pathname, "Save")) return;

        if (pathname.size () == 1 && pathname[0] == '.')
            pathname = PROJECT_DIR;
        else if (!validPathname (pathname)) {
            log ("Error: Filename contains illegal characters.", 'e');
            return;
        }

        std::cout << "Enter filename or leave empty for default name, \"todo.txt\".\n>> ";
        std::getline (std::cin, filename);

        if (taskCancelled (filename, "Save")) return;

        if (filename.empty ())
            filename = "todo.txt";
        else if (!validFilename (filename)) {
            log ("Error: Filename contains illegal characters.", 'e');
            return;
        }

        formatDirectory (filename, pathname);

        std::ofstream outputFile (pathname + filename);
        if (outputFile.fail ()) {
            log ("Error: Something went wrong.", 'e');
            return;
        }

        for (const auto&[category, event]: list) {
            outputFile << "cat=" << category << " => event=" << event.getName ();
            outputFile << " (date=" << event.getDate () << ")\n";
        }

        outputFile.close ();
        log ("File insertion successful.", 's');
    }

    void loadListFromFile (std::multimap<std::string, Event> &list, const std::string &PROJECT_DIR) {
        std::string filepath, filename;
        bool format_error { false };

        std::cout << "———————————————————————————\n";
        std::cout << "[$]   - Cancel\n";
        std::cout << "———————————————————————————\n";
        std::cout << "Enter file path. Enter '.' if using project directory.\n>> ";
        std::getline (std::cin, filepath);

        if (taskCancelled (filepath, "Open file")) return;

        if (filepath.size () == 1 && filepath[0] == '.')
            filepath = PROJECT_DIR;
        else if (!validPathname (filepath)) {
            log ("Error: Filename contains illegal characters.", 'e');
            return;
        }

        std::cout << "Enter file name. Leave blank if using default name \"todo.txt\".\n>> ";
        std::getline (std::cin, filename);

        if (filename.empty ())
            filename = "todo.txt";
        else if (!validFilename (filename)) {
            log ("Error: Filename contains illegal characters.", 'e');
            return;
        }

        formatDirectory (filename, filepath);
        std::ifstream inputFile (filepath + filename);
        list.clear ();

        std::vector<std::string> entries;
        std::string temp;

        if (inputFile.fail ()) {
            log ("Error: Something went wrong.", 'e');
            return;
        }

        while (std::getline (inputFile, temp))
            entries.push_back (temp);
        inputFile.close ();

        if (entries.empty ()) {
            log ("Error: Nothing to insert.", 'e');
            return;
        }

        std::string
            event = "event=",
            date = "date=",
            category = "cat=",
            event_query,
            date_query,
            category_query;

        size_t
            cat_start,
            event_start,
            date_start,
            size_of_arrow = 4,
            excess_spaces = 3,
            last_parenthesis = 1;

        for (const auto &entry: entries) {
            cat_start = entry.find (category);
            event_start = entry.find (event);
            date_start = entry.find (date);

            if (cat_start == nil || event_start == nil || date_start == nil) {
                format_error = true;
                break;
            }

            category_query = entry.substr (
                category.length (),
                event_start - category.length () - size_of_arrow
            );
            event_query = entry.substr (
                event_start + event.length (),
                date_start - cat_start - event_start - date.length () - excess_spaces
            );
            date_query = entry.substr (
                date_start + date.length (),
                entry.substr (date_start + date.length ())
                    .length () - last_parenthesis
            );

            list.insert (
                {
                    category_query,
                    Event (event_query, date_query)
                }
            );
        }

        if (format_error)
            log ("Error: File format is invalid.", 'e');
        else
            log ("Contents loaded from file successfully!", 's');
    }
}
