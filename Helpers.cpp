//
// Created by Leonardo Villalobos on 12/17/21.
//

#include <fstream>
#include <iostream>
#include <vector>
#include "Helpers.hpp"

extern multimap<string, Event> list;

namespace helpers {
    void displayMenu () {
        cout << "  ______          __                \n /_  __/___  ____/ /___    __    __ \n";
        cout << "  / / / __ \\/ __  / __ \\__/ /___/ /_\n / / / /_/ / /_/ / /_/ /_  __/_  __/\n";
        cout << "/_/  \\____/\\____/\\____/ /_/   /_/   \n";
        cout << "Created by Leonardo Villalobos.\nEnter <help> for help.\n\n";
    } // end displayMenu

    bool validFilename (const string &file) {
        set<char> illegalCharacters {
            '#', '%', '&', '{', '}', '\\', '<', '>', '*', '?', '/', '$', '!', '\'', '\"', ':', '@', '+', '`', '|', '=', ' '
        };
        for (const char c: file) {
            if (illegalCharacters.find (c) != illegalCharacters.end ())
                return false;
        }
        return true;
    } // end validFilename

    bool validPathname (const string &path) {
        set<char> illegal_chars {
            '#', '%', '&', '{', '}', '<', '>', '*', '?', '$', '!', '\'', '\"', ':', '@', '`', '|', '=', '+', ' '
        };
        for (const char c: path) {
            if (illegal_chars.find (c) != illegal_chars.end ())
                return false;
        }
        return true;
    } // end validPathname

    void displayHelp () {
        cout << "======================================================\n";
        cout << "<add>                       - Insert an event into the list. Each event must contain a name, date,\n";
        cout << "                              and category.\n";
        cout << "<del> [--d | --a | --n]     - Remove an event from the list. If no argument is present, the --d flag";
        cout << "\n                              is assumed by default.\n";
        cout << "                                   --d: Delete an event given specific details.\n";
        cout << "                                   --a: Delete all events in the list.\n";
        cout << "                                   --n: Delete an event with the given name.\n";
        cout << "<view> [--c | --a]          - View events in the list. If no argument is present, the --a flag is\n";
        cout << "                              assumed by default.\n";
        cout << "                                   --c: View events which fall in the given category.\n";
        cout << "                                   --a: View all events in the list.\n";
        cout << "<save> <-p=...> <-f=...>    - Save list to a file. File path and names cannot contain illegal ";
        cout << "characters\n";
        cout << "<open> <-p=...> <-f=...>    - Open existing file. File path and name cannot contain illegal ";
        cout << "characters.\n" << Color::YELLOW << "                            ";
        cout << "Illegal characters for file names:\n";
        cout << "                                   " << R"('#', '%', '&', '{', '}', '\', '<', '>', '*', '?', '=',)";
        cout << "\n                                   " << R"('/', '$', '!', ''', '"', ':', '@', '+', '`', '|', ' ')";
        cout << "\n                            Illegal characters for file paths:\n";
        cout << "                                   " << R"('#', '%', '&', '{', '}', '<', '>', '*', '?', '$', '!',)";
        cout << "\n                                   " << R"(''', '"', ':', '@', '`', '|', '=', '+', ' ')";
        cout << Color::RESET << "\n<set> <@...> <...>          - Define a special variable which can be used in place\n";
        cout << "                              of text in special contexts.\n";
        cout << "                                   @PATH: Stores a path name. Can be used in place of a path. Reference\n";
        cout << "                                          the variable by typing '@PATH' in the context of a file path.\n";
        cout << "                                   @NAME: Stores a file name. Can be used in place of a file name.\n";
        cout << "                                          Reference the variable by typing '@NAME' in the context of a\n";
        cout << "                                          file name.\n";
        cout << "\n<quit>                      - Terminate the program.\n";
        cout << "======================================================\n";
    } // end displayHelp

