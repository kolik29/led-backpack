#include "Display.h"

Display::Display(uint8_t width, uint8_t height, bool serpentine)
  : width_(width), height_(height), serpentine_(serpentine) {}

void Display::begin() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds_, MAX_LEDS);
  FastLED.setBrightness(brightness_);
  clear(true);
}

void Display::setSize(uint8_t width, uint8_t height) {
  width_ = width;
  height_ = height;
  clear(true);
}

uint8_t Display::width() const { return width_; }
uint8_t Display::height() const { return height_; }

uint16_t Display::ledsCount() const {
  uint16_t n = (uint16_t)width_ * (uint16_t)height_;
  return (n <= MAX_LEDS) ? n : MAX_LEDS;
}

void Display::setBrightness(uint8_t brightness) {
  brightness_ = brightness;
  FastLED.setBrightness(brightness_);
}

uint8_t Display::getBrightness() const { return brightness_; }

void Display::clear(bool doShow) {
  fill_solid(leds_, MAX_LEDS, CRGB::Black);
  if (doShow) show();
}

void Display::show() {
  FastLED.show();
}

void Display::setPixelIndex(uint16_t i, const CRGB& c) {
  if (i >= ledsCount()) return;
  leds_[i] = c;
}

void Display::setPixelXY(uint8_t x, uint8_t y, const CRGB& c) {
  if (x >= width_ || y >= height_) return;
  uint16_t idx = xyToIndex_(x, y);
  if (idx >= ledsCount()) return;
  leds_[idx] = c;
}

uint16_t Display::xyToIndex_(uint8_t x, uint8_t y) const {
  if (!serpentine_) {
    return (uint16_t)y * (uint16_t)width_ + x;
  }

  if ((y % 2) == 0) {
    return (uint16_t)y * (uint16_t)width_ + x;
  }
  return (uint16_t)y * (uint16_t)width_ + (uint16_t)(width_ - 1 - x);
}
