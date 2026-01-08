#include "RenderMgr.h"

RenderMgr::RenderMgr(Display& display) : display_(display) {}

void RenderMgr::setMode(Mode mode) {
  mode_ = mode;

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
  mode_ = Mode::Effect;
  effects_.setEffect(id, display_);
}

void RenderMgr::tick() {
  if (mode_ == Mode::Text) {
    text_.tick(display_);
    return;
  }

  if (mode_ == Mode::Effect) {
    effects_.tick(display_);
    return;
  }
}
