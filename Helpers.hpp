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

constexpr auto nil = std::string::npos;
using query_array = std::array<std::string, 3>;

namespace external {
    /**
    * Display menu with options and brief summaries.
    */
    void displayMenu ();

    /**
     * Check if buffer input is marked as discarded.
     * @param str The input buffer.
     * @param task The task name.
     * @return If task was cancelled.
     */
    bool taskCancelled (const std::string &str, const std::string &task);

    /**
     * Mutate a string to all lower case characters.
     * @param str The input string.
     * @return The string in all lower case.
     */
    std::string &toLower (std::string &str);

    /**
     * Check if a path is valid.
     * @param path The input string.
     * @return If the path is valid.
     */
    bool validFilename (const std::string &path);

    /**
     * Check if a filename is valid.
     * @param path The input string.
     * @return If the filename is valid.
     */
    bool validPathname (const std::string &path);

    /**
     * Display the help menu with details on each command.
     */
    void displayHelp ();

    /**
     * Display a colored message to the console.
     * @param msg The message to be displayed.
     * @param type The type of message to display.
     */
    void log (const std::string &msg, char type);

    /**
     * Trim the whitespace from the right of a string (in-place).
     * @param str The input buffer.
     */
    void trimRight (std::string &str);

    /**
    * A structure to hold constant expressions for color codes.
    */
    struct Color {
        constexpr static auto RED = "\033[0;31m";
        constexpr static auto GREEN = "\033[1;32m";
        constexpr static auto RESET = "\033[0m";
        constexpr static auto YELLOW = "\033[1;33m";
    };
}

namespace list_ops {
    /**
     * Asserts that a file path and name are in the correct format.
     * @param filename The name of the file.
     * @param pathname The path of the file.
     */
    void formatDirectory (std::string &filename, std::string &pathname);

    /**
     * Insert a new event into the event list.
     * @param query An array with the query parameters (name, date, category).
     * @param list The list of events to insert into.
     */
    void insertIntoList (query_array &query, std::multimap<std::string, Event> &list);

    /**
     * Remove an event from the event list.
     * @param query An array with the query parameters (name, date, category).
     * @param list The list of events to remove from.
     */
    void removeFromList (query_array &query, std::multimap<std::string, Event> &list);

    /**
     * Display the events in the event list.
     * @param list The list of events to display.
     */
    void displayList (const std::multimap<std::string, Event> &list);

    /**
     * Save the events in the event list to a file.
     * @param list The list of events to save.
     * @param PROJECT_DIR The project directory.
     */
    void saveListToFile (const std::multimap<std::string, Event> &list, const std::string &PROJECT_DIR);

    /**
     * Load the events from a file into the event list.
     * @param list The list of events to load.
     * @param PROJECT_DIR The project directory.
     */
    void loadListFromFile (std::multimap<std::string, Event> &list, const std::string &PROJECT_DIR);
}

#endif //HELPERS_HPP
