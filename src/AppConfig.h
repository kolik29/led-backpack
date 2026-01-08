#pragma once

#include <Arduino.h>

class AppConfig {
public:
  struct Wifi {
    const char* apSsid;
    const char* apPass;

    const char* staSsid;
    const char* staPass;

    uint32_t staConnectTimeoutMs;
    uint32_t reconnectIntervalMs;

    bool enableAp;
    bool enableSta;
  };

  static const Wifi& wifi();
};
