#ifndef __USER_TIMER_H
#define __USER_TIMER_H


/* (Public) Functions Prototypes */

void interrupt_timer_0_next_tick();
void interrupt_timer_1_move_blinky();
void interrupt_timer_2_play_sound();
void interrupt_timer_3_play_sound_duration();

//TIMER0: Time, Respawn Blinky, Vulnerability Blinky, Generation Spc Pills, Pacman Movement
//TIMER1: Blinky Movement
//TIMER2: Sound
//TIMER3: Sound Duration



#endif //__USER_TIMER_H