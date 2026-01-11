#pragma once

#include "Effects/IEffect.h"
#include <FastLED.h>

class PlasmaEffect : public IEffect {
public:
  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "plasma"; }

  void setSpeed(uint8_t v) { speed_ = v; }
  void setScale(uint8_t v) { scale_ = v; }

private:
  uint16_t t_ = 0;
  uint8_t speed_ = 10;
  uint8_t scale_ = 28;

  CRGBPalette16 palette_ = RainbowColors_p;
};
