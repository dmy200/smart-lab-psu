// main.ino — Умный ЛБП V7.0

#include <WiFi.h>
#include <Wire.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Config.h"
#include "Secrets.h"
#include "Globals.h"

#include "EncoderManager.h"
#include "DisplayManager.h"
#include "Ina226Manager.h"
#include "PowerControl.h"
#include "LbpWebInterface.h"
#include "EepromManager.h"

// Глобальная структура настроек — объявляем здесь, чтобы видна везде
LabSettings settings;

void setup() {
  // Базовая инициализация
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Wire.begin(SDA_PIN, SCL_PIN);

  EepromManager_begin();
  EepromManager_load(settings);

  labI_limit = settings.i_limit;
  labI_fuse  = settings.i_fuse;

  EncoderManager::begin();
  DisplayManager::begin();
  Ina226Manager::begin();
  PowerControl::begin();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  ArduinoOTA.setHostname("lbp");
  ArduinoOTA.begin();

  LbpWebInterface::begin();

  DisplayManager::showMessage("Booting...");
}


void loop() {
  ArduinoOTA.handle();
  EncoderManager::update();
  Ina226Manager::update();
  PowerControl::update();
  LbpWebInterface::update();
  DisplayManager::update();
}