    void toLower (string &str) {
        for (char &c: str)
            c = static_cast<char> (tolower (c));
    } // end toLower

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
    } // end log

    string trimRight (const string &str) {
        size_t end = str.find_last_not_of (" \n\r\t\f\v");
        return (end == nil) ? "" : str.substr (0, end + 1);
    } // end trimRight

    string trimLeft (const string &str) {
        size_t start = str.find_first_not_of (" \n\r\t\f\v");
        return (start == nil) ? "" : str.substr (start);
    } // end trimLeft

    void formatDirectory (string &filename, string &pathname) {
        char pathDelimiter = pathname.find ('/') == nil ? '\\' : '/';
        if (pathname.back () != pathDelimiter)
            pathname += pathDelimiter;

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
    } // end formatDirectory
} // end namespace helpers

namespace operations {
    using namespace helpers;

    void insertIntoList () {
        string eventName, eventDate, eventCategory;

        cout << ">> Event name: ";
        getline (cin, eventName);
        cout << ">> Event date: ";
        getline (cin, eventDate);
        cout << ">> Event category: ";
        getline (cin, eventCategory);

        auto eventQuery = Event (eventName, eventDate);

        if (eventName.empty ()) {
            log ("Event name cannot be empty.", 'e');
            return;
        }
        else if (eventDate.empty ()) {
            log ("Event date cannot be empty.", 'e');
            return;
        }
        else if (eventCategory.empty ()) {
            log ("Event category cannot be empty.", 'e');
            return;
        }

        list.insert (make_pair (eventCategory, eventQuery));
        log ("Event insertion successfully.", 's');
    } // end insertIntoList

    void removeFromList (const string &flag) {
        bool removed = false;

        if (flag == "del")
            goto RemoveByName;

        if (list.empty ()) {
            if (flag != "--d" && flag != "--a" && flag != "--n") {
                log ("Error: Invalid flag specified.", 'e');
                return;
            }
            log ("List is already empty.", 'w');
            return;
        }

        if (flag == "--n") { // Remove by name
            RemoveByName:

            string eventName;
            if (list.empty ()) {
                log ("List is already empty.", 'w');
                return;
            }

            cout << ">> Event name: ";
            getline (cin, eventName);

            if (eventName.empty ()) {
                log ("Error: Event name cannot be empty.", 'e');
                return;
            }

            for (auto it = list.begin (); it != list.end (); it++) {
                if (it->second.getName () == eventName) {
                    list.erase (it);
                    removed = true;
                    log ("Event removal successful.", 's');
                    break;
                }
            }
        }
        else if (flag == "--a") { // Clear list
            list.clear ();
            removed = true;
            log ("List cleared.", 's');
        }
        else if (flag == "--d") { // Remove by specific details
            RemoveByDetails:

            string eventName, eventDate, eventCategory;

            cout << ">> Event name: ";
            getline (cin, eventName);
            cout << ">> Event date: ";
            getline (cin, eventDate);
            cout << ">> Event category: ";
            getline (cin, eventCategory);

            auto event = Event (eventName, eventDate);
            if (eventName.empty ()) {
                log ("Error: Event name cannot be empty.", 'e');
                return;
            }
            else if (eventDate.empty ()) {
                log ("Error: Event date cannot be empty.", 'e');
                return;
            }
            else if (eventCategory.empty ()) {
                log ("Error: Event category cannot be empty.", 'e');
                return;
            }

            auto[begin, end] = list.equal_range (eventCategory);
            for (auto it = begin; it != end; it++) {
                if (it->second == event) {
                    list.erase (it);
                    removed = true;
                    log ("Event removal successful.", 's');
                    break;
                }
            }

            if (!removed)
                log ("Error: Event not found.", 'e');
        }
        else {
            if (flag.empty ())
                goto RemoveByDetails;

            log ("Error: Invalid flag specified.", 'e');
            return;
        }

        if (!removed)
            log ("Error: No such event exists.", 'e');
    } // end removeFromList

