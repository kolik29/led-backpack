#include "Sensors/Mpu6050.h"

static int16_t be16(const uint8_t* p) {
  return (int16_t)((uint16_t)p[0] << 8 | (uint16_t)p[1]);
}

bool Mpu6050::begin(TwoWire& wire, uint8_t addr) {
  wire_ = &wire;
  addr_ = addr;

  // wake up: PWR_MGMT_1 (0x6B) = 0
  if (!writeReg_(0x6B, 0x00)) return false;

  // Accel config: +/-2g (ACCEL_CONFIG 0x1C = 0)
  if (!writeReg_(0x1C, 0x00)) return false;

  // (опционально) DLPF: CONFIG 0x1A
  // 0x03 примерно даёт фильтрацию и уменьшает шум
  writeReg_(0x1A, 0x03);

  return true;
}

bool Mpu6050::calibrate(uint16_t samples, uint16_t delayMs) {
  if (!wire_) return false;

  float sx = 0, sy = 0, sz = 0;
  uint16_t ok = 0;

  for (uint16_t i = 0; i < samples; i++) {
    float ax, ay, az;
    if (readAccelG(ax, ay, az)) {
      sx += ax;
      sy += ay;
      sz += az;
      ok++;
    }
    delay(delayMs);
  }

  if (ok < (samples / 2)) return false;

  bx_ = sx / (float)ok;
  by_ = sy / (float)ok;
  bz_ = sz / (float)ok;

  return true;
}

bool Mpu6050::readAccelG(float& ax, float& ay, float& az) {
  if (!wire_) return false;

  // ACCEL_XOUT_H = 0x3B, читаем 6 байт accel
  uint8_t buf[6];
  if (!readRegs_(0x3B, buf, 6)) return false;

  int16_t rx = be16(&buf[0]);
  int16_t ry = be16(&buf[2]);
  int16_t rz = be16(&buf[4]);

  // scale for +/-2g: 16384 LSB/g
  const float k = 1.0f / 16384.0f;

  // возвращаем уже "относительно базы" (гравитация/наклон как среднее)
  ax = (float)rx * k - bx_;
  ay = (float)ry * k - by_;
  az = (float)rz * k - bz_;

  return true;
}

bool Mpu6050::writeReg_(uint8_t reg, uint8_t val) {
  wire_->beginTransmission(addr_);
  wire_->write(reg);
  wire_->write(val);
  return wire_->endTransmission() == 0;
}

bool Mpu6050::readRegs_(uint8_t reg, uint8_t* buf, uint8_t len) {
  wire_->beginTransmission(addr_);
  wire_->write(reg);
  if (wire_->endTransmission(false) != 0) return false;

  uint8_t read = wire_->requestFrom(addr_, len, (uint8_t)true);
  if (read != len) return false;

  for (uint8_t i = 0; i < len; i++) buf[i] = wire_->read();
  return true;
}