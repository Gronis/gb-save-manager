#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include "hardware.h"

#define BTN_A       P1F_0
#define BTN_B       P1F_1
#define BTN_SELECT  P1F_2
#define BTN_START   P1F_3

#define BTN_RIGHT   P1F_0
#define BTN_LEFT    P1F_1
#define BTN_UP      P1F_2
#define BTN_DOWN    P1F_3

#define is_button_pressed(btn_state, btn_pattern) (((~btn_state) & (btn_pattern)) != 0)

uint8_t read_button_state(void);

#endif // INPUT_H