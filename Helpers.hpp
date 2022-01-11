//
// Created by Leonardo Villalobos on 12/17/21.
//

#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <set>
#include <algorithm>
#include <cctype>
#include <array>
#include <map>
#include <locale>
#include "Event.hpp"

using namespace std;

constexpr auto nil = string::npos;
constexpr size_t USER_DETAILS_SIZE = 3;

namespace helpers {
    /**
    * Display menu with options and brief summaries.
    */
    void displayMenu ();

    /**
     * Mutate a string to all lower case characters.
     * @param str The input string.
     * @return The string in all lower case.
     */
    void toLower (string &str);

    /**
     * Check if a file is valid.
     * @param file The input string.
     * @return If the file is valid.
     */
    bool validFilename (const string &file);

    /**
     * Check if a filename is valid.
     * @param path The input string.
     * @return If the filename is valid.
     */
    bool validPathname (const string &path);

    /**
     * Display the help menu with details on each command.
     */
    void displayHelp ();

    /**
     * Display a colored message to the console.
     * @param msg The message to be displayed.
     * @param type The type of message to display.
     */
    void log (const string &msg, char type);

    /**
     * Trim the whitespace from the right of a string.
     * @param str The input buffer.
     */
    string trimRight (const string &str);

    /**
     * Trim the whitespace from the left of a string.
     * @param str The input buffer.
     */
    string trimLeft (const string &str);

    /**
     * Trim the whitespace from the left and right of a string.
     * @param str The input buffer.
     */
    inline string trim (const string &str) { return trimRight (trimRight (str)); }

    /**
    * A structure to hold constant expressions for color codes.
    */
    struct Color {
        constexpr static auto RED = "\033[0;31m";
        constexpr static auto GREEN = "\033[1;32m";
        constexpr static auto RESET = "\033[0m";
        constexpr static auto YELLOW = "\033[1;33m";
    };

    /**
     * Asserts that a file path and name are in the correct format.
     * @param filename The name of the file.
     * @param pathname The path of the file.
     */
    void formatDirectory (string &filename, string &pathname);
} // end namespace helpers

namespace operations {
    /**
     * Insert a new event into the event list.
     */
    void insertIntoList ();

    /**
     * Remove an event from the event list.
     * @param flag A flag to specify how to remove the event.
     */
    void removeFromList (const string &flag);

    /**
     * Display the events in the event list.
     * @param flag A flag to specify how to display the events.
     */
    void displayList (const string &flag);

    /**
     * Save the events in the event list to a file.
     * @param filepath The path of the file to save the events to.
     * @param filename The name of the file to save the events to.
     */
    void saveToFile (string &filepath, string &filename);

    /**
     * Load the events from a file into the event list.
     * @param filepath The path of the file to load the events from.
     * @param filename The name of the file to load the events from.
     */
    void loadFromPath (string &filepath, string &filename);
} // end namespace operations

#endif //HELPERS_HPP
