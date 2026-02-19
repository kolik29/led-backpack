#pragma once

#include <Arduino.h>
#include <FastLED.h>

class Display {
public:
  static constexpr uint16_t MAX_LEDS = 256;
  static constexpr uint8_t DATA_PIN = 18;

  Display(uint8_t width, uint8_t height, bool serpentine = true);

  void begin();

  void setSize(uint8_t width, uint8_t height);
  uint8_t width() const;
  uint8_t height() const;
  uint16_t ledsCount() const;

  void setBrightness(uint8_t brightness);
  uint8_t getBrightness() const;

  void clear(bool doShow = true);
  void show();

  void setPixelIndex(uint16_t i, const CRGB& c);
  void setPixelXY(uint8_t x, uint8_t y, const CRGB& c);

  void setFlip(bool flipX, bool flipY) { flipX_ = flipX; flipY_ = flipY; }
  bool flipX() const { return flipX_; }
  bool flipY() const { return flipY_; }

private:
  uint16_t xyToIndex_(uint8_t x, uint8_t y) const;

  uint8_t width_ = 8;
  uint8_t height_ = 8;
  bool serpentine_ = true;

  uint8_t brightness_ = 255;
  CRGB leds_[MAX_LEDS];

  bool flipX_ = false;
  bool flipY_ = true;
};
