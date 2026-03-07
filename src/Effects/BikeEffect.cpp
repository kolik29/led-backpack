#include "Effects/BikeEffect.h"
#include "Display.h"

void BikeEffect::begin(Display& /*display*/) {
  tMs_ = 0;
  base_ = 0.0f;

  braking_ = false;
  holdLeftMs_ = 0;
  cooldownLeftMs_ = 0;
  triggerCount_ = 0;

  baseInit_ = false;
  warmupLeftMs_ = WARMUP_MS;

  accelInit_();
}

void BikeEffect::accelInit_() {
  mpuOk_ = mpu_.begin(Wire, 0x68);
  if (!mpuOk_) {
    Serial.println("MPU6050: init failed");
    return;
  }

  // Желательно неподвижно 1–2 сек
  if (!mpu_.calibrate(250, 4)) {
    Serial.println("MPU6050: calibrate failed");
    mpuOk_ = false;
    return;
  }

  Serial.println("MPU6050: ready");
}

void BikeEffect::fillAll_(Display& display, const CRGB& c) {
  const uint8_t w = display.width();
  const uint8_t h = display.height();

  display.clear(false);
  for (uint8_t y = 0; y < h; y++) {
    for (uint8_t x = 0; x < w; x++) {
      display.setPixelXY(x, y, c);
    }
  }
  display.show();
}

bool BikeEffect::isOn_() const {
  if (mode_ == Mode::Solid) return true;

  if (mode_ == Mode::Blink) {
    uint32_t m = tMs_ % BLINK_PERIOD_MS;
    return m < BLINK_ON_MS;
  }

  uint32_t m = tMs_ % DOUBLE_PERIOD_MS;

  uint32_t a = DOUBLE_ON_MS;
  uint32_t b = a + DOUBLE_GAP_MS;
  uint32_t c = b + DOUBLE_ON_MS;

  if (m < a) return true;
  if (m < b) return false;
  if (m < c) return true;
  return false;
}

void BikeEffect::brakeUpdate_(uint32_t dtMs) {
  // ручной тест — как “коммерческий стоп”: сразу включаем и держим HOLD_MS
  if (brakeTest_) {
    braking_ = true;
    holdLeftMs_ = HOLD_MS;
    triggerCount_ = 0;
    return;
  }

  // таймеры
  if (cooldownLeftMs_ > 0) {
    uint16_t dec = (dtMs > 1000) ? 1000 : (uint16_t)dtMs;
    cooldownLeftMs_ = (cooldownLeftMs_ > dec) ? (cooldownLeftMs_ - dec) : 0;
  }

  if (holdLeftMs_ > 0) {
    uint16_t dec = (dtMs > 1000) ? 1000 : (uint16_t)dtMs;
    holdLeftMs_ = (holdLeftMs_ > dec) ? (holdLeftMs_ - dec) : 0;
  }

  if (!mpuOk_) {
    // без датчика — только по ручному тесту
    braking_ = (holdLeftMs_ > 0);
    return;
  }

  float ax, ay, az;
  if (!mpu_.readAccelG(ax, ay, az)) {
    // пропускаем один тик, состояние сохраняем
    braking_ = (holdLeftMs_ > 0) || braking_;
    return;
  }

  float v = (BRAKE_AXIS == 0) ? ax : (BRAKE_AXIS == 1 ? ay : az);
  v *= (float)BRAKE_SIGN;

  // 1) инициализируем базу первым значением, чтобы не было стартового "прыжка"
  if (!baseInit_) {
    base_ = v;
    baseInit_ = true;
  }

  // 2) прогрев: пока warmup идёт — обновляем базу, но тормоз не включаем
  if (warmupLeftMs_ > 0) {
    uint16_t dec = (dtMs > 1000) ? 1000 : (uint16_t)dtMs;
    warmupLeftMs_ = (warmupLeftMs_ > dec) ? (warmupLeftMs_ - dec) : 0;

    // обновляем базу быстрее во время прогрева
    base_ = base_ + (v - base_) * 0.08f; // временно быстрее
    braking_ = false;
    holdLeftMs_ = 0;
    triggerCount_ = 0;
    return;
  }

  // Коммерческий приём: базу обновляем всегда, но заметно медленнее во время торможения,
  // чтобы база не “съедала” тормоз.
  const float alpha = braking_ ? (BASE_ALPHA * 0.2f) : BASE_ALPHA;
  base_ = base_ + (v - base_) * alpha;

  float hp = v - base_; // high-pass: быстрые изменения (тормоз/рывки)

  if (debug_) {
    uint32_t now = millis();
    if (now - debugLastMs_ >= 200) {
      debugLastMs_ = now;

      Serial.print("MPU ok=");
      Serial.print(mpuOk_ ? "1" : "0");

      Serial.print(" ax=");
      Serial.print(ax, 3);
      Serial.print(" ay=");
      Serial.print(ay, 3);
      Serial.print(" az=");
      Serial.print(az, 3);

      Serial.print(" v=");
      Serial.print(v, 3);
      Serial.print(" base=");
      Serial.print(base_, 3);
      Serial.print(" hp=");
      Serial.print(hp, 3);

      Serial.print(" trig=");
      Serial.print((int)triggerCount_);
      Serial.print(" hold=");
      Serial.print((int)holdLeftMs_);
      Serial.print(" cd=");
      Serial.print((int)cooldownLeftMs_);

      Serial.print(" braking=");
      Serial.print(braking_ ? "1" : "0");
      Serial.println();
    }
  }

  // 1) Срабатывание: нужен TRIGGER_SAMPLES подряд ниже -TRIGGER_G
  if (!braking_ && cooldownLeftMs_ == 0) {
    if (hp < -TRIGGER_G) {
      if (triggerCount_ < 255) triggerCount_++;
      if (triggerCount_ >= TRIGGER_SAMPLES) {
        braking_ = true;
        holdLeftMs_ = HOLD_MS;
        cooldownLeftMs_ = COOLDOWN_MS;
        triggerCount_ = 0;
      }
    } else {
      triggerCount_ = 0;
    }
  }

  // 2) Пока тормоз активен: если продолжается “негативный hp”, продлеваем hold
  // Это делает поведение похожим на “горит пока торможу”, но без дребезга.
  if (braking_) {
    if (hp < -RELEASE_G) {
      holdLeftMs_ = HOLD_MS;
    }

    // выключаем, когда hold истёк
    if (holdLeftMs_ == 0) {
      braking_ = false;
    }
  }
}

bool BikeEffect::brakeActive_(uint32_t dtMs) {
  brakeUpdate_(dtMs);
  return braking_;
}

void BikeEffect::tick(Display& display, uint32_t dtMs) {
  tMs_ += dtMs;

  // Brake override (как у коммерческих: ярко и заметно, затем возврат в базовый режим)
  if (brakeActive_(dtMs)) {
    CRGB c = CRGB::Red;
    c.nscale8_video(BRAKE_BRIGHTNESS);
    fillAll_(display, c);
    return;
  }

  // Base pattern
  if (isOn_()) {
    CRGB c = CRGB::Red;
    c.nscale8_video(BASE_BRIGHTNESS);
    fillAll_(display, c);
  } else {
    fillAll_(display, CRGB::Black);
  }
}