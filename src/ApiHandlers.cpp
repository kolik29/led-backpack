#include "ApiHandlers.h"

#include <WiFi.h>
#include <esp_system.h>
#include <ArduinoJson.h>
#include <mbedtls/base64.h>

static bool base64DecodeInto(const String& b64, uint8_t* out, size_t outCap, size_t& outLen) {
  outLen = 0;
  int rc = mbedtls_base64_decode(
    out, outCap,
    &outLen,
    (const unsigned char*)b64.c_str(),
    b64.length()
  );
  return (rc == 0);
}

ApiHandlers::ApiHandlers(WebServer& server, Display& display, RenderMgr& renderer)
  : server_(server), display_(display), renderer_(renderer) {}

void ApiHandlers::begin() {
  server_.on("/api/ping", HTTP_GET, [this]() { handlePing_(); });
  server_.on("/api/info", HTTP_GET, [this]() { handleInfo_(); });
  server_.on("/api/reboot", HTTP_POST, [this]() { handleReboot_(); });

  server_.on("/api/brightness", HTTP_POST, [this]() { handleBrightness_(); });
  server_.on("/api/frame", HTTP_POST, [this]() { handleFrame_(); });
  server_.on("/api/clear", HTTP_POST, [this]() { handleClear_(); });
  server_.on("/api/mode", HTTP_POST, [this]() { handleMode_(); });
  server_.on("/api/text", HTTP_POST, [this]() { handleText_(); });
  server_.on("/api/effect", HTTP_POST, [this]() { handleEffect_(); });

  server_.onNotFound([this]() { handleNotFound_(); });
}

void ApiHandlers::handlePing_() {
  server_.send(200, "text/plain", "pong");
}

void ApiHandlers::handleInfo_() {
  bool staConnected = (WiFi.status() == WL_CONNECTED);

  String json = "{";
  json += "\"ap_ip\":\"" + WiFi.softAPIP().toString() + "\",";
  json += "\"sta_connected\":" + String(staConnected ? "true" : "false") + ",";
  json += "\"sta_ip\":\"" + (staConnected ? WiFi.localIP().toString() : String("")) + "\",";
  json += "\"rssi\":" + String(staConnected ? WiFi.RSSI() : 0) + ",";
  json += "\"matrix\":{";
  json += "\"w\":" + String(display_.width()) + ",";
  json += "\"h\":" + String(display_.height()) + ",";
  json += "\"brightness\":" + String(display_.getBrightness()) + ",";
  json += "\"mode\":" + String((int)renderer_.mode());
  json += "}";
  json += ",\"effect\":\"";
  json += renderer_.effects().effectName();
  json += "\"";
  json += "}";

  sendJson_(200, json);
}

void ApiHandlers::handleReboot_() {
  sendJson_(200, "{\"ok\":true}");
  delay(150);
  ESP.restart();
}

bool ApiHandlers::parseHexColor_(const char* s, CRGB& out) const {
  if (!s || s[0] != '#' || strlen(s) != 7) return false;

  auto hexVal = [](char c) -> int {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
  };

  auto hexByte = [&](char hi, char lo) -> int {
    int a = hexVal(hi);
    int b = hexVal(lo);
    if (a < 0 || b < 0) return -1;
    return (a << 4) | b;
  };

  int rr = hexByte(s[1], s[2]);
  int gg = hexByte(s[3], s[4]);
  int bb = hexByte(s[5], s[6]);
  if (rr < 0 || gg < 0 || bb < 0) return false;

  out = CRGB((uint8_t)rr, (uint8_t)gg, (uint8_t)bb);
  return true;
}

void ApiHandlers::handleBrightness_() {
  if (!server_.hasArg("plain")) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Missing body\"}");
    return;
  }

  JsonDocument doc;
  if (deserializeJson(doc, server_.arg("plain"))) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Bad JSON\"}");
    return;
  }

  int b = doc["brightness"] | -1;
  if (b < 0 || b > 255) {
    sendJson_(400, "{\"ok\":false,\"error\":\"brightness must be 0..255\"}");
    return;
  }

  display_.setBrightness((uint8_t)b);
  sendJson_(200, "{\"ok\":true}");
}

