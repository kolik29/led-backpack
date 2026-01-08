#include "Utf8.h"

bool Utf8::nextCodepoint(const char* s, size_t len, size_t& i, uint32_t& out) {
  if (i >= len) return false;

  uint8_t b0 = (uint8_t)s[i++];

  if (b0 < 0x80) {
    out = b0;
    return true;
  }

  if ((b0 & 0xE0) == 0xC0) {
    if (i >= len) return false;
    uint8_t b1 = (uint8_t)s[i++];
    if ((b1 & 0xC0) != 0x80) return false;

    out = ((uint32_t)(b0 & 0x1F) << 6) | (uint32_t)(b1 & 0x3F);
    return true;
  }

  if ((b0 & 0xF0) == 0xE0) {
    if (i + 1 >= len) return false;
    uint8_t b1 = (uint8_t)s[i++];
    uint8_t b2 = (uint8_t)s[i++];
    if ((b1 & 0xC0) != 0x80 || (b2 & 0xC0) != 0x80) return false;

    out = ((uint32_t)(b0 & 0x0F) << 12) | ((uint32_t)(b1 & 0x3F) << 6) | (uint32_t)(b2 & 0x3F);
    return true;
  }

  if ((b0 & 0xF8) == 0xF0) {
    if (i + 2 >= len) return false;
    uint8_t b1 = (uint8_t)s[i++];
    uint8_t b2 = (uint8_t)s[i++];
    uint8_t b3 = (uint8_t)s[i++];
    if ((b1 & 0xC0) != 0x80 || (b2 & 0xC0) != 0x80 || (b3 & 0xC0) != 0x80) return false;

    out = ((uint32_t)(b0 & 0x07) << 18) |
          ((uint32_t)(b1 & 0x3F) << 12) |
          ((uint32_t)(b2 & 0x3F) << 6) |
          (uint32_t)(b3 & 0x3F);
    return true;
  }

  return false;
}
