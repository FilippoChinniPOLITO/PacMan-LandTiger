# include "user_timer.h"


void interrupt_timer_1_update_time() {
	handle_time_update();
}

void interrupt_timer_2_next_tick() {
	handle_next_tick();
}

void interrupt_timer_3_spc_pill_gen() {
	handle_special_pill_generation();
}

