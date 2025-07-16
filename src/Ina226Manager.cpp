#include "Ina226Manager.h"
#include "Globals.h"
#include "Config.h"

#include <Wire.h>
#include <INA226_WE.h>

namespace Ina226Manager {

// ОБЪЯВЛЯЕМ объект ina226 с типом!
INA226_WE ina226(INA226_I2C_ADDRESS);

void begin() {
  if (!ina226.init()) {
    // DisplayManager::showMessage("INA226 NOT FOUND");  // если надо
    delay(3000);
    return;
  }

  ina226.setAverage(AVERAGE_16);
  ina226.setConversionTime(CONV_TIME_1100);
  ina226.setMeasureMode(CONTINUOUS);
}

void update() {
  labV_meas = ina226.getBusVoltage_V();
  labI_meas = ina226.getCurrent_mA() / 1000.0f;
  labQ_meas = ina226.getBusPower() / 1000.0f;;
}

float getVoltage() { return labV_meas; }
float getCurrent() { return labI_meas; }
float getPower()   { return labQ_meas; }

} // namespace Ina226Manager
