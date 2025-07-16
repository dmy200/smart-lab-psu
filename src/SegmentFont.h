#pragma once

#include <Arduino.h>
#include <U8g2lib.h>  // нужен, потому что функции используют U8G2&

// === Биты 7-сегментного шрифта ===
enum SegmentBits {
  SEG_A = 1 << 0,
  SEG_B = 1 << 1,
  SEG_C = 1 << 2,
  SEG_D = 1 << 3,
  SEG_E = 1 << 4,
  SEG_F = 1 << 5,
  SEG_G = 1 << 6
};

// === Структура отображаемого символа ===
struct SegChar {
  char c;
  uint8_t mask;
};

// === Таблица символов ===
extern const SegChar segmentChars[];

// === Мигание сегментов ===
extern bool blinkState;
extern uint16_t blinkInterval;
void updateBlink();

// === Основные функции для рисования и форматирования ===
void drawSegmentStringSmart(const char* str, int x, int y, uint8_t h, uint8_t w, bool inv, U8G2& u8g2);

String formatSegmentValue(float val, int width = 3);

