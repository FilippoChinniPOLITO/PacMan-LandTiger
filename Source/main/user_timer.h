#ifndef __USER_TIMER_H
#define __USER_TIMER_H


#include "./pacman/pacman_ctrl.h"


void interrupt_timer_1_update_time();
void interrupt_timer_2_next_tick();
void interrupt_timer_3_spc_pill_gen();



#endif //__USER_TIMER_H