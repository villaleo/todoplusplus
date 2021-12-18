//
// Created by Leonardo Villalobos on 12/17/21.
//

#include "Helpers.hpp"

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
    std::cout << "spaces nor illegal characters\n\t\t" << R"(such as '#', '%', '&', '{', '}', etc.)";
    std::cout << "\n[q]   - Terminate the program   \n———————————————————————————\n";
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