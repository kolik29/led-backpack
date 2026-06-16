#include <Arduino.h>
#include <Wire.h>

#include "AppConfig.h"
#include "WifiMgr.h"
#include "Display.h"
#include "RenderMgr.h"
#include "WebServerMgr.h"

Display display(16, 16, true);
WifiMgr wifi(AppConfig::wifi());
RenderMgr renderer(display);
WebServerMgr web(display, renderer, 80);

static constexpr uint32_t CPU_FREQ_MHZ = 80;

void setup() {
  setCpuFrequencyMhz(CPU_FREQ_MHZ);

  Serial.begin(115200);
  delay(200);

  Wire.begin(21, 20);
  Wire.setClock(100000);

  display.begin();
  display.clear(true);

  wifi.begin();
  if (wifi.isActive()) {
    web.begin();
  }

  Serial.println("Ready");
}

void loop() {
  wifi.tick();
  web.tick(wifi.isActive());
  renderer.tick();

  delay(wifi.isActive() ? 5 : 20);
}
