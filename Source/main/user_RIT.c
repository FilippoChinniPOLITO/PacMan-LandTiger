#include "user_RIT.h"


//	DIRECTION_STILL	= 0,
//	DIRECTION_RIGHT	= 1,
//	DIRECTION_LEFT	= 2,
//	DIRECTION_UP	= 3,
//	DIRECTION_DOWN	= 4
//THIS IS BAD, but there are not better solutions without duplication of the Direction enum

void interrupt_joystick_direction_update_up() {
	handle_direction_change(3);
}

void interrupt_joystick_direction_update_down() {
	handle_direction_change(4);
}

void interrupt_joystick_direction_update_right() {
	handle_direction_change(1);
}

void interrupt_joystick_direction_update_left() {
	handle_direction_change(2);
}


void interrupt_button_INT0_pause() {
	handle_pause();
}


void interrupt_speaker_play_full_sound() {
	handle_play_full_sound();
}


