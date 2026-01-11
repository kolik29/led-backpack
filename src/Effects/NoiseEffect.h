#pragma once

#include "Effects/IEffect.h"
#include <FastLED.h>

class NoiseEffect : public IEffect {
public:
  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "noise"; }

  void setSpeed(uint16_t v) { speed_ = v; }
  void setScale(uint8_t v) { scale_ = v; }
  void setContrast(uint8_t v) { contrast_ = v; }

private:
  uint16_t z_ = 0;
  uint16_t xoff_ = 0;
  uint16_t yoff_ = 0;

  uint16_t speed_ = 45;
  uint8_t scale_ = 30;
  uint8_t contrast_ = 160;

  CRGBPalette16 palette_ = PartyColors_p;
};
