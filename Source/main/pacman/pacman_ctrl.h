#ifndef __PACMAN_CTRL_H
#define __PACMAN_CTRL_H


/* (Public) User Imports */

#include "pacman_core.h"


/* (Public) Functions Prototypes */

void handle_level_init();
void handle_pause();
void handle_direction_change(Direction new_direction);
void handle_next_tick();
void handle_blinky_move();

void handle_play_full_sound();
void handle_play_single_sound();
void handle_play_single_sound_duration();



#endif //__PACMAN_CTRL_H