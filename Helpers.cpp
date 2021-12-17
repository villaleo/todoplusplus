//
// Created by Leonardo Villalobos on 12/17/21.
//

#include "Helpers.hpp"

void displayMenu () {
    std::cout << "———————————————————————————\n[ins] - Insert an event    \n";
    std::cout << "[rm]  - Remove an event    \n[vw]  - View events        \n";
    std::cout << "[sv]  - Save list to a file\n[q]   - Quit the program   \n";
    std::cout << "[h]   - Help               \n———————————————————————————\n";
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
        std::cout << "\n* " << task << " cancelled.\n";
        return true;
    }
    return false;
};

std::string &toLower (std::string &str) {
    for (char &c: str)
        c = static_cast<char>(std::tolower (c));
    return str;
}
