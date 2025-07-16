//DisplayManager.h
#pragma once

#include <U8g2lib.h>
#include <Arduino.h>
#include "SegmentFont.h"
#include "Globals.h"
#include "Config.h"

namespace DisplayManager {

// === Внешние переменные ===
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C display;
extern uint8_t currentMode;
extern bool inSystemMode;
extern bool inCurrentSettings;
extern bool editingFuse;
extern bool editingValue;
extern bool editingFooter;
extern bool displayError;
extern char errorMsg[32];

// === Сглаженные значения ===
extern float smoothV;
extern float smoothI;
extern float smoothP;
extern const float alphaDisplay;

// === Режимы ===
enum ModeID {
  MODE_VOLTAGE,
  MODE_CURRENT,
  MODE_UNKNOWN
};

extern const char* const modeNames[];
extern const uint8_t modeCount;

ModeID getCurrentModeID();

// === КООРДИНАТЫ И РАЗМЕРЫ ===
constexpr int MAIN_VAL_X      = 47;
constexpr int MAIN_VAL_Y      = 16;
constexpr int MAIN_H          = 14;
constexpr int MAIN_W          = 2;
constexpr int MAIN_UNIT_X     = 110;
constexpr int MAIN_UNIT_Y     = 50;
constexpr int LEFT_H          = 7;
constexpr int LEFT_W          = 1;
constexpr int LEFT_VAL_X      = 0;
constexpr int LEFT_UNIT_X     = 34;
constexpr int TOP_Y           = 16;
constexpr int BOTTOM_Y        = 36;
constexpr int FOOT_IL_X       = 18;
constexpr int FOOT_IF_X       = 78;
constexpr int FOOT_VAL_Y      = 55;
constexpr int FOOT_UNIT_IL_X  = 40;
constexpr int FOOT_UNIT_IF_X  = 100;
constexpr int FOOT_UNIT_Y     = 64;
constexpr int SMALL_H         = 3;
constexpr int SMALL_W         = 1;

constexpr int MAIN_VAL_WIDTH  = 3;
constexpr int LEFT_VAL_WIDTH  = 3;
constexpr int FOOT_VAL_WIDTH  = 3;

// === Функции ===
const char* formatUnit(float val, const char* type);

void begin();
void updateDisplaySmoothing();
void drawHeader(const char* mode);
void drawValueWithUnit(float val, int xVal, int yVal, int h, int w,
                      const char* type, int xUnit, int yUnit, int width);
void drawFooterSmart(float il, float ifault);
void renderEditFloat(float& target, const char* label);
void renderCurrent();
void renderVoltage();
void renderSystem();
void renderStub();
void update();

// === Управление энкодером ===
void onEncoderRotate(int delta);
void onEncoderClick();
void onEncoderLongPress();
void onEncoderDoubleClick();

void beginEdit(float* target, bool* flag, bool fuse);
void confirmEdit();
void cancelEdit();

void showMessage(const char* msg);

} // namespace DisplayManager
