// TemperatureSensor.cpp
#include "AHT20.h"

AHT20::AHT20(int sda_pin, int scl_pin) {
    i2c_init(I2C_PORT, SPEED);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    sleep_ms(40); //Wait 40ms after power-on. Before reading the temperature and humidity values
    uint8_t status_cmd[] = {0x71};
    i2c_write_blocking(I2C_PORT, AHT20_ADDRESS, status_cmd, sizeof(status_cmd), false);
    uint8_t status;
    i2c_read_blocking(I2C_PORT, AHT20_ADDRESS, &status, sizeof(status), false);
    if ((status & 0x08) != 1) {
        uint8_t init_cmd[] = {0xBE, 0x08, 0x00};
        i2c_write_blocking(I2C_PORT, AHT20_ADDRESS, init_cmd, sizeof(init_cmd), false);
        sleep_ms(10);
    }
}

void AHT20::triggerMeasurement(){
    uint8_t trigger_cmd[] = {0xAC, 0x33, 0x00}; // Send the 0xAC command directly. The parameter of this command has two bytes, 1st is 0x33 and 2.nd is 0x00.
    uint8_t status; // result status
    uint8_t data[6]; // data array

    i2c_write_blocking(I2C_PORT, AHT20_ADDRESS, trigger_cmd, sizeof(trigger_cmd), false);
    sleep_ms(80); //Wait for 80ms to wait for the measurement to be completed.
    // If the read status word Bit [7] is 0, it indicates that the measurement is completed, and then six bytes can be read in a row; otherwise, continue to wait.
    i2c_read_blocking(I2C_PORT, AHT20_ADDRESS, &status, 1, false); // Read status byte

    // Wait for status to be ready before continuing
    while ((status & 0x80) != 0) {
        printf("Waiting for read to complete\n");
        i2c_read_blocking(I2C_PORT, AHT20_ADDRESS, &status, 1, false);
    }

    i2c_read_blocking(I2C_PORT, AHT20_ADDRESS, data, sizeof(data), false); // Read all data
    //uint8_t temperature_data, humidity_data;
    //humidity_data= (((data[1] << 12) | (data[2] << 4) | (data[3] & 0xF0)));
    //temperature_data = ((((data[3] & 0x0F) << 8) | (data[4] << 8) | data[5]));
    //humidity = ((float)humidity_data * 100) / 1048576;
    //temperature= ((float)temperature_data * 200 / 1048576) - 50; // 
    /*
    uint32_t humidity = data[1];
    humidity <<= 12;
    humidity |= data[2];
    humidity <<= 4;
    humidity |= data[3] & 0xF0;
    humidity = ((float)humidity * 100) / 1048576;
    */
    humidity = ((float)((data[1] << 12) | (data[2] << 4) | (data[3] & 0xF0))) / 1048576.0 * 100.0;
    uint32_t temp = data[3] & 0x0F;
    temp <<= 8;
    temp |= data[4];
    temp <<= 8;
    temp |= data[5];
    temperature = ((float)temp * 200 / 1048576) - 50;

}

float AHT20::getTemperature() {
    return temperature;
}

float AHT20::getTemperature_f() {
    return (temperature * 1.8) + 32;
}

float AHT20::getHumidity() {
    return humidity;
}



/*
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "AHT20.h"

#define I2C_PORT i2c0

AHT20::AHT20() {
}

void AHT20::init(int sdaPort, int sclPort) {
    sleep_ms(20); // Wait 20ms for sensor to power on and go to idle state

    i2c_init(I2C_PORT, 400000);
    gpio_set_function(sdaPort, GPIO_FUNC_I2C);
    gpio_set_function(sclPort,GPIO_FUNC_I2C);
    gpio_pull_up(sdaPort);
    gpio_pull_up(sclPort);

    sleep_ms(40); // Wait 40ms after power on
    while(checkCalibration() == false) {
        sensorInit();
    }
}

void AHT20::sensorInit() {
    uint8_t reg[3] = {0xBE, 0x08, 0x00};

    i2c_write_blocking(I2C_PORT, addr, reg, 3, false);
    sleep_ms(10); // Wait 10ms for initialisation
}

bool AHT20::checkCalibration() {
    uint8_t statusReg = 0x71;
    uint8_t cal; // Result byte
    i2c_write_blocking(I2C_PORT, addr, &statusReg, 1, false);
    i2c_read_blocking(I2C_PORT, addr, &cal, 1, false);

    return ((cal >> 4) & 1) == 1; // Check 4th bit is equal to 1
}

void AHT20::triggerMeasurement() {
    while(checkCalibration() == false) {
        sensorInit();
    }

    uint8_t measureCommand[3] = {0xAC, 0x33, 0x00};
    uint8_t status; // result status
    uint8_t data[6]; // data array

    i2c_write_blocking(I2C_PORT, addr, measureCommand, 3, false);
    sleep_ms(80); // Wait for measurement

    i2c_read_blocking(I2C_PORT, addr, &status,1, false); // Read status byte

    // Wait for status to be ready before continuing
    while (((status >> 8) & 1) != 0) {
        printf("Waiting for read to complete\n");
        i2c_read_blocking(I2C_PORT, addr, &status,1, false);
    }

    i2c_read_blocking(I2C_PORT, addr, data, 6, false); // Read all data

    // Convert data for reading and store in variable
    // Based on Adafruit methodology for Arduino
    uint32_t humidity = data[1];
    humidity <<= 8;
    humidity |= data[2];
    humidity <<= 4;
    humidity |= data[3] >> 4;
    humReading = ((float)humidity * 100) / 1048576;

    uint32_t temp = data[3] & 0x0F;
    temp <<= 8;
    temp |= data[4];
    temp <<= 8;
    temp |= data[5];
    tempReading = ((float)temp * 200 / 1048576) - 50;
}

float AHT20::getTemp() {
    return tempReading;
}

float AHT20::getTemp_f() {
    return (tempReading * 1.8) + 32;
}

float AHT20::getHumidity() {
    return humReading;
}
*/