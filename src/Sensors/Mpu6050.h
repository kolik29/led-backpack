#pragma once

#include <Arduino.h>
#include <Wire.h>

class Mpu6050 {
public:
  bool begin(TwoWire& wire = Wire, uint8_t addr = 0x68);

  // прочитать ускорение в "g" (float)
  bool readAccelG(float& ax, float& ay, float& az);

  // калибровка нуля (среднее по N измерениям)
  bool calibrate(uint16_t samples = 200, uint16_t delayMs = 5);

private:
  TwoWire* wire_ = nullptr;
  uint8_t addr_ = 0x68;

  // смещения "гравитационной" базы (в g)
  float bx_ = 0.0f;
  float by_ = 0.0f;
  float bz_ = 0.0f;

  bool writeReg_(uint8_t reg, uint8_t val);
  bool readRegs_(uint8_t reg, uint8_t* buf, uint8_t len);
};