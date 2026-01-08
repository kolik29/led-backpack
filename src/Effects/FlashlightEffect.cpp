#include "Effects/FlashlightEffect.h"
#include <Display.h>

void FlashlightEffect::begin(Display& display) {
  render_(display);
}

void FlashlightEffect::tick(Display& display, uint32_t /*dtMs*/) {
  render_(display);
}

void FlashlightEffect::render_(Display& display) {
  display.clear(false);

  for (uint8_t y = 0; y < display.height(); y++) {
    for (uint8_t x = 0; x < display.width(); x++) {
      display.setPixelXY(x, y, color_);
    }
  }

  display.show();
}