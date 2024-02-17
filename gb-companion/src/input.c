
#include "input.h"

uint8_t read_button_state(void){
    *rP1 = P1F_GET_BTN;
    uint8_t btn_state = *rP1;
    btn_state = *rP1;
    btn_state = *rP1;
    return btn_state;
}
