#include "WifiMgr.h"

WifiMgr::WifiMgr(const AppConfig::Wifi& cfg) : cfg_(cfg) {}

void WifiMgr::begin() {
  WiFi.persistent(false);
  WiFi.setAutoReconnect(false);
  WiFi.setSleep(true);

  if (!cfg_.enableAp) {
    WiFi.mode(WIFI_OFF);
    apActive_ = false;
    return;
  }

  WiFi.mode(WIFI_AP);
  startAp_();

  printInfo_();
}

void WifiMgr::tick() {
  if (!apActive_) return;

  if (millis() - apStartedMs_ >= AP_LIFETIME_MS) {
    stopAp_();
  }
}

bool WifiMgr::isActive() const {
  return apActive_;
}

void WifiMgr::startAp_() {
  bool ok = WiFi.softAP(cfg_.apSsid, cfg_.apPass);
  if (!ok) {
    Serial.println("WiFi: AP start failed");
    apActive_ = false;
    return;
  }

  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  apStartedMs_ = millis();
  apActive_ = true;

  Serial.print("WiFi: AP SSID = ");
  Serial.println(cfg_.apSsid);
  Serial.print("WiFi: AP IP = ");
  Serial.println(WiFi.softAPIP());
  Serial.println("WiFi: AP will shut down after 5 minutes");
}

void WifiMgr::stopAp_() {
  Serial.println("WiFi: setup window expired, shutting down AP");
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_OFF);
  apActive_ = false;
}

void WifiMgr::printInfo_() const {
  Serial.print("WiFi mode: ");
  Serial.println((int)WiFi.getMode());

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

bool WifiMgr::isStaConnected() const {
  return false;
}

IPAddress WifiMgr::apIp() const {
  return WiFi.softAPIP();
}

IPAddress WifiMgr::staIp() const {
  return IPAddress(0, 0, 0, 0);
}
