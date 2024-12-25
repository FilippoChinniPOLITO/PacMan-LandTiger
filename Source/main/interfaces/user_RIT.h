#ifndef __USER_RIT_H
#define __USER_RIT_H


/* (Public) Functions Prototypes */

void interrupt_joystick_direction_update_up();
void interrupt_joystick_direction_update_down();
void interrupt_joystick_direction_update_right();
void interrupt_joystick_direction_update_left();

void interrupt_button_INT0_pause();

void interrupt_speaker_play_full_sound();



#endif //__USER_RIT_H