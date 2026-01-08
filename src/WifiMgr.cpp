#include "WifiMgr.h"

WifiMgr::WifiMgr(const AppConfig::Wifi& cfg) : cfg_(cfg) {}

void WifiMgr::begin() {
  WiFi.persistent(false);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);

  if (cfg_.enableAp && cfg_.enableSta) {
    WiFi.mode(WIFI_AP_STA);
  } else if (cfg_.enableAp) {
    WiFi.mode(WIFI_AP);
  } else if (cfg_.enableSta) {
    WiFi.mode(WIFI_STA);
  } else {
    WiFi.mode(WIFI_OFF);
    return;
  }

  if (cfg_.enableAp) {
    startAp_();
  }

  if (cfg_.enableSta && cfg_.staSsid && cfg_.staSsid[0] != '\0') {
    connectStaOnce_(cfg_.staConnectTimeoutMs);
  }

  printInfo_();
}

void WifiMgr::tick() {
  if (!cfg_.enableSta) return;
  if (!cfg_.staSsid || cfg_.staSsid[0] == '\0') return;

  if (WiFi.status() == WL_CONNECTED) return;

  const uint32_t now = millis();
  if (now - lastReconnectTryMs_ < cfg_.reconnectIntervalMs) return;

  lastReconnectTryMs_ = now;
  Serial.println("WiFi: reconnect try");
  connectStaOnce_(2000);
}

bool WifiMgr::connectStaOnce_(uint32_t timeoutMs) {
  WiFi.disconnect(true, true);
  delay(50);

  WiFi.begin(cfg_.staSsid, cfg_.staPass);

  const uint32_t start = millis();
  Serial.print("WiFi: connecting to STA ");
  Serial.println(cfg_.staSsid);

  while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeoutMs) {
    delay(200);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi: STA connected");
    Serial.print("WiFi: STA IP = ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("WiFi: STA not connected");
  return false;
}

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

  if (cfg_.enableAp) {
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
  }

  if (cfg_.enableSta) {
    Serial.print("STA status: ");
    Serial.println((int)WiFi.status());

    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("STA IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("RSSI: ");
      Serial.println(WiFi.RSSI());
    }
  }
}

bool WifiMgr::isStaConnected() const {
  return WiFi.status() == WL_CONNECTED;
}

IPAddress WifiMgr::apIp() const {
  return WiFi.softAPIP();
}

IPAddress WifiMgr::staIp() const {
  return WiFi.localIP();
}
