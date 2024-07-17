#ifndef STRING_PARSER_H
#define STRING_PARSER_H

#include "Arduino.h"
#include <tuple>

std::tuple<uint8_t *, uint32_t *> extractFunc(String str);

#endif