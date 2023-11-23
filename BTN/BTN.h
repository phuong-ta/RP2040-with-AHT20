#ifndef BTN_H
#define BTN_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

class BTN {
public:
    BTN(int pin);
    bool is_clicked();
private:
    uint pin_number;
    bool invert;
};

#endif // AHT20_H