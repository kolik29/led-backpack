#pragma once

#include "Effects/IEffect.h"
#include <FastLED.h>

class PongEffect : public IEffect {
public:
  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "pong"; }

  void setStepMs(uint16_t v) { stepMs_ = (v == 0 ? 1 : v); }
  void setPaddleSize(uint8_t v) { paddleSize_ = (v < 2 ? 2 : v); }
  void setBallColor(const CRGB& c) { ballColor_ = c; }
  void setPaddleColor(const CRGB& c) { paddleColor_ = c; }

private:
  uint8_t w_ = 0;
  uint8_t h_ = 0;

  // ball in "subpixels" (Q8.8 fixed)
  int16_t bx_ = 0;
  int16_t by_ = 0;
  int16_t vx_ = 0;
  int16_t vy_ = 0;

  // paddles center in Q8.8
  int16_t ly_ = 0;
  int16_t ry_ = 0;

  uint8_t paddleSize_ = 5; // height in pixels

  uint16_t stepMs_ = 45;
  uint32_t accMs_ = 0;

  CRGB ballColor_ = CRGB::White;
  CRGB paddleColor_ = CRGB::Green;

  void resetBall_(bool toRight);
  void step_();
  void botMove_();
  void draw_(Display& display);

  int16_t clampQ_(int16_t v, int16_t lo, int16_t hi) const {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
  }
};