#include "Effects/NoiseEffect.h"
#include "Display.h"

static uint8_t applyContrast(uint8_t n, uint8_t contrast) {
  int16_t v = (int16_t)n - 128;
  v = (v * (int16_t)contrast) / 128;
  v += 128;
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  return (uint8_t)v;
}

static uint16_t speedCurve(uint16_t s) {
  static constexpr uint16_t kMax = 12;    // 8..16
  static constexpr uint16_t kCurve = 30;  // 12..30
  return (uint16_t)((uint32_t)kMax * s / (s + kCurve));
}

static uint16_t smoothU16(uint16_t cur, uint16_t target, uint32_t dtMs) {
  static constexpr uint32_t kTauMs = 400; // 150..400
  if (dtMs >= kTauMs) return target;
  uint32_t a = (dtMs * 255UL) / kTauMs;
  int32_t diff = (int32_t)target - (int32_t)cur;
  return (uint16_t)((int32_t)cur + (diff * (int32_t)a) / 255);
}

static void advance16(uint16_t &v, uint32_t &accQ8, uint32_t stepQ8) {
  accQ8 += stepQ8;
  v += (uint16_t)(accQ8 >> 8);
  accQ8 &= 0xFF;
}

void NoiseEffect::begin(Display& /*display*/) { }

void NoiseEffect::tick(Display& display, uint32_t dtMs) {
  if (dtMs > 50) dtMs = 50;

  speedSmoothed_ = smoothU16(speedSmoothed_, speedTarget_, dtMs);

  const uint16_t eff = speedCurve(speedSmoothed_);

  const uint32_t baseQ8 = (uint32_t)eff * (uint32_t)dtMs * 256UL;

  advance16(z_,    zAcc_,   baseQ8);
  advance16(hueZ_, hueAcc_, baseQ8 / 2UL);
  advance16(xoff_, xAcc_,   baseQ8 / 6UL);
  advance16(yoff_, yAcc_,   baseQ8 / 8UL);

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
