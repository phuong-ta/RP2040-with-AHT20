#include "Buzzer.h"

Buzzer::Buzzer(uint pin){
    this->buzzerPin = pin;
    uint slice_num = pwm_gpio_to_slice_num(this->buzzerPin);
    pwm_config config = pwm_get_default_config();
    pwm_set_enabled(slice_num, false);
    pwm_config_set_clkdiv_int(&config, 125.0f);
    pwm_config_set_wrap(&config, 1.0f / 440.0); // Set PWM period
    pwm_init(slice_num, &config,false);
    gpio_set_function(this->buzzerPin, GPIO_FUNC_PWM);
    pwm_set_enabled(slice_num, true);
}

void Buzzer::playTone() {
    pwm_set_gpio_level(buzzerPin, 1); // Start PWM
    sleep_ms(500);
    pwm_set_gpio_level(buzzerPin, 0); // Stop PWM
    sleep_ms(50); // A small delay for stability
}
