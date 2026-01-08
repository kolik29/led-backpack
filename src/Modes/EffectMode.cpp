#include "Modes/EffectMode.h"

IEffect* EffectMode::byId_(Id id) {
  switch (id) {
    case Id::Flashlight: return &flashlight_;
    case Id::ColorShift: return &colorShift_;
    case Id::Fire: return &fire_;
  }
  return &flashlight_;
}

void EffectMode::setEffect(Id id, Display& display) {
  currentId_ = id;
  current_ = byId_(id);
  lastMs_ = millis();
  current_->begin(display);
}

void EffectMode::tick(Display& display) {
  if (!current_) {
    setEffect(currentId_, display);
  }

  uint32_t now = millis();
  uint32_t dt = now - lastMs_;
  lastMs_ = now;

  if (dt > 200) dt = 200;

  current_->tick(display, dt);
}

bool EffectMode::setEffectByName(const String& name, Display& display) {
  if (name == "flashlight") {
    setEffect(Id::Flashlight, display);
    return true;
  }
  if (name == "colorshift") {
    setEffect(Id::ColorShift, display);
    return true;
  }
  if (name == "fire") {
    setEffect(Id::Fire, display);
    return true;
  }
  return false;
}