#include "WebServerMgr.h"

WebServerMgr::WebServerMgr(Display& display, RenderMgr& renderer, uint16_t port)
  : server_(port),
    api_(server_, display, renderer) {}

void WebServerMgr::begin() {
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
  } else {
    Serial.println("LittleFS mounted");
  }

  api_.begin();

  server_.on("/", HTTP_GET, [this]() {
    if (!serveFile_("/www/index.html")) {
      server_.send(500, "text/plain", "index.html not found in LittleFS");
    }
  });

  server_.onNotFound([this]() { handleNotFound_(); });

  server_.begin();
  Serial.println("HTTP server started");
}

void WebServerMgr::tick() {
  server_.handleClient();
}

void WebServerMgr::handleNotFound_() {
  const String uri = server_.uri();

  if (uri.startsWith("/api/")) {
    server_.send(404, "application/json", "{\"ok\":false,\"error\":\"not found\"}");
    return;
  }

  String path = "/www" + uri;

  if (uri.endsWith("/")) {
    path += "index.html";
  }

  if (serveFile_(path)) return;

  if (serveFile_("/www/index.html")) return;

  server_.send(404, "text/plain", "Not found");
}

bool WebServerMgr::serveFile_(String path) {
  if (!LittleFS.exists(path)) return false;

  File f = LittleFS.open(path, "r");
  if (!f) return false;

  server_.streamFile(f, contentType_(path));
  f.close();
  return true;
}

String WebServerMgr::contentType_(const String& path) const {
  if (path.endsWith(".html")) return "text/html";
  if (path.endsWith(".css"))  return "text/css";
  if (path.endsWith(".js"))   return "application/javascript";
  if (path.endsWith(".map"))  return "application/json";
  if (path.endsWith(".json")) return "application/json";
  if (path.endsWith(".png"))  return "image/png";
  if (path.endsWith(".jpg") || path.endsWith(".jpeg")) return "image/jpeg";
  if (path.endsWith(".svg"))  return "image/svg+xml";
  if (path.endsWith(".ico"))  return "image/x-icon";
  if (path.endsWith(".woff2")) return "font/woff2";
  if (path.endsWith(".woff"))  return "font/woff";
  return "application/octet-stream";
}