void ApiHandlers::handleClear_() {
  display_.clear(true);
  renderer_.setMode(RenderMgr::Mode::Frame);
  renderer_.text().reset();
  sendJson_(200, "{\"ok\":true}");
}

void ApiHandlers::handleMode_() {
  if (!server_.hasArg("plain")) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Missing body\"}");
    return;
  }

  JsonDocument doc;
  if (deserializeJson(doc, server_.arg("plain"))) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Bad JSON\"}");
    return;
  }

  const char* mode = doc["mode"] | "";
  String m = String(mode);
  m.trim();
  m.toLowerCase();

  if (m == "text") {
    renderer_.setMode(RenderMgr::Mode::Text);
    sendJson_(200, "{\"ok\":true}");
    return;
  }

  if (m == "effect") {
    const char* eff = doc["effect"] | "";
    String e = String(eff);
    e.trim();
    e.toLowerCase();

    if (e.length() == 0) {
      renderer_.setMode(RenderMgr::Mode::Effect);
      sendJson_(200, "{\"ok\":true}");
      return;
    }

    if (e == "fire") renderer_.setEffect(EffectMode::Id::Fire);
    else if (e == "colorshift") renderer_.setEffect(EffectMode::Id::ColorShift);
    else if (e == "flashlight") renderer_.setEffect(EffectMode::Id::Flashlight);
    else {
      sendJson_(400, "{\"ok\":false,\"error\":\"unknown effect\"}");
      return;
    }

    sendJson_(200, "{\"ok\":true}");
    return;
  }

  renderer_.setMode(RenderMgr::Mode::Frame);
  sendJson_(200, "{\"ok\":true}");
}

void ApiHandlers::handleText_() {
  if (!server_.hasArg("plain")) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Missing body\"}");
    return;
  }

  JsonDocument doc;
  if (deserializeJson(doc, server_.arg("plain"))) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Bad JSON\"}");
    return;
  }

  const char* t = doc["text"] | "";
  String text = String(t);

  const char* c = doc["color"] | "#ff0000";
  CRGB color;
  if (!parseHexColor_(c, color)) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Bad color\"}");
    return;
  }

  int speed = doc["speed"] | 20;
  bool scroll = doc["scroll"] | true;
  int spacing = doc["spacing"] | 1;
  int y = doc["y"] | -1;

  renderer_.text().setText(text);
  renderer_.text().setColor(color);
  renderer_.text().setSpeedPxPerSec((uint16_t)((speed <= 0) ? 1 : speed));
  renderer_.text().setScroll(scroll);
  renderer_.text().setSpacing((uint8_t)((spacing < 0) ? 0 : spacing));
  renderer_.text().setY((int8_t)y);

  renderer_.setMode(RenderMgr::Mode::Text);

  sendJson_(200, "{\"ok\":true}");
}

