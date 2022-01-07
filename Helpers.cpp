//
// Created by Leonardo Villalobos on 12/17/21.
//

#include <fstream>
#include <iostream>
#include <vector>
#include "Helpers.hpp"

namespace external {
    void displayMenu () {
        cout << "  ______          __                \n /_  __/___  ____/ /___    __    __ \n";
        cout << "  / / / __ \\/ __  / __ \\__/ /___/ /_\n / / / /_/ / /_/ / /_/ /_  __/_  __/\n";
        cout << "/_/  \\____/\\____/\\____/ /_/   /_/   \n";
        cout << "Created by Leonardo Villalobos.\nPress [h] for help.\n\n";
    }

    bool validFilename (const string &path) {
        set<char> illegal_chars {
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

    bool validPathname (const string &path) {
        set<char> illegal_chars {
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
        cout << "———————————————————————————\n[ins] - Insert an event into the list. Event details cannot";
        cout << " be empty.\n[rm]  - Remove an event from the list. Event details cannot be empty.\n";
        cout << "[vw]  - View events in the list.\n[sv]  - Save list to a file. Filename and path cannot contain ";
        cout << "spaces nor illegal characters";
        cout << "\n[op]  - Open existing file. Filename and path cannot contain spaces nor illegal characters";
        cout << "\n[q]   - Terminate the program\n" << Color::YELLOW << "Illegal characters include: ";
        cout << R"('#', '%', '&', '{', '}', etc.)" << Color::RESET;
        cout << "\n———————————————————————————\n";
    }

    bool taskCancelled (const string &str, const string &task) {
        if (str.length () == 1 && str[0] == '$') {
            string message = task + " cancelled.";
            log (message, 'w');
            return true;
        }
        return false;
    }

    string &toLower (string &str) {
        for (char &c: str)
            c = static_cast<char> (tolower (c));
        return str;
    }

    void log (const string &msg, char type) {
        type = static_cast<char> (tolower (type));
        if (type == 'w')
            cout << "\n* " << Color::YELLOW << msg << Color::RESET << '\n';
        else if (type == 'e')
            cout << "\n* " << Color::RED << msg << Color::RESET << '\n';
        else if (type == 's')
            cout << "\n* " << Color::GREEN << msg << Color::RESET << '\n';
        else
            cout << "\n* " << msg << '\n';
    }

    void trimRight (string &str) {
        str.erase (
            find_if (
                str.rbegin (), str.rend (), [] (unsigned char c) {
                    return !isspace (c);
                }
            ).base (), str.end ());
    }

} // namespace external

namespace list_ops {
    using namespace external;

    void formatDirectory (string &filename, string &pathname) {
        // Check if path ends correctly
        if (pathname.back () != '/' || pathname.back () != '\\')
            pathname += (find (pathname.begin (), pathname.end (), '/') != pathname.end ()) ? '/' : '\\';

        // Check if file extension is correct
        auto extension = find (filename.begin (), filename.end (), '.');
        if (extension == filename.end ())
            filename += ".txt";
        else {
            // Change extension
            string newFilename;
            for (auto i = filename.begin (); i != extension; i++)
                newFilename += *i;
            filename = newFilename + ".txt";
        }
    }

    void insertIntoList (array<string, USER_DETAILS_SIZE> &query, multimap<string, Event> &list) {
        cout << "[$]   - Cancel\n>> Event name: ";
        getline (cin, query.at (0));

        if (taskCancelled (query.at (0), "Insert")) return;

        cout << ">> Event date: ";
        getline (cin, query.at (1));

        if (taskCancelled (query.at (1), "Insert")) return;

        cout << ">> Event category: ";
        getline (cin, query.at (2));

        if (taskCancelled (query.at (2), "Insert")) return;

        auto event = Event (query.at (0), query.at (1));
        if (!event.null ()) {
            list.insert ({ query.at (2), event });
            log ("Event insertion successfully.", 's');
        }
        else
            log ("Error: Invalid event details specified.", 'e');
    }

    void removeFromList (array<string, USER_DETAILS_SIZE> &query, multimap<string, Event> &list) {
        bool removed = false;
        string input_buffer;

        cout << "———————————————————————————\n";
        cout << "[a]  - Remove by specific details\n";
        cout << "[n]  - Remove by name\n";
        cout << "[k]  - Clear list\n";
        cout << "[c]  - Cancel\n";
        cout << "———————————————————————————\n>> ";

        getline (cin, input_buffer);

        if (taskCancelled (input_buffer, "Remove")) return;

        if (input_buffer == "a") { // Remove by specific details
            cout << ">> Event name: ";
            getline (cin, query.at (0));
            cout << ">> Event date: ";
            getline (cin, query.at (1));
            cout << ">> Event category: ";
            getline (cin, query.at (2));

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
            string name;
            cout << ">> Event name: ";

            getline (cin, name);

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

    void displayList (const multimap<string, Event> &list) {
        cout << "———————————————————————————\n";
        cout << "[a]  - View all events\n";
        cout << "[c]  - View by category\n";
        cout << "[$]   - Cancel\n";
        cout << "———————————————————————————\n";

        string choice, category;
        getline (cin, choice);

        if (taskCancelled (choice, "View")) return;

        else if (choice == "a") {
            if (list.empty ()) {
                log ("List is empty.", 'w');
                return;
            }

            cout << "———————————————————————————\n";
            for (const auto &i: list) {
                cout << i.second
                    .getName () << " (" << i.second
                    .getDate () << ")\n";
            }
            cout << "———————————————————————————\n";
        }
        else if (choice == "c") {
            cout << ">> Category: ";
            getline (cin, category);

            if (list.find (category) == list.end ()) {
                log ("Error: No matching category found.", 'e');
                return;
            }

            auto[begin, end] = list.equal_range (category);
            if (begin == list.end ()) {
                log ("Error: List is empty, nothing to display.", 'e');
                return;
            }

            cout << "———————————————————————————\n";
            string buffer;
            for (auto i = begin; i != end; i++) {
                cout << i->second
                            .getName () + " (";
                cout << i->second
                            .getDate () + ")\n";
            }
            cout << "———————————————————————————\n";
        }
    }

    void saveListToFile (const multimap<string, Event> &list, const string &PROJECT_DIR) {
        string choice, pathname, filename;

        cout << "———————————————————————————\n";
        cout << "[$]   - Cancel\n";
        cout << "———————————————————————————\n>> ";


        cout << "Enter pathname. If project directory, enter '.':\n>> ";
        getline (cin, pathname);

        if (taskCancelled (pathname, "Save")) return;

        if (pathname.size () == 1 && pathname[0] == '.')
            pathname = PROJECT_DIR;
        else if (!validPathname (pathname)) {
            log ("Error: Filename contains illegal characters.", 'e');
            return;
        }

        cout << "Enter filename or leave empty for default name, \"todo.txt\".\n>> ";
        getline (cin, filename);

        if (taskCancelled (filename, "Save")) return;

        if (filename.empty ())
            filename = "todo.txt";
        else if (!validFilename (filename)) {
            log ("Error: Filename contains illegal characters.", 'e');
            return;
        }

        formatDirectory (filename, pathname);

        ofstream outputFile (pathname + filename);
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

    void loadListFromFile (multimap<string, Event> &list, const string &PROJECT_DIR) {
        string filepath, filename;
        bool format_error { false };

        cout << "———————————————————————————\n";
        cout << "[$]   - Cancel\n";
        cout << "———————————————————————————\n";
        cout << "Enter file path. Enter '.' if using project directory.\n>> ";
        getline (cin, filepath);

        if (taskCancelled (filepath, "Open file")) return;

        if (filepath.size () == 1 && filepath[0] == '.')
            filepath = PROJECT_DIR;
        else if (!validPathname (filepath)) {
            log ("Error: Filename contains illegal characters.", 'e');
            return;
        }

        cout << "Enter file name. Leave blank if using default name \"todo.txt\".\n>> ";
        getline (cin, filename);

        if (filename.empty ())
            filename = "todo.txt";
        else if (!validFilename (filename)) {
            log ("Error: Filename contains illegal characters.", 'e');
            return;
        }

        formatDirectory (filename, filepath);
        ifstream inputFile (filepath + filename);
        list.clear ();

        vector<string> entries;
        string temp;

        if (inputFile.fail ()) {
            log ("Error: Something went wrong.", 'e');
            return;
        }

        while (getline (inputFile, temp))
            entries.push_back (temp);

        inputFile.close ();

        if (entries.empty ()) {
            log ("Error: Nothing to insert.", 'e');
            return;
        }

        string event = "event=", date = "date=", category = "cat=", event_query, date_query, category_query;
        size_t cat_start, event_start, date_start, size_of_arrow = 4, excess_spaces = 3, last_parenthesis = 1;

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

            list.insert ({ category_query, Event (event_query, date_query) });
        }

        if (format_error)
            log ("Error: File format is invalid.", 'e');
        else
            log ("Contents loaded from file successfully!", 's');
    }
}
