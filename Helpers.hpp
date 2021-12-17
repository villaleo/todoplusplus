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

#endif //HELPERS_HPP
