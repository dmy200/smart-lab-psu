//DisplayManager.cpp
#include "DisplayManager.h"
#include "SegmentFont.h"
#include "Globals.h"

namespace DisplayManager {

// === Внешние переменные ===
// объявлены extern в заголовке, тут определяем
float smoothV = 0.0f;
float smoothI = 0.0f;
float smoothP = 0.0f;
const float alphaDisplay = 0.4f;

unsigned long confirmedDisplayUntil = 0;
bool confirmedState = false;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, SCL_PIN, SDA_PIN, U8X8_PIN_NONE);

bool displayError = false;
char errorMsg[32] = "";



// === Вспомогательные переменные для редактирования (тебе их добавить) ===
// предположим, что есть
float* editTarget = nullptr;
bool* editFlag = nullptr;
bool editingFuse = false;
bool editingValue = false;
bool editingFooter = false;


uint8_t currentMode = 0;

bool inSystemMode = false;
bool inCurrentSettings = false;

// === Массив с именами режимов ===
const char* const modeNames[] = {
  "Voltage",
  "Current"
};
const uint8_t modeCount = sizeof(modeNames) / sizeof(modeNames[0]);

using ::labV_meas;
using ::labI_meas;
using ::labQ_meas;
using ::labI_limit;
using ::labI_fuse;

// === Функции ===
void begin() {
    display.begin();
    display.clearBuffer();
    display.setFont(u8g2_font_6x12_tf);
    display.drawStr(0, 12, "Booting...");
    display.sendBuffer();
    delay(1000);
}

void updateDisplaySmoothing() {
  smoothV = alphaDisplay * labV_meas + (1.0f - alphaDisplay) * smoothV;
  smoothI = alphaDisplay * labI_meas + (1.0f - alphaDisplay) * smoothI;
  smoothP = alphaDisplay * labQ_meas + (1.0f - alphaDisplay) * smoothP;
}

ModeID getCurrentModeID() {
  if (currentMode < modeCount) {
    return (ModeID)currentMode;
  }
  return MODE_UNKNOWN;
}

const char* formatUnit(float val, const char* type) {
  if (strcmp(type, "VOLTAGE") == 0) return (val >= 1.0f) ? " V" : "mV";
  if (strcmp(type, "CURRENT") == 0) return (val >= 1.0f) ? " A" : "mA";
  if (strcmp(type, "POWER") == 0) return (val >= 1.0f) ? " W" : "mW";
  return "  ";
}

void drawHeader(const char* mode) {
  display.setFont(u8g2_font_6x12_tf);
  display.drawStr(0, 12, mode);
}

void drawValueWithUnit(float val, int xVal, int yVal, int h, int w, const char* type, int xUnit, int yUnit, int width) {
  String formatted = formatSegmentValue(val, width);
  drawSegmentStringSmart(formatted.c_str(), xVal, yVal, h, w, false, display);
  display.setFont(u8g2_font_5x8_tf);
  display.drawStr(xUnit, yUnit, formatUnit(val, type));
}

void drawFooterSmart(float il, float ifault) {
  display.setFont(u8g2_font_5x8_tf);
  display.drawStr(0, FOOT_UNIT_Y, "IL:");
  display.drawStr(60, FOOT_UNIT_Y, "IF:");

  drawSegmentStringSmart(formatSegmentValue(il, FOOT_VAL_WIDTH).c_str(), FOOT_IL_X, FOOT_VAL_Y, SMALL_H, SMALL_W, false, display);
  drawSegmentStringSmart(formatSegmentValue(ifault, FOOT_VAL_WIDTH).c_str(), FOOT_IF_X, FOOT_VAL_Y, SMALL_H, SMALL_W, false, display);

  display.drawStr(FOOT_UNIT_IL_X, FOOT_UNIT_Y, formatUnit(il, "CURRENT"));
  display.drawStr(FOOT_UNIT_IF_X, FOOT_UNIT_Y, formatUnit(ifault, "CURRENT"));
}

void renderEditFloat(float& target, const char* label) {
  display.setFont(u8g2_font_5x8_tf);
  display.drawStr(0, FOOT_UNIT_Y, label);

  String valueStr = formatSegmentValue(target, 4);
  drawSegmentStringSmart(valueStr.c_str(), MAIN_VAL_X, MAIN_VAL_Y, MAIN_H, MAIN_W, editingValue, display);

  display.drawStr(MAIN_UNIT_X, MAIN_UNIT_Y, formatUnit(target, "CURRENT"));

  display.setFont(u8g2_font_6x12_tf);

  if (displayError) {
    display.drawStr(0, FOOT_UNIT_Y, errorMsg);
  } else {
    if (editingFooter) {
      if (editingFuse) {
        display.drawStr(20, FOOT_UNIT_Y, "OK  CANCEL");
      } else {
        display.drawStr(20, FOOT_UNIT_Y, "OK  IF:");
      }
    } else {
      display.drawStr(20, FOOT_UNIT_Y, "OK");
    }
  }
}

