#include "Effects/SparklesEffect.h"
#include "Display.h"

void SparklesEffect::begin(Display& display) {
  const uint8_t w = display.width();
  const uint8_t h = display.height();

  uint16_t want = (uint16_t)w * (uint16_t)h;
  if (want > MAX_PIXELS) want = MAX_PIXELS;
  count_ = want;

  for (uint16_t i = 0; i < count_; i++) {
    intensity_[i] = 0;
    hue_[i] = random8();
  }
}

void SparklesEffect::tick(Display& display, uint32_t dtMs) {
  if (count_ == 0) begin(display);

  uint32_t fade = (uint32_t)fadePerSecond_ * (uint32_t)dtMs / 1000UL;
  if (fade > 255) fade = 255;
  uint8_t fade8 = (uint8_t)fade;

  if (fade8 > 0) {
    for (uint16_t i = 0; i < count_; i++) {
      intensity_[i] = qsub8(intensity_[i], fade8);
    }
  }

  uint32_t spawn = (uint32_t)spawnPerSecond_ * (uint32_t)dtMs;
  uint16_t spawnCount = (uint16_t)(spawn / 1000UL);
  uint16_t rem = (uint16_t)(spawn % 1000UL);
  if (random16(1000) < rem) spawnCount++;

  for (uint16_t n = 0; n < spawnCount; n++) {
    uint16_t i = (count_ > 0) ? (uint16_t)random16(count_) : 0;
    intensity_[i] = 255;
    hue_[i] = random8();
  }

  const uint8_t w = display.width();
  const uint8_t h = display.height();

  display.clear(false);

  uint16_t i = 0;
  for (uint8_t y = 0; y < h; y++) {
    for (uint8_t x = 0; x < w; x++) {
      if (i >= count_) break;

      uint8_t v = intensity_[i];
      if (v != 0) {
        CRGB c;
        if (randomColors_) {
          c = CHSV(hue_[i], 255, v);
        } else {
          c = color_;
          c.nscale8_video(v);
        }
        display.setPixelXY(x, y, c);
      }

      i++;
    }
  }

  display.show();
}
