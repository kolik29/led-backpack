#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "Display.h"

static constexpr uint8_t FONT_W = 6;
static constexpr uint8_t FONT_H = 10;

class TextMode {
public:
  void setText(const String& text);
  void setColor(const CRGB& color);
  void setSpeedPxPerSec(uint16_t speed);
  void setScroll(bool scroll);
  void setSpacing(uint8_t spacing);
  void setY(int8_t y); // -1 = автоцентр

  void reset();
  void tick(Display& display);

  const String& text() const;

private:
  static constexpr size_t MAX_COLS = 1024;

  String text_;
  CRGB color_ = CRGB::Red;

  uint16_t speedPxPerSec_ = 20;
  bool scroll_ = true;
  uint8_t spacing_ = 1;
  int8_t y_ = -1;

  uint32_t lastStepMs_ = 0;
  uint16_t offset_ = 0;

  uint16_t cols_[MAX_COLS];
  uint16_t colsLen_ = 0;

  void rebuildColumns_(uint8_t displayWidth);
  int8_t resolveY_(uint8_t displayHeight) const;
};
