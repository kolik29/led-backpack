#include "Effects/FireEffect.h"

void FireEffect::begin(Display& display) {
  w_ = display.width();
  h_ = display.height();

  if (w_ == 0 || h_ == 0) {
    w_ = 0;
    h_ = 0;
    n_ = 0;
    return;
  }

  uint32_t want = (uint32_t)w_ * (uint32_t)h_;
  if (want > MAX_PIXELS) want = MAX_PIXELS;
  n_ = (uint16_t)want;

  if (flareRows_ > h_) flareRows_ = h_;

  resetState_();
}

void FireEffect::resetState_() {
  for (uint16_t i = 0; i < n_; i++) {
    heat_[i] = 0;
  }

  for (uint8_t i = 0; i < MAX_FLARES; i++) {
    flares_[i].active = false;
    flares_[i].x = 0;
    flares_[i].y = 0;
    flares_[i].vx = 0;
    flares_[i].heat = 0;
  }

  accMs_ = 0;
}

void FireEffect::tick(Display& display, uint32_t dtMs) {
  if (display.width() != w_ || display.height() != h_) {
    begin(display);
  }

  if (n_ == 0) return;

  accMs_ += dtMs;
  while (accMs_ >= stepMs_) {
    accMs_ -= stepMs_;
    step_();
  }

  display.clear(false);

  for (uint8_t y = 0; y < h_; y++) {
    for (uint8_t x = 0; x < w_; x++) {
      uint8_t v = heat_[idx_(x, y)];

      CRGB c = ColorFromPalette(palette_, v, 255, LINEARBLEND);

      uint8_t dy = flipY_ ? (uint8_t)((h_ - 1) - y) : y;
      display.setPixelXY(x, dy, c);
    }
  }

  display.show();
}

void FireEffect::step_() {
  coolDown_();
  diffuseUp_();
  igniteSparks_();
  updateFlares_();
}

void FireEffect::coolDown_() {
  for (uint8_t y = 0; y < h_; y++) {
    uint8_t coolExtra = (uint8_t)((uint16_t)cooling_ * (uint16_t)y / (h_ ? h_ : 1));

    for (uint8_t x = 0; x < w_; x++) {
      uint16_t i = idx_(x, y);

      uint8_t cool = random8(0, (uint8_t)(((uint16_t)cooling_ * 10U) / (h_ ? h_ : 1) + 2U));
      cool = qadd8(cool, coolExtra);

      heat_[i] = qsub8(heat_[i], cool);
    }
  }
}

void FireEffect::diffuseUp_() {
  for (uint8_t x = 0; x < w_; x++) {
    for (int y = (int)h_ - 1; y >= 1; y--) {
      uint16_t i = idx_(x, (uint8_t)y);
      uint16_t below = idx_(x, (uint8_t)(y - 1));

      uint8_t a = heat_[i];
      uint8_t b = heat_[below];

      heat_[i] = (uint8_t)((a + b + b) / 3);
    }
  }
}

void FireEffect::igniteSparks_() {
  if (random8() < sparking_) {
    uint8_t x = random8(w_);
    uint8_t y = (h_ > 2) ? random8(2) : 0;
    uint16_t i = idx_(x, y);

    heat_[i] = qadd8(heat_[i], random8(160, 255));
  }
}

uint8_t FireEffect::activeFlares_() const {
  uint8_t n = 0;
  for (uint8_t i = 0; i < MAX_FLARES; i++) {
    if (flares_[i].active) n++;
  }
  return n;
}

void FireEffect::spawnFlare_() {
  for (uint8_t i = 0; i < MAX_FLARES; i++) {
    if (!flares_[i].active) {
      flares_[i].active = true;
      flares_[i].x = (int8_t)random8(w_);
      flares_[i].y = 0;
      flares_[i].vx = (int8_t)random8(3) - 1;
      flares_[i].heat = random8(180, 255);
      return;
    }
  }
}

void FireEffect::addHeat_(int8_t x, int8_t y, uint8_t v) {
  if (!inBounds_(x, y)) return;
  uint16_t i = idx_((uint8_t)x, (uint8_t)y);
  heat_[i] = qadd8(heat_[i], v);
}

void FireEffect::updateFlares_() {
  if (maxFlares_ > MAX_FLARES) maxFlares_ = MAX_FLARES;

  if (maxFlares_ > 0) {
    uint8_t act = activeFlares_();
    if (act < maxFlares_) {
      if (random8(100) < flareChance_) {
        spawnFlare_();
      }
    }
  }

  for (uint8_t i = 0; i < MAX_FLARES; i++) {
    Flare& f = flares_[i];
    if (!f.active) continue;

    uint8_t h0 = f.heat;
    addHeat_(f.x,     f.y,     h0);
    addHeat_(f.x - 1, f.y,     h0 / 2);
    addHeat_(f.x + 1, f.y,     h0 / 2);
    addHeat_(f.x,     f.y + 1, h0 / 2);
    addHeat_(f.x,     f.y - 1, h0 / 4);
    addHeat_(f.x - 1, f.y + 1, h0 / 3);
    addHeat_(f.x + 1, f.y + 1, h0 / 3);

    f.heat = qsub8(f.heat, flareDecay_);

    f.y++;

    if (random8() < 90) {
      f.vx = (int8_t)random8(3) - 1;
    }
    if (random8() < 160) {
      f.x = (int8_t)(f.x + f.vx);
    }

    if (f.x < 0) f.x = 0;
    if (f.x >= (int8_t)w_) f.x = (int8_t)w_ - 1;

    uint8_t limitY = flareRows_;
    if (limitY == 0) limitY = 1;
    if (f.y >= (int8_t)limitY || f.y >= (int8_t)h_ || f.heat < 10) {
      f.active = false;
    }
  }
}
