// LbpWebInterface.h — версия 1.2

#pragma once
#include <Arduino.h>

// HTML-шаблоны
extern const char index_html[] PROGMEM;
extern const char settings_html[] PROGMEM;
extern const char system_html[] PROGMEM;

namespace LbpWebInterface {
  void begin();
  void update();
}