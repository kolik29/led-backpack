#pragma once

#include <Arduino.h>

#include "Display.h"
#include "Effects/IEffect.h"
#include "Effects/FlashlightEffect.h"
#include "Effects/ColorShiftEffect.h"
#include "Effects/FireEffect.h"

class EffectMode {
public:
  enum class Id : uint8_t {
    Flashlight,
    ColorShift,
    Fire,
  };
  
  void setEffect(Id id, Display& display);
  Id effect() const { return currentId_; }

  void tick(Display& display);

  bool setEffectByName(const String& name, Display& display);

  const char* effectName() const { return current_ ? current_->name() : "none"; }

  FireEffect& fire() { return fire_; }
  ColorShiftEffect& colorShift() { return colorShift_; }
  FlashlightEffect& flashlight() { return flashlight_; }

private:
  Id currentId_ = Id::Flashlight;
  IEffect* current_ = nullptr;

  uint32_t lastMs_ = 0;

  FlashlightEffect flashlight_;
  ColorShiftEffect colorShift_;
  FireEffect fire_;

  IEffect* byId_(Id id);
};