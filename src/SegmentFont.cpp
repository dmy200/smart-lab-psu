#pragma once
#include <U8g2lib.h>
#include <Arduino.h>
#include "SegmentFont.h"

/*
  === SegmentFont.cpp === (v11.7)
  Ретро 7-сегментный шрифт для OLED с точкой, встроенной в предыдущий символ.
  -------------------------------------------------------------------------
  Использование:
    updateBlink(); // вызывать в loop()

    drawSegmentStringSmart("12.3", x, y, h, w, invert, u8g2);
    drawSegmentFloatSmart(-3.14, x, y, h, w, invert, leadZero, u8g2);
    drawSegmentBlink("Err", x, y, h, w, invert, blinkLen, u8g2);
    drawSegmentBlinkRange("36.00", x, y, h, w, invert, start, len, u8g2);
    formatSegmentValue(val, 3);

  Особенности:
    — Точка прикрепляется к символу слева, как на настоящем семисегментнике
    — Двоеточие ":" рисует две точки по центру символа (для часов)
    — Ширина символов постоянна, spacing зависит от ширины сегмента
*/

// === Вспомогательные переменные для мигания ===
bool blinkState = true;
unsigned long lastBlinkTime = 0;
uint16_t blinkInterval = 500;

// === Обновление состояния мигания (вызывать в loop) ===
void updateBlink() {
  if (millis() - lastBlinkTime >= blinkInterval) {
    blinkState = !blinkState;
    lastBlinkTime = millis();
  }
}

// === Таблица символов с их сегментной маской ===
const SegChar segmentChars[] = {
  {'0', SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F},
  {'1', SEG_B | SEG_C},
  {'2', SEG_A | SEG_B | SEG_G | SEG_E | SEG_D},
  {'3', SEG_A | SEG_B | SEG_C | SEG_D | SEG_G},
  {'4', SEG_F | SEG_G | SEG_B | SEG_C},
  {'5', SEG_A | SEG_F | SEG_G | SEG_C | SEG_D},
  {'6', SEG_A | SEG_F | SEG_G | SEG_E | SEG_C | SEG_D},
  {'7', SEG_A | SEG_B | SEG_C},
  {'8', SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G},
  {'9', SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G},
  {'A', SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G},
  {'b', SEG_C | SEG_D | SEG_E | SEG_F | SEG_G},
  {'C', SEG_A | SEG_D | SEG_E | SEG_F},
  {'c', SEG_G | SEG_E | SEG_D},
  {'d', SEG_B | SEG_C | SEG_D | SEG_E | SEG_G},
  {'E', SEG_A | SEG_D | SEG_E | SEG_F | SEG_G},
  {'F', SEG_A | SEG_E | SEG_F | SEG_G},
  {'L', SEG_D | SEG_E | SEG_F},
  {'P', SEG_A | SEG_B | SEG_E | SEG_F | SEG_G},
  {'U', SEG_B | SEG_C | SEG_D | SEG_E | SEG_F},
  {'r', SEG_E | SEG_G},
  {'n', SEG_C | SEG_E | SEG_G},
  {'o', SEG_C | SEG_D | SEG_E | SEG_G},
  {'-', SEG_G},
  {' ', 0},
  {'I', SEG_E | SEG_F},
};
// === Отрисовка одного сегмента по его индексу ===
void drawSegment(uint8_t seg, int x, int y, uint8_t h, uint8_t w, bool inv, U8G2& u8g2) {
  switch(seg) {
    case 0: u8g2.drawBox(x + w, y, h, w); break;
    case 1: u8g2.drawBox(x + w + h, y + w, w, h); break;
    case 2: u8g2.drawBox(x + w + h, y + w + h + w, w, h); break;
    case 3: u8g2.drawBox(x + w, y + 2 * h + 2 * w, h, w); break;
    case 4: u8g2.drawBox(x, y + w + h + w, w, h); break;
    case 5: u8g2.drawBox(x, y + w, w, h); break;
    case 6: u8g2.drawBox(x + w, y + h + w, h, w); break;
  }
}

// === Отрисовка двоеточия ===
void drawColon(int x, int y, uint8_t h, uint8_t w, bool inv, U8G2& u8g2) {
  int cy = y + h + w;
  int top = cy - w - 1;
  int bottom = cy + w + 1;
  u8g2.drawBox(x, top, w, w);
  u8g2.drawBox(x, bottom, w, w);
}

