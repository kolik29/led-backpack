#pragma once

#include "Effects/IEffect.h"
#include <FastLED.h>

class ColorShiftEffect : public IEffect {
public:
  enum class Style : uint8_t {
    PerPixel = 0,
    Global   = 1,
  };

  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "colorshift"; }

  void setChangeEveryMs(uint32_t ms) { changeEveryMs_ = (ms == 0 ? 1 : ms); }
  void setBlendPerSecond(uint8_t v) { blendPerSecond_ = (v == 0 ? 1 : v); }

  void setUpdateCount(uint16_t v) { updateCount_ = v; }

  void setStyle(Style s) { style_ = s; }

private:
  static constexpr uint16_t MAX_PIXELS = 16 * 16;

  CRGB cur_[MAX_PIXELS];
  CRGB target_[MAX_PIXELS];

  CRGB curColor_ = CRGB::Black;
  CRGB targetColor_ = CRGB::Black;

  uint8_t  w_ = 0;
  uint8_t  h_ = 0;
  uint16_t count_ = 0;

  bool inited_ = false;

  uint32_t accMs_ = 0;
  uint32_t changeEveryMs_ = 900;
  uint8_t  blendPerSecond_ = 120;

  uint16_t updateCount_ = 0;

  Style style_ = Style::PerPixel;

  CRGB randomVividColor_();
  void setAllTargets_();
  void renderPerPixel_(Display& display);
  void renderGlobal_(Display& display);
};