void ApiHandlers::handleEffect_() {
  if (!server_.hasArg("plain")) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Missing body\"}");
    return;
  }

  JsonDocument doc;
  if (deserializeJson(doc, server_.arg("plain"))) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Bad JSON\"}");
    return;
  }

  const char* eff = doc["effect"] | "";
  String e = String(eff);
  e.trim();
  e.toLowerCase();

  if (e.length() == 0) {
    sendJson_(400, "{\"ok\":false,\"error\":\"effect required\"}");
    return;
  }

  JsonObject params;
  bool hasParams = doc["params"].is<JsonObject>();
  if (hasParams) params = doc["params"].as<JsonObject>();

  auto clampInt = [](int v, int lo, int hi) -> int {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
  };

  if (e == "fire") {
    renderer_.setEffect(EffectMode::Id::Fire);

    if (hasParams) {
      int cooling  = params["cooling"]  | 55;
      int sparking = params["sparking"] | 120;
      int stepMs   = params["stepMs"]   | 30;

      cooling  = clampInt(cooling,  10, 120);
      sparking = clampInt(sparking,  0, 255);
      stepMs   = clampInt(stepMs,    5, 200);

      renderer_.effects().fire().setCooling((uint8_t)cooling);
      renderer_.effects().fire().setSparking((uint8_t)sparking);
      renderer_.effects().fire().setStepMs((uint16_t)stepMs);

      int maxFlares   = params["maxFlares"]   | 3;
      int flareChance = params["flareChance"] | 30;
      int flareRows   = params["flareRows"]   | 7;
      int flareDecay  = params["flareDecay"]  | 14;
      bool flipY      = params["flipY"]       | true;

      maxFlares   = clampInt(maxFlares,   0, 8);
      flareChance = clampInt(flareChance, 0, 100);
      flareRows   = clampInt(flareRows,   1, (int)display_.height());
      flareDecay  = clampInt(flareDecay,  1, 60);

      renderer_.effects().fire().setMaxFlares((uint8_t)maxFlares);
      renderer_.effects().fire().setFlareChance((uint8_t)flareChance);
      renderer_.effects().fire().setFlareRows((uint8_t)flareRows);
      renderer_.effects().fire().setFlareDecay((uint8_t)flareDecay);
      renderer_.effects().fire().setFlipY(flipY);
    }

    sendJson_(200, "{\"ok\":true}");
    return;
  }

  if (e == "colorshift") {
    renderer_.setEffect(EffectMode::Id::ColorShift);

    if (hasParams) {
      int changeEveryMs  = params["changeEveryMs"]  | 900;
      int blendPerSecond = params["blendPerSecond"] | 120;
      int updateCount    = params["updateCount"]    | 0;

      changeEveryMs  = clampInt(changeEveryMs,  50, 10000);
      blendPerSecond = clampInt(blendPerSecond,  1, 255);
      updateCount    = clampInt(updateCount,     0, 256);

      if (params["style"].is<const char*>()) {
        String s = String((const char*)params["style"]);
        s.trim();
        s.toLowerCase();

        if (s == "global") {
          renderer_.effects().colorShift().setStyle(ColorShiftEffect::Style::Global);
        } else if (s == "perpixel" || s == "per_pixel") {
          renderer_.effects().colorShift().setStyle(ColorShiftEffect::Style::PerPixel);
        } else {
          sendJson_(400, "{\"ok\":false,\"error\":\"colorshift.style must be global|perpixel\"}");
          return;
        }
      }

      renderer_.effects().colorShift().setChangeEveryMs((uint32_t)changeEveryMs);
      renderer_.effects().colorShift().setBlendPerSecond((uint8_t)blendPerSecond);
      renderer_.effects().colorShift().setUpdateCount((uint16_t)updateCount);
    }

    sendJson_(200, "{\"ok\":true}");
    return;
  }

  if (e == "flashlight") {
    renderer_.setEffect(EffectMode::Id::Flashlight);

    if (hasParams) {
      if (params["color"].is<const char*>()) {
        const char* s = params["color"] | "#ffffff";
        CRGB cc;
        if (!parseHexColor_(s, cc)) {
          sendJson_(400, "{\"ok\":false,\"error\":\"Bad color\"}");
          return;
        }
        renderer_.effects().flashlight().setColor(cc);
      } else {
        int r = params["r"] | 255;
        int g = params["g"] | 255;
        int b = params["b"] | 255;

        r = clampInt(r, 0, 255);
        g = clampInt(g, 0, 255);
        b = clampInt(b, 0, 255);

        renderer_.effects().flashlight().setColor(CRGB((uint8_t)r, (uint8_t)g, (uint8_t)b));
      }
    }

    sendJson_(200, "{\"ok\":true}");
    return;
  }

  sendJson_(400, "{\"ok\":false,\"error\":\"unknown effect\"}");
}

