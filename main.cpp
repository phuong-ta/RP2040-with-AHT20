#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define AHT20_ADDRESS 0x38

void aht20_init(i2c_inst_t *i2c) {
    uint8_t init_cmd[] = {0xBE, 0x08, 0x00};
    i2c_write_blocking(i2c, AHT20_ADDRESS, init_cmd, sizeof(init_cmd), false);
}

void aht20_read(i2c_inst_t *i2c, float *temperature, float *humidity) {
    uint8_t read_cmd[] = {0xAC, 0x33, 0x00};
    i2c_write_blocking(i2c, AHT20_ADDRESS, read_cmd, sizeof(read_cmd), false);
    sleep_ms(75);
    uint8_t data[6];
    i2c_read_blocking(i2c, AHT20_ADDRESS, data, sizeof(data), false);
    *humidity = ((float)((data[1] << 12) | (data[2] << 4) | (data[3] & 0xF0))) / 1048576.0 * 100.0;
    *temperature = ((float)(((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5])) / 1048576.0 * 200.0 - 50.0;
}

int main() {
    stdio_init_all();
    i2c_inst_t *i2c = i2c0;
    i2c_init(i2c, 400000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);
    aht20_init(i2c);
    while (true) {
        float temperature, humidity;
        aht20_read(i2c, &temperature, &humidity);
        printf("Temperature: %.2f C, Humidity: %.2f %%\n", temperature, humidity);
        sleep_ms(2000);
    }
}
