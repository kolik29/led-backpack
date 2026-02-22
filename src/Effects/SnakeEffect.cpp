#include "Effects/SnakeEffect.h"
#include "Display.h"

void SnakeEffect::begin(Display& display) {
  w_ = display.width();
  h_ = display.height();

  if (w_ == 0 || h_ == 0 || (uint16_t)w_ * (uint16_t)h_ > MAX_CELLS) {
    len_ = 0;
    hasFood_ = false;
    return;
  }

  uint16_t cells = (uint16_t)w_ * (uint16_t)h_;
  len_ = fixedLen_;
  if (len_ < 2) len_ = 2;
  if (len_ >= cells) len_ = cells - 1;

  // стартовая “горизонтальная” змейка
  Pt p{ (uint8_t)(w_ / 2), (uint8_t)(h_ / 2) };
  for (uint16_t i = 0; i < len_; i++) {
    uint8_t xx = (p.x >= i) ? (uint8_t)(p.x - i) : 0;
    snake_[i] = Pt{xx, p.y};
  }

  accMs_ = 0;
  spawnFood_();
}

void SnakeEffect::tick(Display& display, uint32_t dtMs) {
  if (len_ == 0) {
    begin(display);
    if (len_ == 0) return;
  }

  accMs_ += dtMs;
  while (accMs_ >= stepMs_) {
    accMs_ -= stepMs_;
    step_(display);
  }

  display.clear(false);

  if (hasFood_) {
    display.setPixelXY(food_.x, food_.y, foodColor_);
  }

  for (uint16_t i = 0; i < len_; i++) {
    display.setPixelXY(snake_[i].x, snake_[i].y, snakeColor_);
  }

  display.show();
}

void SnakeEffect::step_(Display& display) {
  Pt head = snake_[len_ - 1];
  Pt next = head;

  bool ok = false;

  if (hasFood_) {
    ok = findNextStepBfs_(head, food_, next);
  }

  if (!ok) {
    ok = findNextStepToTail_(head, next);
  }

  if (!ok) {
    ok = pickAnySafeNeighbor_(head, next);
  }

  if (!ok) {
    // вообще некуда идти — перезапуск
    begin(display);
    return;
  }

  // обычный шаг фиксированной длины: хвост уходит всегда
  for (uint16_t i = 0; i + 1 < len_; i++) {
    snake_[i] = snake_[i + 1];
  }
  snake_[len_ - 1] = next;

  // “съели” цель => просто переносим её, длина НЕ меняется
  if (hasFood_ && next.x == food_.x && next.y == food_.y) {
    spawnFood_();
  }
}

bool SnakeEffect::isOnSnake_(Pt p) const {
  for (uint16_t i = 0; i < len_; i++) {
    if (snake_[i].x == p.x && snake_[i].y == p.y) return true;
  }
  return false;
}

void SnakeEffect::spawnFood_() {
  uint16_t cells = (uint16_t)w_ * (uint16_t)h_;
  if (cells == 0 || len_ >= cells) {
    hasFood_ = false;
    return;
  }

  for (uint16_t tries = 0; tries < 800; tries++) {
    Pt p{ (uint8_t)random8(w_), (uint8_t)random8(h_) };
    if (!isOnSnake_(p)) {
      food_ = p;
      hasFood_ = true;
      return;
    }
  }

  hasFood_ = false;
}

// BFS: найти следующий шаг из start в goal
bool SnakeEffect::findNextStepBfs_(Pt start, Pt goal, Pt& outNext) {
  const uint16_t cells = (uint16_t)w_ * (uint16_t)h_;
  if (cells == 0) return false;

  const uint16_t s = idx_(start.x, start.y);
  const uint16_t g = idx_(goal.x, goal.y);

  // занятость
  bool blocked[MAX_CELLS];
  for (uint16_t i = 0; i < cells; i++) blocked[i] = false;

  for (uint16_t i = 0; i < len_; i++) {
    blocked[idx_(snake_[i].x, snake_[i].y)] = true;
  }

  // хвост считаем свободным (он уйдёт на этом ходу)
  Pt tail = snake_[0];
  blocked[idx_(tail.x, tail.y)] = false;

  // BFS структуры
  int16_t parent[MAX_CELLS];
  for (uint16_t i = 0; i < cells; i++) parent[i] = -1;

  uint16_t q[MAX_CELLS];
  uint16_t qh = 0, qt = 0;

  parent[s] = s;
  q[qt++] = s;

  auto push = [&](uint16_t v, uint16_t p) {
    if (parent[v] != -1) return;
    if (blocked[v]) return;
    parent[v] = (int16_t)p;
    q[qt++] = v;
  };

  while (qh < qt) {
    uint16_t v = q[qh++];
    if (v == g) break;

    Pt p = ptFromIdx_(v);

    if (p.x > 0)           push((uint16_t)(v - 1), v);
    if (p.x + 1 < w_)      push((uint16_t)(v + 1), v);
    if (p.y > 0)           push((uint16_t)(v - w_), v);
    if (p.y + 1 < h_)      push((uint16_t)(v + w_), v);
  }

  if (parent[g] == -1) return false;

  // восстановим первый шаг: идём от goal к start по parent
  uint16_t cur = g;
  while ((uint16_t)parent[cur] != s) {
    uint16_t p = (uint16_t)parent[cur];
    if (p == cur) break; // защита
    cur = p;
  }

  outNext = ptFromIdx_(cur);
  return true;
}

// fallback: путь к хвосту (чтобы не зависать)
bool SnakeEffect::findNextStepToTail_(Pt head, Pt& outNext) {
  Pt tail = snake_[0];
  return findNextStepBfs_(head, tail, outNext);
}

// совсем fallback: любой безопасный сосед
bool SnakeEffect::pickAnySafeNeighbor_(Pt head, Pt& outNext) {
  bool occ[MAX_CELLS];
  uint16_t cells = (uint16_t)w_ * (uint16_t)h_;
  for (uint16_t i = 0; i < cells; i++) occ[i] = false;
  for (uint16_t i = 0; i < len_; i++) occ[idx_(snake_[i].x, snake_[i].y)] = true;

  // хвост свободен (он уйдёт)
  Pt tail = snake_[0];
  occ[idx_(tail.x, tail.y)] = false;

  auto tryCell = [&](int nx, int ny) -> bool {
    if (nx < 0 || ny < 0 || nx >= (int)w_ || ny >= (int)h_) return false;
    uint16_t id = idx_((uint8_t)nx, (uint8_t)ny);
    if (occ[id]) return false;
    outNext = Pt{(uint8_t)nx, (uint8_t)ny};
    return true;
  };

  // порядок можно менять
  if (tryCell((int)head.x + 1, (int)head.y)) return true;
  if (tryCell((int)head.x - 1, (int)head.y)) return true;
  if (tryCell((int)head.x, (int)head.y + 1)) return true;
  if (tryCell((int)head.x, (int)head.y - 1)) return true;

  return false;
}
