#ifndef __PACMAN_CTRL_H
#define __PACMAN_CTRL_H


/* System Imports */

#include <stdlib.h>


/* Hardware Imports */

#include "../CAN/CAN.h"


/* User Imports */

#include "pacman_core.h"
#include "pacman_view.h"
#include "pacman_utils.h"
#include "pacman_sound.h"


/* Constants Definitions */


/* Type Definitions */



/* (Public) Functions Prototypes */

void handle_level_init(unsigned short is_first_init);
void handle_pause();
void handle_direction_change(Direction new_direction);
void handle_next_tick();

void handle_play_full_sound();
void handle_play_single_sound();
void handle_play_single_sound_duration();



#endif //__PACMAN_CTRL_H