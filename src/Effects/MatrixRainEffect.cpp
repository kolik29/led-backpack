#include "Effects/MatrixRainEffect.h"
#include "Display.h"

void MatrixRainEffect::spawnCol_(uint8_t x) {
  headY_[x] = (int16_t)(-(int16_t)random8(0, 4));
  uint16_t a = minStepMs_;
  uint16_t b = maxStepMs_;
  if (b < a) {
    uint16_t t = a; a = b; b = t;
  }
  stepMs_[x] = (uint16_t)random16(a, b + 1);
  accMs_[x] = 0;
}

void MatrixRainEffect::begin(Display& display) {
  w_ = display.width();
  h_ = display.height();
  if (w_ > MAX_COLS) w_ = MAX_COLS;

  for (uint8_t x = 0; x < MAX_COLS; x++) {
    headY_[x] = -1;
    stepMs_[x] = minStepMs_;
    accMs_[x] = 0;
  }

  for (uint8_t x = 0; x < w_; x++) {
    if (random8() < (uint8_t)(spawnChance_ / 2)) {
      spawnCol_(x);
    }
  }
}

void MatrixRainEffect::tick(Display& display, uint32_t dtMs) {
  if (w_ == 0 || h_ == 0) begin(display);

  for (uint8_t x = 0; x < w_; x++) {
    if (headY_[x] < 0) {
      if (random8() < spawnChance_) {
        spawnCol_(x);
      }
      continue;
    }

    accMs_[x] = (uint16_t)(accMs_[x] + dtMs);
    while (accMs_[x] >= stepMs_[x]) {
      accMs_[x] = (uint16_t)(accMs_[x] - stepMs_[x]);
      headY_[x]++;

      if (headY_[x] > (int16_t)h_ + (int16_t)trail_) {
        headY_[x] = -1;
        break;
      }
    }
  }

  display.clear(false);

  for (uint8_t x = 0; x < w_; x++) {
    int16_t head = headY_[x];
    if (head < -10) continue;

    for (uint8_t t = 0; t < trail_; t++) {
      int16_t yy = head - (int16_t)t;
      if (yy < 0 || yy >= (int16_t)h_) continue;

      uint8_t v = 255;
      if (trail_ > 1) {
        v = (uint8_t)(255 - (uint16_t)t * 255 / (uint16_t)trail_);
      }

      CRGB c = color_;
      c.nscale8_video(v);

      display.setPixelXY(x, (uint8_t)yy, c);
    }

    if (head >= 0 && head < (int16_t)h_) {
      display.setPixelXY(x, (uint8_t)head, color_);
    }
  }

  display.show();
}
