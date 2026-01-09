#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <LittleFS.h>

#include "ApiHandlers.h"
#include "Display.h"
#include "RenderMgr.h"

class WebServerMgr {
public:
  WebServerMgr(Display& display, RenderMgr& renderer, uint16_t port = 80);

  void begin();
  void tick();

private:
  WebServer server_;
  ApiHandlers api_;

  void handleNotFound_();
  bool serveFile_(String path);
  String contentType_(const String& path) const;
};
