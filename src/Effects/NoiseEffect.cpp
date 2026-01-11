#include "Effects/NoiseEffect.h"
#include "Display.h"

void NoiseEffect::begin(Display& /*display*/) {
}

void NoiseEffect::tick(Display& display, uint32_t dtMs) {
  z_    += (uint16_t)((uint32_t)speed_ * dtMs);
  xoff_ += (uint16_t)((uint32_t)speed_ * dtMs / 6UL);
  yoff_ += (uint16_t)((uint32_t)speed_ * dtMs / 8UL);

  const uint8_t w = display.width();
  const uint8_t h = display.height();

  display.clear(false);

  for (uint8_t y = 0; y < h; y++) {
    for (uint8_t x = 0; x < w; x++) {
      uint16_t nx = (uint16_t)x * (uint16_t)scale_ + xoff_;
      uint16_t ny = (uint16_t)y * (uint16_t)scale_ + yoff_;

      uint8_t n = inoise8(nx, ny, z_);

      int16_t v = (int16_t)n - 128;
      v = (v * (int16_t)contrast_) / 128;
      v += 128;
      if (v < 0) v = 0;
      if (v > 255) v = 255;

      CRGB col = ColorFromPalette(palette_, (uint8_t)v, 255, LINEARBLEND);
      display.setPixelXY(x, y, col);
    }
  }

  display.show();
}
