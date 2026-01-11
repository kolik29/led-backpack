#pragma once

#include "Effects/IEffect.h"
#include <FastLED.h>

class MatrixRainEffect : public IEffect {
public:
  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "matrixrain"; }

  void setTrail(uint8_t v) { trail_ = v; }
  void setSpawnChance(uint8_t v) { spawnChance_ = v; }
  void setMinStepMs(uint16_t v) { minStepMs_ = (v == 0 ? 1 : v); }
  void setMaxStepMs(uint16_t v) { maxStepMs_ = (v == 0 ? 1 : v); }
  void setColor(const CRGB& c) { color_ = c; }

private:
  static constexpr uint8_t MAX_COLS = 32;

  uint8_t w_ = 0;
  uint8_t h_ = 0;

  int16_t headY_[MAX_COLS];
  uint16_t stepMs_[MAX_COLS];
  uint16_t accMs_[MAX_COLS];

  uint8_t trail_ = 8;
  uint8_t spawnChance_ = 60;
  uint16_t minStepMs_ = 45;
  uint16_t maxStepMs_ = 120;

  CRGB color_ = CRGB(0, 255, 0);

  void spawnCol_(uint8_t x);
};
