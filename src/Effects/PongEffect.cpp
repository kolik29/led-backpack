#include "Effects/PongEffect.h"
#include "Display.h"

static int16_t pxQ(int x) { return (int16_t)(x << 8); }
static int16_t qToPx(int16_t q) { return (int16_t)(q >> 8); }

void PongEffect::begin(Display& display) {
  w_ = display.width();
  h_ = display.height();

  // paddles centered
  ly_ = pxQ((int)(h_ / 2));
  ry_ = pxQ((int)(h_ / 2));

  resetBall_(true);
  accMs_ = 0;
}

void PongEffect::resetBall_(bool toRight) {
  bx_ = pxQ((int)(w_ / 2));
  by_ = pxQ((int)(h_ / 2));

  int16_t base = (int16_t)random16(220, 360); // ~0.86..1.40 px/step
  vx_ = toRight ? base : -base;

  int16_t vy = (int16_t)random16(120, 280);
  if (random8() & 1) vy = -vy;
  vy_ = vy;
}

void PongEffect::tick(Display& display, uint32_t dtMs) {
  if (w_ == 0 || h_ == 0) begin(display);

  accMs_ += dtMs;
  while (accMs_ >= stepMs_) {
    accMs_ -= stepMs_;
    step_();
  }

  draw_(display);
}

void PongEffect::botMove_() {
  const int16_t target = by_;
  const int16_t maxSpeed = pxQ(1); // 1 px per step

  auto moveToward = [&](int16_t& py) {
    int16_t d = target - py;
    int16_t step = 0;

    if (d > pxQ(1)) step = maxSpeed;
    else if (d < -pxQ(1)) step = -maxSpeed;

    if ((random8() & 0x0F) == 0) {
      step = (int16_t)(step + (int16_t)random16(-60, 61)); // +-0.23 px
    }

    py += step;

    int16_t half = pxQ((int)paddleSize_ / 2);
    py = clampQ_(py, half, pxQ((int)h_ - 1) - half);
  };

  moveToward(ly_);
  moveToward(ry_);
}

void PongEffect::step_() {
  botMove_();

  // next pos
  int16_t nbx = (int16_t)(bx_ + vx_);
  int16_t nby = (int16_t)(by_ + vy_);

  int16_t yMin = 0;
  int16_t yMax = pxQ((int)h_ - 1);

  // bounce top/bottom
  if (nby < yMin) {
    nby = (int16_t)(yMin + (yMin - nby));
    vy_ = (int16_t)(-vy_);
  } else if (nby > yMax) {
    nby = (int16_t)(yMax - (nby - yMax));
    vy_ = (int16_t)(-vy_);
  }

  // paddle collision zone x
  const int ballX = qToPx(nbx);
  const int ballY = qToPx(nby);

  auto paddleHit = [&](bool left) -> bool {
    const int paddleX = left ? 0 : (int)w_ - 1;
    if (ballX != paddleX) return false;

    int16_t py = left ? ly_ : ry_;
    int pc = qToPx(py);
    int half = (int)paddleSize_ / 2;
    int top = pc - half;
    int bot = pc + half;

    return (ballY >= top && ballY <= bot);
  };

  // left paddle at x=0
  if (vx_ < 0 && paddleHit(true)) {
    vx_ = (int16_t)(-vx_);

    int pc = qToPx(ly_);
    int d = ballY - pc; // -half..half
    vy_ = (int16_t)(vy_ + (int16_t)(d * 20));
    if (vy_ > 500) vy_ = 500;
    if (vy_ < -500) vy_ = -500;
  }

  // right paddle at x=w-1
  if (vx_ > 0 && paddleHit(false)) {
    vx_ = (int16_t)(-vx_);

    int pc = qToPx(ry_);
    int d = ballY - pc;
    vy_ = (int16_t)(vy_ + (int16_t)(d * 20));
    if (vy_ > 500) vy_ = 500;
    if (vy_ < -500) vy_ = -500;
  }

  if (ballX < 0) {
    resetBall_(false); // to left
    return;
  }
  if (ballX >= (int)w_) {
    resetBall_(true); // to right
    return;
  }

  // commit
  bx_ = nbx;
  by_ = nby;
}

void PongEffect::draw_(Display& display) {
  display.clear(false);

  // paddles
  auto drawPaddle = [&](int x, int16_t py) {
    int pc = qToPx(py);
    int half = (int)paddleSize_ / 2;
    for (int dy = -half; dy <= half; dy++) {
      int y = pc + dy;
      if (y >= 0 && y < (int)h_) {
        display.setPixelXY((uint8_t)x, (uint8_t)y, paddleColor_);
      }
    }
  };

  drawPaddle(0, ly_);
  drawPaddle((int)w_ - 1, ry_);

  // ball
  int x = qToPx(bx_);
  int y = qToPx(by_);
  if (x >= 0 && x < (int)w_ && y >= 0 && y < (int)h_) {
    display.setPixelXY((uint8_t)x, (uint8_t)y, ballColor_);
  }

  display.show();
}