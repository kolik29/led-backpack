#include "Effects/BikeEffect.h"
#include "Display.h"

void BikeEffect::begin(Display& /*display*/) {
  tMs_ = 0;
  brakeHoldLeftMs_ = 0;
}

void BikeEffect::fillAll_(Display& display, const CRGB& c) {
  const uint8_t w = display.width();
  const uint8_t h = display.height();

  display.clear(false);
  for (uint8_t y = 0; y < h; y++) {
    for (uint8_t x = 0; x < w; x++) {
      display.setPixelXY(x, y, c);
    }
  }
  display.show();
}

bool BikeEffect::isOn_() const {
  if (mode_ == Mode::Solid) return true;

  if (mode_ == Mode::Blink) {
    uint32_t m = tMs_ % BLINK_PERIOD_MS;
    return m < BLINK_ON_MS;
  }

  // Mode::Double
  uint32_t m = tMs_ % DOUBLE_PERIOD_MS;

  uint32_t a = DOUBLE_ON_MS;
  uint32_t b = a + DOUBLE_GAP_MS;
  uint32_t c = b + DOUBLE_ON_MS;

  if (m < a) return true;
  if (m < b) return false;
  if (m < c) return true;
  return false;
}

bool BikeEffect::brakeActive_(uint32_t dtMs) {
  // todo: for accel add updateBrakeFromSensor
  // accelTick(dtMs);
  // bool brakingNow = accelBraking();

  bool brakingNow = false; // todo
  (void)dtMs;

  // ручной тест из API имеет приоритет
  if (brakeTest_) {
    brakeHoldLeftMs_ = BRAKE_HOLD_MS;
    return true;
  }

  // todo: accelBraking
  if (brakingNow) {
    brakeHoldLeftMs_ = BRAKE_HOLD_MS;
    return true;
  }

  if (brakeHoldLeftMs_ > 0) {
    uint16_t dec = (dtMs > 1000) ? 1000 : (uint16_t)dtMs;
    if (brakeHoldLeftMs_ > dec) brakeHoldLeftMs_ -= dec;
    else brakeHoldLeftMs_ = 0;
    return (brakeHoldLeftMs_ > 0);
  }

  return false;
}

void BikeEffect::tick(Display& display, uint32_t dtMs) {
  tMs_ += dtMs;

  if (brakeActive_(dtMs)) {
    CRGB c = CRGB::Red;
    c.nscale8_video(BRAKE_BRIGHTNESS);
    fillAll_(display, c);
    return;
  }

  if (isOn_()) {
    CRGB c = CRGB::Red;
    c.nscale8_video(BASE_BRIGHTNESS);
    fillAll_(display, c);
  } else {
    fillAll_(display, CRGB::Black);
  }
}