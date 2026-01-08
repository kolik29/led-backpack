#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "AppConfig.h"

class WifiMgr {
public:
  explicit WifiMgr(const AppConfig::Wifi& cfg);

  void begin();
  void tick();

  bool isStaConnected() const;
  IPAddress apIp() const;
  IPAddress staIp() const;

private:
  AppConfig::Wifi cfg_;

  uint32_t lastReconnectTryMs_ = 0;

  void startAp_();
  bool connectStaOnce_(uint32_t timeoutMs);
  void printInfo_() const;
};