// === Отрисовка одного символа с возможной встроенной точкой ===
void drawCharSmart(char ch, bool withDot, int x, int y, uint8_t h, uint8_t w, bool inv, U8G2& u8g2) {
  char uc = (ch == 'c') ? 'c' : toupper(ch);
  for (const SegChar& sc : segmentChars) {
    if (sc.c == uc) {
      if (inv) u8g2.setDrawColor(0);
      for (uint8_t i = 0; i < 7; i++)
        if (sc.mask & (1 << i)) drawSegment(i, x, y, h, w, inv, u8g2);
      if (withDot) {
        const int spacing = w + 1;
        int dotX = x + h + 2 * w + spacing;
        int dotY = y + 2 * h + 2 * w;
        u8g2.drawBox(dotX, dotY, w, w);
      }
      if (inv) u8g2.setDrawColor(1);
      return;
    }
  }
}

// === Отрисовка строки в сегментном шрифте с поддержкой встроенной точки ===
void drawSegmentStringSmart(const char* str, int x, int y, uint8_t h, uint8_t w, bool inv, U8G2& u8g2) {
  const int charWidth = h + 2 * w;
  const int spacing = w + 1;

  while (*str) {
    if (*str == ':') {
      drawColon(x, y, h, w, inv, u8g2);
      x += charWidth + spacing;
      str++;
      continue;
    }

    bool hasDot = (*(str + 1) == '.');
    drawCharSmart(*str, hasDot, x, y, h, w, inv, u8g2);

    x += charWidth + spacing;
    if (hasDot) {
      x += spacing;
      str++;
    }
    str++;
  }
}

// === Отрисовка строки с миганием в выделенном диапазоне символов ===
void drawSegmentBlinkRange(const char* str, int x, int y, uint8_t h, uint8_t w, bool inv, uint8_t start, uint8_t len, U8G2& u8g2) {
  const int charWidth = h + 2 * w;
  const int spacing = w + 1;
  uint8_t i = 0;

  while (*str) {
    if (*str == ':') {
      if (!(i >= start && i < start + len && !blinkState))
        drawColon(x, y, h, w, inv, u8g2);
      x += charWidth + spacing;
      str++;
      i++;
      continue;
    }
    bool hasDot = (*(str + 1) == '.');
    bool inRange = (i >= start && i < start + len);
    bool show = !inRange || blinkState;
    if (show)
      drawCharSmart(*str, hasDot, x, y, h, w, inv, u8g2);

    x += charWidth + spacing;
    if (hasDot) {
      x += spacing;
      str++;
    }
    str++;
    i++;
  }
}




// === Форматирование числа для сегментного отображения с динамичной точкой ===
String formatSegmentValue(float val, int width) {
  if (width < 2 || width > 4) return "Err";

  if ((width == 3 && val >= 1000.0f) || (width == 4 && val >= 10000.0f)) return "----";

  char buf[10] = {0};

  if (width == 3) {
    if (val >= 100.0f) {
      snprintf(buf, sizeof(buf), "%3d", (int)(val + 0.5f));
      return String(buf);
    }
    if (val >= 10.0f) {
      float rounded = ((int)(val * 10.0f + 0.5f)) / 10.0f;
      snprintf(buf, sizeof(buf), "%3.1f", rounded);
      return String(buf);
    }
    if (val >= 1.0f) {
      float rounded = ((int)(val * 100.0f + 0.5f)) / 100.0f;
      snprintf(buf, sizeof(buf), "%3.2f", rounded);
      return String(buf);
    }
    // миллиамперы: до 999
    int mA = (int)(val * 1000.0f + 0.5f);
    if (mA <= 999) {
      if (mA >= 100) return String(mA);
      if (mA >= 10)  return String(" ") + String(mA);
      return String("  ") + String(mA);
    }
  }

  if (width == 4) {
    if (val >= 1000.0f) {
      snprintf(buf, sizeof(buf), "%4d", (int)(val + 0.5f));
      return String(buf);
    }
    if (val >= 100.0f) {
      float rounded = ((int)(val * 10.0f + 0.5f)) / 10.0f;
      snprintf(buf, sizeof(buf), "%4.1f", rounded);
      return String(buf);
    }
    if (val >= 10.0f) {
      float rounded = ((int)(val * 100.0f + 0.5f)) / 100.0f;
      snprintf(buf, sizeof(buf), "%4.2f", rounded);
      return String(buf);
    }
    if (val >= 1.0f) {
      float rounded = ((int)(val * 1000.0f + 0.5f)) / 1000.0f;
      snprintf(buf, sizeof(buf), "%4.3f", rounded);
      return String(buf);
    }
    // миллиамперы: до 999
    int mA = (int)(val * 1000.0f + 0.5f);
    if (mA <= 999) {
      if (mA >= 100) return String(mA);
      if (mA >= 10)  return String(" ") + String(mA);
      return String("  ") + String(mA);
    }
  }

  return "Err";
}








