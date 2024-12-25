#include "user_ADC.h"


/* (Private) User Imports */

#include "../pacman/pacman_sound.h"


/* Functions Implementations */

void interrupt_ADC_change_volume(unsigned short value) {
	handle_change_volume(value);
}

