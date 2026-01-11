#include "Effects/NoiseEffect.h"
#include "Display.h"

static uint8_t applyContrast(uint8_t n, uint8_t contrast) {
  // contrast = 128 => без изменений
  // 120..180 обычно ок; 255 часто даёт “2 цвета”
  int16_t v = (int16_t)n - 128;
  v = (v * (int16_t)contrast) / 128;
  v += 128;
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  return (uint8_t)v;
}

void NoiseEffect::begin(Display& /*display*/) { }

void NoiseEffect::tick(Display& display, uint32_t dtMs) {
  z_    += (uint16_t)((uint32_t)speed_ * dtMs);
  hueZ_ += (uint16_t)((uint32_t)speed_ * dtMs / 2UL);
  xoff_ += (uint16_t)((uint32_t)speed_ * dtMs / 6UL);
  yoff_ += (uint16_t)((uint32_t)speed_ * dtMs / 8UL);

  const uint8_t w = display.width();
  const uint8_t h = display.height();

  display.clear(false);

  for (uint8_t y = 0; y < h; y++) {
    for (uint8_t x = 0; x < w; x++) {
      uint16_t nx = (uint16_t)x * (uint16_t)scale_ + xoff_;
      uint16_t ny = (uint16_t)y * (uint16_t)scale_ + yoff_;

      uint8_t hue = inoise8(nx, ny, hueZ_);
      uint8_t bri = inoise8(nx + 1337, ny + 7331, z_);

      bri = applyContrast(bri, contrast_);
      bri = qadd8(40, scale8(bri, 215));

      display.setPixelXY(x, y, CHSV(hue, 255, bri));
    }
  }

  display.show();
}
