#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include "hardware.h"

#define BTN_A P1F_0
#define BTN_B P1F_1

#define is_button_pressed(btn_state, btn_pattern) (((~btn_state) & (btn_pattern)) != 0)

uint8_t read_button_state(void);

#endif // INPUT_H