#pragma once

#include <Arduino.h>

class Font {
public:
  static constexpr uint8_t W = 6;
  static constexpr uint8_t H = 10;

  static bool getGlyph(uint32_t codepoint, uint16_t outCols[W]);
  static uint32_t normalize(uint32_t codepoint);
};
