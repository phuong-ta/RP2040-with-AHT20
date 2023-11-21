#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "AHT20/AHT20.h"

int main() {
    stdio_init_all();
    AHT20 aht20(16,17);
    aht20.triggerMeasurement();
    while (true) {
        printf("humidity: %.2f %\n", aht20.getHumidity());
        printf("temperature: %.2f\n", aht20.getTemperature());
        sleep_ms(2000);
    }
}
