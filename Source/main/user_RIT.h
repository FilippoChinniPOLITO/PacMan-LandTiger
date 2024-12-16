#ifndef __USER_RIT_H
#define __USER_RIT_H


#include "./pacman/pacman_ctrl.h"


void interrupt_joystick_direction_update_up();
void interrupt_joystick_direction_update_down();
void interrupt_joystick_direction_update_right();
void interrupt_joystick_direction_update_left();

void interrupt_button_INT0_pause();



#endif //__USER_RIT_H