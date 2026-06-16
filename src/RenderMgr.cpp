#include "RenderMgr.h"

RenderMgr::RenderMgr(Display& display) : display_(display) {}

void RenderMgr::setMode(Mode mode) {
  Mode prev = mode_;
  if (prev == Mode::Effect && mode != Mode::Effect) {
    effects_.end(display_);
  }

  mode_ = mode;
  lastFrameMs_ = 0;

  if (mode_ == Mode::Text) {
    text_.reset();
  }

  if (mode_ == Mode::Effect) {
    effects_.setEffect(effects_.effect(), display_);
  }
}

RenderMgr::Mode RenderMgr::mode() const {
  return mode_;
}

TextMode& RenderMgr::text() {
  return text_;
}

EffectMode& RenderMgr::effects() {
  return effects_;
}

void RenderMgr::setEffect(EffectMode::Id id) {
  lastFrameMs_ = 0;
  mode_ = Mode::Effect;
  effects_.setEffect(id, display_);
}

void RenderMgr::tick() {
  if (mode_ == Mode::Frame) {
    return;
  }

  uint32_t now = millis();
  if (lastFrameMs_ != 0 && now - lastFrameMs_ < FRAME_INTERVAL_MS) {
    return;
  }
  lastFrameMs_ = now;

  if (mode_ == Mode::Text) {
    text_.tick(display_);
    return;
  }

  if (mode_ == Mode::Effect) {
    effects_.tick(display_);
    return;
  }
}
