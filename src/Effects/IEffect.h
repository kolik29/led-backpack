#pragma once
#include <Arduino.h>

class Display;

class IEffect {
public:
  virtual ~IEffect() = default;
  virtual void begin(Display& display) = 0;
  virtual void tick(Display& display, uint32_t dtMs) = 0;
  virtual const char* name() const = 0;
};