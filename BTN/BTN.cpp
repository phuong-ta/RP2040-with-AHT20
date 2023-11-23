#include "BTN.h"

BTN::BTN(int pin) {
    pin_number = pin;
    invert = false;
    gpio_init(pin_number);
    gpio_set_dir(pin_number, GPIO_IN);
    gpio_pull_up(pin_number);
}

bool BTN::is_clicked() {
    static bool last_state = false;
    bool current_state = !gpio_get(pin_number);
    if (current_state != last_state) {
        last_state = current_state;
        return current_state != invert;
    }
    return false;
}