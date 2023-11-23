#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "AHT20/AHT20.h"
#include "BTN/BTN.h"
#define SW0 8
int main() {
    stdio_init_all();
    BTN btn1(9); // BTN GPIO_PIN 9,8,7
    AHT20 aht20(16,17);
    aht20.triggerMeasurement();
    while (true) {
        if(btn1.is_clicked()){
            printf("humidity: %.2f\n", aht20.getHumidity());
            printf("temperature: %.2f\n", aht20.getTemperature());
        }
        sleep_ms(200);
    }
}
