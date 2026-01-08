#pragma once

#include <Arduino.h>

class Utf8 {
public:
  static bool nextCodepoint(const char* s, size_t len, size_t& i, uint32_t& out);
};
