#pragma once

#include "Effects/IEffect.h"
#include <FastLED.h>

class SnakeEffect : public IEffect {
public:
  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "snake"; }

  void setStepMs(uint16_t v) { stepMs_ = (v == 0 ? 1 : v); }
  void setLength(uint16_t v) { fixedLen_ = v; } // фиксированная длина
  void setSnakeColor(const CRGB& c) { snakeColor_ = c; }
  void setFoodColor(const CRGB& c) { foodColor_ = c; }

private:
  static constexpr uint16_t MAX_CELLS = 16 * 16;

  struct Pt { uint8_t x; uint8_t y; };

  uint8_t  w_ = 0;
  uint8_t  h_ = 0;

  Pt snake_[MAX_CELLS];
  uint16_t len_ = 0;
  uint16_t fixedLen_ = 14; // можно менять параметром

  Pt food_{0,0};
  bool hasFood_ = false;

  uint16_t stepMs_ = 140;
  uint32_t accMs_ = 0;

  CRGB snakeColor_ = CRGB::Green;
  CRGB foodColor_  = CRGB::Red;

  // utils
  uint16_t idx_(uint8_t x, uint8_t y) const { return (uint16_t)y * w_ + x; }
  Pt ptFromIdx_(uint16_t i) const { return Pt{ (uint8_t)(i % w_), (uint8_t)(i / w_) }; }

  bool isOnSnake_(Pt p) const;
  void spawnFood_();
  void step_(Display& display);

  // pathfinding
  bool findNextStepBfs_(Pt start, Pt goal, Pt& outNext);
  bool findNextStepToTail_(Pt head, Pt& outNext);
  bool pickAnySafeNeighbor_(Pt head, Pt& outNext);
};
