#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include "Display.h"
#include "RenderMgr.h"

class ApiHandlers {
public:
  ApiHandlers(WebServer& server, Display& display, RenderMgr& renderer);

  void begin();

private:
  WebServer& server_;
  Display& display_;
  RenderMgr& renderer_;

  void handlePing_();
  void handleInfo_();
  void handleReboot_();

  void handleBrightness_();
  void handleFrame_();
  void handleClear_();
  void handleMode_();
  void handleText_();
  void handleEffect_();

  void handleNotFound_();

  void sendJson_(int code, const String& json);

  bool parseHexColor_(const char* s, CRGB& out) const;
};
