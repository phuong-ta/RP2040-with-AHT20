#ifndef AHT20_H
#define AHT20_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SPEED 400000
#define AHT20_ADDRESS 0x38


class AHT20 {
public:
    AHT20(int sda_pin, int scl_pin);
    void triggerMeasurement();
    float getTemperature();
    float getHumidity();
    float getTemperature_f();
    void softReset();

private:
    float temperature;
    float humidity;
};

#endif // AHT20_H
