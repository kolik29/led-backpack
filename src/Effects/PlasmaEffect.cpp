#include "Effects/PlasmaEffect.h"
#include "Display.h"

void PlasmaEffect::begin(Display& /*display*/) {
}

void PlasmaEffect::tick(Display& display, uint32_t dtMs) {
  t_ += (uint16_t)((uint32_t)speed_ * dtMs);

  const uint8_t w = display.width();
  const uint8_t h = display.height();

  display.clear(false);

  for (uint8_t y = 0; y < h; y++) {
    for (uint8_t x = 0; x < w; x++) {
      uint8_t a = sin8((uint8_t)(x * scale_) + (uint8_t)(t_ >> 2));
      uint8_t b = cos8((uint8_t)(y * scale_) + (uint8_t)(t_ >> 3));
      uint8_t c = sin8((uint8_t)((x + y) * (scale_ >> 1)) + (uint8_t)(t_ >> 4));

      uint8_t v = (uint8_t)((uint16_t)a + (uint16_t)b + (uint16_t)c) / 3;
      CRGB col = ColorFromPalette(palette_, v, 255, LINEARBLEND);

      display.setPixelXY(x, y, col);
    }
  }

  display.show();
}
