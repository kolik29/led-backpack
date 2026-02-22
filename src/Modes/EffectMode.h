#pragma once

#include <Arduino.h>

#include "Display.h"
#include "Effects/IEffect.h"
#include "Effects/FlashlightEffect.h"
#include "Effects/ColorShiftEffect.h"
#include "Effects/FireEffect.h"
#include "Effects/SparklesEffect.h"
#include "Effects/PlasmaEffect.h"
#include "Effects/NoiseEffect.h"
#include "Effects/MatrixRainEffect.h"
#include "Effects/SnakeEffect.h"

class EffectMode {
public:
  enum class Id : uint8_t {
    Flashlight,
    ColorShift,
    Fire,
    Sparkles,
    Plasma,
    Noise,
    MatrixRain,
    Snake
  };
  
  void setEffect(Id id, Display& display);
  Id effect() const { return currentId_; }

  void tick(Display& display);

  bool setEffectByName(const String& name, Display& display);

  const char* effectName() const { return current_ ? current_->name() : "none"; }

  FireEffect& fire() { return fire_; }
  ColorShiftEffect& colorShift() { return colorShift_; }
  FlashlightEffect& flashlight() { return flashlight_; }
  SparklesEffect& sparkles() { return sparkles_; }
  PlasmaEffect& plasma() { return plasma_; }
  NoiseEffect& noise() { return noise_; }
  MatrixRainEffect& matrixRain() { return matrixRain_; }
  SnakeEffect& snake() { return snake_; }

private:
  Id currentId_ = Id::Flashlight;
  IEffect* current_ = nullptr;

  uint32_t lastMs_ = 0;

  FlashlightEffect flashlight_;
  ColorShiftEffect colorShift_;
  FireEffect fire_;
  SparklesEffect sparkles_;
  PlasmaEffect plasma_;
  NoiseEffect noise_;
  MatrixRainEffect matrixRain_;
  SnakeEffect snake_;

  IEffect* byId_(Id id);
};