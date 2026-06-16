#pragma once

#include <Arduino.h>
#include "Display.h"
#include "Modes/TextMode.h"
#include "Modes/EffectMode.h"

class RenderMgr {
public:
  enum class Mode : uint8_t {
    Frame = 0,
    Text = 1,
    Effect = 2
  };

  explicit RenderMgr(Display& display);

  void setMode(Mode mode);
  Mode mode() const;

  TextMode& text();
  EffectMode& effects();
  void setEffect(EffectMode::Id id);

  void tick();

private:
  static constexpr uint32_t FRAME_INTERVAL_MS = 80;

  Display& display_;
  Mode mode_ = Mode::Frame;
  TextMode text_;
  EffectMode effects_;
  uint32_t lastFrameMs_ = 0;
};
