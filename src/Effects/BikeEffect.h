#pragma once

#include "Effects/IEffect.h"
#include <FastLED.h>

class BikeEffect : public IEffect {
public:
  enum class Mode : uint8_t {
    Solid = 0,
    Blink = 1,
    Double = 2,
  };

  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "bike"; }

  void setMode(Mode m) { mode_ = m; }
  Mode mode() const { return mode_; }

  void setBrakeTest(bool on) { brakeTest_ = on; }

  void accelInit() {} // todo: init accel
  void accelTick(uint32_t /*dtMs*/) {} // todo: read accel
  bool accelBraking() const { return false; } // todo

private:
  Mode mode_ = Mode::Blink;

  uint32_t tMs_ = 0;

  bool brakeTest_ = false;

  static constexpr uint8_t  BASE_BRIGHTNESS = 120;
  static constexpr uint8_t  BRAKE_BRIGHTNESS = 255;
  static constexpr uint16_t BLINK_PERIOD_MS = 900;
  static constexpr uint16_t BLINK_ON_MS     = 140;

  static constexpr uint16_t DOUBLE_PERIOD_MS = 900;
  static constexpr uint16_t DOUBLE_ON_MS     = 120;
  static constexpr uint16_t DOUBLE_GAP_MS    = 120;

  // todo: accel anti-jitter
  static constexpr uint16_t BRAKE_HOLD_MS = 900;
  uint16_t brakeHoldLeftMs_ = 0;

  bool brakeActive_(uint32_t dtMs);
  bool isOn_() const;

  void fillAll_(Display& display, const CRGB& c);
};