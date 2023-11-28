#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"

#include "AHT20/AHT20.h"
#include "BTN/BTN.h"
#include "BUZZER/Buzzer.h"



#define SW0 8
#define BUZZER_PIN 15  // Change this to the GPIO pin you are using
#define WIFI_SSID "DNA-WLAN-2G-8008"
#define WIFI_PASS "60846109170"

void connect_wifi(){
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_FINLAND)) {
        printf("failed to initialise\n");
    }
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
    } else {
        printf("Connected.\n");
    }
}

int main() {
    stdio_init_all();
    Buzzer buzzer(14);

    // Play the melody using the Buzzer class
    //buzzer.playMelody(notes, durations, melody_length);
    BTN btn1(9); // BTN GPIO_PIN 9,8,7
    AHT20 aht20(16,17);
    aht20.triggerMeasurement();
    //buzzer_init();
    connect_wifi();

    while (true) {

        if(btn1.is_clicked()){
            printf("humidity: %.2f\n", aht20.getHumidity());
            printf("temperature: %.2f\n", aht20.getTemperature());
        }

        buzzer.playTone();
        sleep_ms(1000);
    }
}