void renderVoltage() {
  if (confirmedState && millis() < confirmedDisplayUntil) {
    drawSegmentStringSmart(formatSegmentValue(labV_meas, MAIN_VAL_WIDTH).c_str(), MAIN_VAL_X, MAIN_VAL_Y, MAIN_H, MAIN_W, false, display);
  } else {
    confirmedState = false;
    drawSegmentStringSmart(formatSegmentValue(smoothV, MAIN_VAL_WIDTH).c_str(), MAIN_VAL_X, MAIN_VAL_Y, MAIN_H, MAIN_W, false, display);
  }
  display.setFont(u8g2_font_6x12_tf);
  display.drawStr(MAIN_UNIT_X, MAIN_UNIT_Y, formatUnit(labV_meas, "VOLTAGE"));

  drawValueWithUnit(smoothI, LEFT_VAL_X, TOP_Y, LEFT_H, LEFT_W, "CURRENT", LEFT_UNIT_X, TOP_Y + 17, LEFT_VAL_WIDTH);
  drawValueWithUnit(smoothP, LEFT_VAL_X, BOTTOM_Y, LEFT_H, LEFT_W, "POWER", LEFT_UNIT_X, BOTTOM_Y + 17, LEFT_VAL_WIDTH);

  drawFooterSmart(labI_limit, labI_fuse);
}

void renderCurrent() {
  if (inCurrentSettings) {
    if (editingFuse) renderEditFloat(labI_fuse, "IF:");
    else renderEditFloat(labI_limit, "IL:");
  } else {
    if (confirmedState && millis() < confirmedDisplayUntil) {
      drawSegmentStringSmart(formatSegmentValue(labI_meas, MAIN_VAL_WIDTH).c_str(), MAIN_VAL_X, MAIN_VAL_Y, MAIN_H, MAIN_W, false, display);
    } else {
      confirmedState = false;
      drawSegmentStringSmart(formatSegmentValue(smoothI, MAIN_VAL_WIDTH).c_str(), MAIN_VAL_X, MAIN_VAL_Y, MAIN_H, MAIN_W, false, display);
    }
    display.setFont(u8g2_font_6x12_tf);
    display.drawStr(MAIN_UNIT_X, MAIN_UNIT_Y, formatUnit(labI_meas, "CURRENT"));

    drawValueWithUnit(smoothV, LEFT_VAL_X, TOP_Y, LEFT_H, LEFT_W, "VOLTAGE", LEFT_UNIT_X, TOP_Y + 17, LEFT_VAL_WIDTH);
    drawValueWithUnit(smoothP, LEFT_VAL_X, BOTTOM_Y, LEFT_H, LEFT_W, "POWER", LEFT_UNIT_X, BOTTOM_Y + 17, LEFT_VAL_WIDTH);

    drawFooterSmart(labI_limit, labI_fuse);
  }
}

void renderSystem() {
  display.setFont(u8g2_font_10x20_tf);
  display.drawStr(10, 30, "PRESS OK TO ENTER");
  display.setFont(u8g2_font_6x12_tf);
  display.drawStr(20, 50, "Config Mode");
}

void renderStub() {
  display.setFont(u8g2_font_fub20_tr);
  display.drawStr(0, 40, "...page...");
}

void update() {
  const unsigned long now = millis();
  if (now - lastDisplayUpdate < DISPLAY_UPDATE_INTERVAL) return;
  lastDisplayUpdate = now;

  updateDisplaySmoothing();
  display.clearBuffer();

  if (inSystemMode) {
    drawHeader("System");
    renderSystem();
  } else {
    ModeID mode = getCurrentModeID();
    const char* modeStr = (currentMode < modeCount) ? modeNames[currentMode] : "Unknown";
    drawHeader(modeStr);

    switch (mode) {
      case MODE_VOLTAGE: renderVoltage(); break;
      case MODE_CURRENT: renderCurrent(); break;
      default: renderStub(); break;
    }
  }
  display.sendBuffer();
}


void onEncoderRotate(int delta) {
  if (editingValue) {
    float step = 0.1f;
    if (editTarget && editFlag && *editFlag) {
      *editTarget += delta * step;
      *editTarget = constrain(*editTarget, 0.0f, 10.0f); // Примерный диапазон
    }
  } else {
    // Переключение главных экранов (напряжение ↔ ток)
    if (delta > 0) currentMode = (currentMode + 1) % modeCount;
    else currentMode = (currentMode + modeCount - 1) % modeCount;
  }
}

void onEncoderClick() {
  if (!editingValue) {
    beginEdit(editTarget, editFlag, false);
  } else {
    confirmEdit();
    confirmedState = true;
    confirmedDisplayUntil = millis() + 3000;  // Показываем подтверждение 3 секунды
  }
}

void onEncoderLongPress() {
  if (inCurrentSettings && editingValue) {
    // Переключение между labI_limit и labI_fuse
    editingFuse = !editingFuse;
    if (editTarget && editFlag)
      beginEdit(editingFuse ? &labI_fuse : &labI_limit, editFlag, editingFuse);
  } else {
    inSystemMode = !inSystemMode;
    editingValue = false;
  }
}

void onEncoderDoubleClick() {
  if (editingValue) {
    cancelEdit();
  }
}

void beginEdit(float* target, bool* flag, bool fuse) {
  editingValue = true;
  editTarget = target;
  editFlag = flag;
  editingFuse = fuse;
}

void confirmEdit() {
  editingValue = false;
  // Тут можно добавить логику подтверждения
}

void cancelEdit() {
  editingValue = false;
  // Логика отмены редактирования
}

void showMessage(const char* msg) {
  display.clearBuffer();
  display.setFont(u8g2_font_6x12_tf);
  display.drawStr(0, 30, msg);
  display.sendBuffer();
}


} // namespace DisplayManager