    void displayList (const string &flag) {
        if (flag == "view")
            goto DisplayAll;

        if (list.empty ()) {
            if (flag != "--c" && flag != "--a") {
                log ("Error: Invalid flag specified.", 'e');
                return;
            }
            log ("List is empty.", 'w');
            return;
        }

        if (flag == "--a") {  // Display all
            DisplayAll:
            if (list.empty ()) {
                log ("Nothing to display. List is empty.", 'w');
                return;
            }

            cout << "======================================================\n";
            for (const auto &i: list) {
                cout << i.first << ": " << i.second.getName () << " (" << i.second.getDate () << ")\n";
            }
            cout << "======================================================\n";
        }
        else if (flag == "--c") {  // Display by category
            if (list.empty ()) {
                log ("Nothing to display. List is empty.", 'w');
                return;
            }

            string category;
            cout << ">> Category: ";
            getline (cin, category);

            if (list.find (category) == list.end ()) {
                log ("Error: No matching category found.", 'e');
                return;
            }

            auto[begin, end] = list.equal_range (category);
            string buffer;

            cout << "======================================================\n";
            for (auto i = begin; i != end; i++)
                cout << i->first << ": " << i->second.getName () << " (" << i->second.getDate () + ")\n";
            cout << "======================================================\n";
        }
        else
            log ("Error: Invalid flag specified.", 'e');
    } // end displayList

    void saveToFile (string &filepath, string &filename) {
        filepath = filepath.substr (3);  // Remove the "p=" prefix.
        filename = filename.substr (3);  // Remove the "f=" prefix.

        if (filepath == "@PATH") {
            filepath = variables::defaultPath;
        }
        if (filename == "@NAME") {
            filename = variables::defaultName;
        }

        if (filepath.empty ()) {
            log ("Error: Filepath cannot be empty.", 'e');
            return;
        }
        else if (filename.empty ()) {
            log ("Error: Filename cannot be empty.", 'e');
            return;
        }

        if (!validPathname (filepath)) {
            log ("Error: Path name contains illegal characters.", 'e');
            return;
        }
        if (!validFilename (filename)) {
            log ("Error: File name contains illegal characters.", 'e');
            return;
        }
        formatDirectory (filename, filepath);

        ofstream outputFile (filepath + filename);
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
    } // end saveToFile

    void loadFromPath (string &filepath, string &filename) {
        bool formatError = false;

        filepath = filepath.substr (3);  // Remove the "p=" prefix.
        filename = filename.substr (3);  // Remove the "f=" prefix.

        if (filepath == "@PATH") {
            filepath = variables::defaultPath;
        }
        if (filename == "@NAME") {
            filename = variables::defaultName;
        }

        if (!validPathname (filepath)) {
            log ("Error: Path name contains illegal characters.", 'e');
            return;
        }
        if (!validFilename (filename)) {
            log ("Error: File name contains illegal characters.", 'e');
            return;
        }
        formatDirectory (filename, filepath);

        ifstream inputFile (filepath + filename);
        if (inputFile.fail ()) {
            log ("Error: Something went wrong.", 'e');
            return;
        }

        list.clear ();
        vector<string> entries;
        string temp;

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
                formatError = true;
                break;
            }

            category_query = entry.substr (category.length (), event_start - category.length () - size_of_arrow);
            event_query = entry.substr (
                event_start + event.length (), date_start - cat_start - event_start - date.length () - excess_spaces
            );
            date_query = entry.substr (
                date_start + date.length (), entry.substr (date_start + date.length ()).length () - last_parenthesis
            );

            list.insert (make_pair (category_query, Event (event_query, date_query)));
        }

        if (formatError)
            log ("Error: File format is invalid.", 'e');
        else
            log ("Contents loaded from file successfully!", 's');
    } // end loadFromPath

    void set (string &flag, string &argument) {
        if (flag == "@PATH") {
            if (!validPathname (argument))
                log ("Error: Path name contains invalid characters.", 'e');

            variables::defaultPath = argument;
        }
        else if (flag == "@NAME") {
            if (!validFilename (argument))
                log ("Error: File name contains invalid characters.", 'e');

            variables::defaultName = argument;
        }
        else {
            log ("Error: Invalid flag specified.", 'e');
        }
    } // end set
} // end namespace operations
