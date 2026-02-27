#pragma once

#include <Arduino.h>

class AppConfig {
public:
  struct Wifi {
    const char* apSsid;
    const char* apPass;

    bool enableAp;
  };

  static const Wifi& wifi();
};
