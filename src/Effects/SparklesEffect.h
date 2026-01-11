#pragma once

#include "Effects/IEffect.h"
#include <FastLED.h>

class SparklesEffect : public IEffect {
public:
  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "sparkles"; }

  void setSpawnPerSecond(uint16_t v) { spawnPerSecond_ = v; }
  void setFadePerSecond(uint16_t v) { fadePerSecond_ = v; }
  void setRandomColors(bool v) { randomColors_ = v; }
  void setColor(const CRGB& c) { color_ = c; }

private:
  static constexpr uint16_t MAX_PIXELS = 16 * 16;

  uint16_t count_ = 0;

  uint8_t intensity_[MAX_PIXELS];
  uint8_t hue_[MAX_PIXELS];

  uint16_t spawnPerSecond_ = 40;
  uint16_t fadePerSecond_  = 220;
  bool randomColors_ = true;
  CRGB color_ = CRGB::White;

  uint16_t idx_(uint8_t x, uint8_t y, uint8_t w) const { return (uint16_t)y * (uint16_t)w + x; }
};
