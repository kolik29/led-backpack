#include "Effects/ColorShiftEffect.h"
#include "Display.h"

CRGB ColorShiftEffect::randomVividColor_() {
  return CHSV(random8(), 255, 255);
}

void ColorShiftEffect::begin(Display& display) {
  w_ = display.width();
  h_ = display.height();

  count_ = (uint16_t)w_ * (uint16_t)h_;
  if (count_ > MAX_PIXELS) count_ = MAX_PIXELS;

  for (uint16_t i = 0; i < count_; i++) {
    cur_[i] = randomVividColor_();
    target_[i] = randomVividColor_();
  }

  curColor_ = randomVividColor_();
  targetColor_ = randomVividColor_();

  accMs_ = 0;
  inited_ = true;
}

void ColorShiftEffect::setAllTargets_() {
  if (updateCount_ == 0 || updateCount_ >= count_) {
    for (uint16_t i = 0; i < count_; i++) {
      target_[i] = randomVividColor_();
    }
    return;
  }

  uint16_t k = updateCount_;
  if (k > count_) k = count_;

  for (uint16_t n = 0; n < k; n++) {
    uint16_t i = (uint16_t)random16(0, count_);
    target_[i] = randomVividColor_();
  }
}

void ColorShiftEffect::renderPerPixel_(Display& display) {
  display.clear(false);

  for (uint8_t y = 0; y < h_; y++) {
    for (uint8_t x = 0; x < w_; x++) {
      uint16_t i = (uint16_t)y * (uint16_t)w_ + x;
      if (i >= count_) break;
      display.setPixelXY(x, y, cur_[i]);
    }
  }

  display.show();
}

void ColorShiftEffect::renderGlobal_(Display& display) {
  display.clear(false);

  for (uint8_t y = 0; y < display.height(); y++) {
    for (uint8_t x = 0; x < display.width(); x++) {
      display.setPixelXY(x, y, curColor_);
    }
  }

  display.show();
}

void ColorShiftEffect::tick(Display& display, uint32_t dtMs) {
  if (!inited_ || display.width() != w_ || display.height() != h_) {
    begin(display);
  }

  accMs_ += dtMs;

  if (accMs_ >= changeEveryMs_) {
    accMs_ = 0;

    if (style_ == Style::Global) {
      targetColor_ = randomVividColor_();
    } else {
      setAllTargets_();
    }
  }

  uint32_t amt = (uint32_t)dtMs * (uint32_t)blendPerSecond_ / 1000UL;
  if (amt < 1) amt = 1;
  if (amt > 255) amt = 255;

  const uint8_t blendAmt = (uint8_t)amt;

  if (style_ == Style::Global) {
    nblend(curColor_, targetColor_, blendAmt);
    renderGlobal_(display);
    return;
  }

  for (uint16_t i = 0; i < count_; i++) {
    nblend(cur_[i], target_[i], blendAmt);
  }

  renderPerPixel_(display);
}
