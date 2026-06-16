#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "AppConfig.h"

class WifiMgr {
public:
  explicit WifiMgr(const AppConfig::Wifi& cfg);

  void begin();
  void tick();
  bool isActive() const;

  bool isStaConnected() const;
  IPAddress apIp() const;
  IPAddress staIp() const;

private:
  static constexpr uint32_t AP_LIFETIME_MS = 5UL * 60UL * 1000UL;

  AppConfig::Wifi cfg_;

  uint32_t apStartedMs_ = 0;
  bool apActive_ = false;

  void startAp_();
  void stopAp_();
  bool connectStaOnce_(uint32_t timeoutMs);
  void printInfo_() const;
};
