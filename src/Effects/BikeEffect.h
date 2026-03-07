#pragma once

#include "Effects/IEffect.h"
#include <FastLED.h>
#include "Sensors/Mpu6050.h"

class BikeEffect : public IEffect {
public:
  enum class Mode : uint8_t { Solid = 0, Blink = 1, Double = 2 };

  void begin(Display& display) override;
  void tick(Display& display, uint32_t dtMs) override;
  const char* name() const override { return "bike"; }

  void setMode(Mode m) { mode_ = m; }
  void setBrakeTest(bool on) { brakeTest_ = on; } // ручная проверка

private:
  Mode mode_ = Mode::Blink;
  uint32_t tMs_ = 0;

  // ---- LED pattern (фиксировано) ----
  static constexpr uint8_t  BASE_BRIGHTNESS  = 120;
  static constexpr uint8_t  BRAKE_BRIGHTNESS = 255;

  static constexpr uint16_t BLINK_PERIOD_MS = 900;
  static constexpr uint16_t BLINK_ON_MS     = 140;

  static constexpr uint16_t DOUBLE_PERIOD_MS = 900;
  static constexpr uint16_t DOUBLE_ON_MS     = 120;
  static constexpr uint16_t DOUBLE_GAP_MS    = 120;

  // ---- MPU / Brake detection ----
  Mpu6050 mpu_;
  bool mpuOk_ = false;

  // Выбор оси и знака (под установку датчика)
  // 0=X, 1=Y, 2=Z. BRAKE_SIGN подбирается: чтобы при торможении hp было отрицательным.
  static constexpr uint8_t BRAKE_AXIS = 0;
  static constexpr int8_t  BRAKE_SIGN = -1;

  // Фильтр базовой линии (гравитация/наклон/медленные изменения)
  // Коммерческие обычно обновляют базу медленно и только вне торможения.
  static constexpr float BASE_ALPHA = 0.01f; // чем меньше — тем медленнее база

  // Порог срабатывания и отпускания (гистерезис), в "g" для high-pass компоненты
  // Стартовые значения, потом подберёшь по логу.
  static constexpr float TRIGGER_G = 0.22f;
  static constexpr float RELEASE_G = 0.10f;

  // Нужно N подряд измерений, чтобы сработало (анти-кочки)
  static constexpr uint8_t TRIGGER_SAMPLES = 3;

  // Минимальная пауза между триггерами (чтобы не “строчило”)
  static constexpr uint16_t COOLDOWN_MS = 250;

  // Удержание стоп-сигнала после события
  static constexpr uint16_t HOLD_MS = 1000;

  // Внутренние состояния
  bool brakeTest_ = false;

  float base_ = 0.0f;        // медленная база по оси
  bool braking_ = false;     // текущее состояние тормоза
  uint16_t holdLeftMs_ = 0;  // таймер удержания
  uint16_t cooldownLeftMs_ = 0;
  uint8_t triggerCount_ = 0;

  // helpers
  void fillAll_(Display& display, const CRGB& c);
  bool isOn_() const;

  void accelInit_();
  void brakeUpdate_(uint32_t dtMs);
  bool brakeActive_(uint32_t dtMs);

  bool baseInit_ = false;
  uint16_t warmupLeftMs_ = 0;
  static constexpr uint16_t WARMUP_MS = 2000;

  bool debug_ = false;
  uint32_t debugLastMs_ = 0;
};