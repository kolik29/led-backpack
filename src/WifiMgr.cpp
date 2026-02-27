#include "WifiMgr.h"

WifiMgr::WifiMgr(const AppConfig::Wifi& cfg) : cfg_(cfg) {}

void WifiMgr::begin() {
  WiFi.persistent(false);
  WiFi.setAutoReconnect(false);
  WiFi.setSleep(false);

  if (!cfg_.enableAp) {
    WiFi.mode(WIFI_OFF);
    return;
  }

  WiFi.mode(WIFI_AP);
  startAp_();

  printInfo_();
}

void WifiMgr::tick() { }

void WifiMgr::startAp_() {
  bool ok = WiFi.softAP(cfg_.apSsid, cfg_.apPass);
  if (!ok) {
    Serial.println("WiFi: AP start failed");
    return;
  }

  Serial.print("WiFi: AP SSID = ");
  Serial.println(cfg_.apSsid);
  Serial.print("WiFi: AP IP = ");
  Serial.println(WiFi.softAPIP());
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