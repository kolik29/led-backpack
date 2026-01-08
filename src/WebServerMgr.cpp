#include "WebServerMgr.h"

WebServerMgr::WebServerMgr(Display& display, RenderMgr& renderer, uint16_t port)
  : server_(port),
    api_(server_, display, renderer) {}

void WebServerMgr::begin() {
  api_.begin();
  server_.begin();
  Serial.println("HTTP server started");
}

void WebServerMgr::tick() {
  server_.handleClient();
}