void ApiHandlers::handleFrame_() {
  if (!server_.hasArg("plain")) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Missing body\"}");
    return;
  }

  JsonDocument doc;
  if (deserializeJson(doc, server_.arg("plain"))) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Bad JSON\"}");
    return;
  }

  int w = doc["w"] | 0;
  int h = doc["h"] | 0;

  if (w <= 0 || h <= 0) {
    sendJson_(400, "{\"ok\":false,\"error\":\"w/h required\"}");
    return;
  }

  uint16_t n = (uint16_t)w * (uint16_t)h;
  if (n == 0 || n > Display::MAX_LEDS) {
    sendJson_(400, "{\"ok\":false,\"error\":\"w*h too big\"}");
    return;
  }

  if (w != display_.width() || h != display_.height()) {
    display_.setSize((uint8_t)w, (uint8_t)h);
  }

  display_.clear(false);

  if (doc["pixels"].is<JsonArray>()) {
    JsonArray arr = doc["pixels"].as<JsonArray>();
    if (arr.size() != n) {
      sendJson_(400, "{\"ok\":false,\"error\":\"pixels array length != w*h\"}");
      return;
    }

    for (uint16_t i = 0; i < n; i++) {
      const char* s = arr[i] | "#000000";
      CRGB cc;
      if (!parseHexColor_(s, cc)) {
        sendJson_(400, "{\"ok\":false,\"error\":\"bad color in pixels\"}");
        return;
      }

      uint8_t x = (uint8_t)(i % (uint16_t)w);
      uint8_t y = (uint8_t)(i / (uint16_t)w);
      display_.setPixelXY(x, y, cc);
    }

    display_.show();
    renderer_.setMode(RenderMgr::Mode::Frame);
    sendJson_(200, "{\"ok\":true}");
    return;
  }

  if (!doc["palette"].is<JsonArray>() || !doc["pixels"].is<const char*>()) {
    sendJson_(400, "{\"ok\":false,\"error\":\"Expected pixels array OR palette+pixels(base64)\"}");
    return;
  }

  JsonArray palArr = doc["palette"].as<JsonArray>();
  size_t palSize = palArr.size();
  if (palSize < 1 || palSize > 64) {
    sendJson_(400, "{\"ok\":false,\"error\":\"palette must be 1..64\"}");
    return;
  }

  CRGB palette[64];
  for (size_t i = 0; i < palSize; i++) {
    const char* s = palArr[i] | "#000000";
    if (!parseHexColor_(s, palette[i])) {
      sendJson_(400, "{\"ok\":false,\"error\":\"bad color in palette\"}");
      return;
    }
  }

  String b64 = String((const char*)doc["pixels"]);

  uint8_t idxBuf[Display::MAX_LEDS];
  size_t outLen = 0;

  if (!base64DecodeInto(b64, idxBuf, sizeof(idxBuf), outLen)) {
    sendJson_(400, "{\"ok\":false,\"error\":\"bad base64\"}");
    return;
  }

  if (outLen != n) {
    sendJson_(400, "{\"ok\":false,\"error\":\"base64 length != w*h\"}");
    return;
  }

  for (uint16_t i = 0; i < n; i++) {
    uint8_t pi = idxBuf[i];
    if (pi >= palSize) pi = 0;

    uint8_t x = (uint8_t)(i % (uint16_t)w);
    uint8_t y = (uint8_t)(i / (uint16_t)w);
    display_.setPixelXY(x, y, palette[pi]);
  }

  display_.show();
  renderer_.setMode(RenderMgr::Mode::Frame);
  sendJson_(200, "{\"ok\":true}");
}

void ApiHandlers::handleNotFound_() {
  server_.send(404, "text/plain", "Not found");
}

void ApiHandlers::sendJson_(int code, const String& json) {
  server_.send(code, "application/json", json);
}
