// Buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"


class Buzzer {
    public:
        Buzzer(uint pin);
        void playTone();

    private:
        uint buzzerPin;};

#endif // BUZZER_H