#ifndef STRING_PARSER_H
#define STRING_PARSER_H

#include "Arduino.h"
#include <tuple>

/* Function to split a function in two, delimited by a certain character.
   Best used after String::trim() as it removes redundant characters, including '\r' and '\n' */
std::tuple<String, String> splitFunc(String, char);

/* Default function, delimits based on the whitespace character */
std::tuple<String, String> splitFunc(String, char = ' ');

#endif