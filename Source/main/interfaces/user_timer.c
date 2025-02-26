# include "user_timer.h"


/* (Private) User Imports */

#include "../pacman/pacman_ctrl.h"


/* Functions Implementations */

void interrupt_timer_0_next_tick() {
	handle_next_tick();
}

void interrupt_timer_1_move_blinky() {
	handle_blinky_move();
}

void interrupt_timer_2_play_sound() {
	handle_play_single_sound();
}

void interrupt_timer_3_play_sound_duration() {
	handle_play_single_sound_duration();
}

