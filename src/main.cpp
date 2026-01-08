#include <Arduino.h>

#include "AppConfig.h"
#include "WifiMgr.h"
#include "Display.h"
#include "RenderMgr.h"
#include "WebServerMgr.h"

WifiMgr wifi(AppConfig::wifi());
RenderMgr renderer(display);
WebServerMgr web(display, renderer, 80);
Display display(16, 16, true);

void setup() {
  Serial.begin(115200);
  delay(200);

  display.begin();
  display.clear(true);

  wifi.begin();
  web.begin();

  Serial.println("Ready");
}

void loop() {
  wifi.tick();
  web.tick();
  renderer.tick();

  delay(1);
}
