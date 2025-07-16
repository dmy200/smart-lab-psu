// Config.h
#pragma once

// === Пины ===
#define SDA_PIN         3
#define SCL_PIN         7
#define LED_PIN         15
#define ENC_CLK         4
#define ENC_DT          2
#define ENC_SW          1

#define MOSFET_OUT_PIN  6   // Управление выходным MOSFET
#define PWM_PIN         5   // ШИМ на нижнее плечо обратной связи
#define NTC_ADC_PIN     8   // АЦП-канал для чтения термистора

// === Настройки ina226 ===
#define INA226_I2C_ADDRESS     0x40
#define SHUNT_RESISTANCE_OHMS  0.01f

// === Параметры термистора NTC ===
#define NTC_NOMINAL_RES      10000.0f     // Номинальное сопротивление, Ом (при 25°C)
#define NTC_BETA_COEFF       3470.0f      // B-коэффициент термистора
#define NTC_SERIES_RESISTOR  10000.0f     // Сопротивление подтягивающего резистора, Ом
#define NTC_NOMINAL_TEMP_C   25.0f        // Номинальная температура, °C
