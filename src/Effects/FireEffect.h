#pragma once

#include "Effects/IEffect.h"
#include "Display.h"
#include <FastLED.h>

class FireEffect : public IEffect {
public:
  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "fire"; }

  void setCooling(uint8_t v) { cooling_ = v; }
  void setSparking(uint8_t v) { sparking_ = v; }
  void setStepMs(uint16_t v) { stepMs_ = (v == 0 ? 1 : v); }

  void setMaxFlares(uint8_t v) { maxFlares_ = v; }
  void setFlareChance(uint8_t v) { flareChance_ = v; }
  void setFlareRows(uint8_t v) { flareRows_ = v; }
  void setFlareDecay(uint8_t v) { flareDecay_ = (v == 0 ? 1 : v); }
  void setFlipY(bool v) { flipY_ = v; }

private:
  struct Flare {
    bool active = false;
    int8_t x = 0;
    int8_t y = 0;
    int8_t vx = 0;
    uint8_t heat = 0;
  };

  static constexpr uint16_t MAX_PIXELS = Display::MAX_LEDS;
  static constexpr uint8_t  MAX_FLARES = 8;

  uint8_t  w_ = 0;
  uint8_t  h_ = 0;
  uint16_t n_ = 0;

  uint8_t heat_[MAX_PIXELS];

  Flare flares_[MAX_FLARES];

  bool flipY_ = true;

  uint8_t  cooling_ = 55;
  uint8_t  sparking_ = 120;

  uint8_t  maxFlares_ = 3;
  uint8_t  flareChance_ = 30;   // %
  uint8_t  flareRows_ = 7;
  uint8_t  flareDecay_ = 14;

  uint16_t stepMs_ = 30;
  uint32_t accMs_ = 0;

  CRGBPalette16 palette_ = HeatColors_p;

  void resetState_();
  void step_();

  void coolDown_();
  void diffuseUp_();
  void igniteSparks_();
  void updateFlares_();

  void spawnFlare_();
  uint8_t activeFlares_() const;

  void addHeat_(int8_t x, int8_t y, uint8_t v);

  bool inBounds_(int8_t x, int8_t y) const {
    return (x >= 0 && y >= 0 && x < (int8_t)w_ && y < (int8_t)h_);
  }

  uint16_t idx_(uint8_t x, uint8_t y) const {
    return (uint16_t)y * (uint16_t)w_ + (uint16_t)x;
  }
};
