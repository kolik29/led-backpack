#include "Modes/TextMode.h"
#include "Utils/Font.h"
#include "Utils/Utf8.h"

void TextMode::setText(const String& text) {
  text_ = text;
  colsLen_ = 0;
  offset_ = 0;
  lastStepMs_ = 0;
}

void TextMode::setColor(const CRGB& color) {
  color_ = color;
}

void TextMode::setSpeedPxPerSec(uint16_t speed) {
  speedPxPerSec_ = (speed == 0) ? 1 : speed;
}

void TextMode::setScroll(bool scroll) {
  scroll_ = scroll;
}

void TextMode::setSpacing(uint8_t spacing) {
  spacing_ = spacing;
}

void TextMode::setY(int8_t y) {
  y_ = y;
}

void TextMode::reset() {
  colsLen_ = 0;
  offset_ = 0;
  lastStepMs_ = 0;
}

const String& TextMode::text() const {
  return text_;
}

int8_t TextMode::resolveY_(uint8_t displayHeight) const {
  if (y_ >= 0) return y_;

  int8_t yy = ((int)displayHeight - (int)Font::H) / 2;
  if (yy < 0) yy = 0;
  return yy;
}

void TextMode::rebuildColumns_(uint8_t displayWidth) {
  colsLen_ = 0;

  const char* s = text_.c_str();
  size_t len = text_.length();
  size_t i = 0;

  while (i < len && (colsLen_ + Font::W + spacing_) < MAX_COLS) {
    uint32_t cp = 0;
    if (!Utf8::nextCodepoint(s, len, i, cp)) {
      break;
    }

    uint16_t glyph[Font::W];
    Font::getGlyph(cp, glyph);

    for (uint8_t c = 0; c < Font::W; c++) {
      if (colsLen_ < MAX_COLS) {
        cols_[colsLen_++] = glyph[c];
      }
    }

    for (uint8_t sp = 0; sp < spacing_; sp++) {
      if (colsLen_ < MAX_COLS) {
        cols_[colsLen_++] = 0x0000;
      }
    }
  }

  for (uint8_t k = 0; k < displayWidth; k++) {
    if (colsLen_ < MAX_COLS) {
      cols_[colsLen_++] = 0x0000;
    }
  }

  if (colsLen_ == 0) {
    cols_[0] = 0x0000;
    colsLen_ = 1;
  }

  offset_ = 0;
}

void TextMode::tick(Display& display) {
  uint8_t w = display.width();
  uint8_t h = display.height();

  if (colsLen_ == 0) {
    rebuildColumns_(w);
  }

  if (scroll_) {
    uint32_t now = millis();
    uint32_t stepMs = 1000UL / (uint32_t)speedPxPerSec_;
    if (stepMs == 0) stepMs = 1;

    if (lastStepMs_ == 0) lastStepMs_ = now;

    if (now - lastStepMs_ >= stepMs) {
      lastStepMs_ = now;
      offset_++;

      if (offset_ >= colsLen_) {
        offset_ = 0;
      }
    }
  } else {
    offset_ = 0;
  }

  int8_t y0 = resolveY_(h);

  display.clear(false);

  for (uint8_t x = 0; x < w; x++) {
    uint16_t idx = (uint16_t)(offset_ + x);
    if (idx >= colsLen_) idx %= colsLen_;

    uint16_t col = cols_[idx];

    for (uint8_t y = 0; y < Font::H; y++) {
      if ((col >> y) & 0x01) {
        int16_t yy = (int16_t)y0 + (int16_t)y;
        if (yy >= 0 && yy < h) {
          display.setPixelXY(x, (uint8_t)yy, color_);
        }
      }
    }
  }

  display.show();
}
