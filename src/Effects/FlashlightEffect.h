#pragma once
#include "Effects/IEffect.h"
#include <FastLED.h>

class FlashlightEffect : public IEffect {
public:
  void setColor(const CRGB& c) { color_ = c; }
  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "flashlight"; }

private:
  CRGB color_ = CRGB::White;

  void render_(Display& display);
};
