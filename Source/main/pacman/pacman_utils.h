#ifndef __PACMAN_UTILS_H
#define __PACMAN_UTILS_H


/* (Public) User Imports */

#include "pacman_core.h"


/* (Public) Functions Prototypes */

void get_game_map(GameMap* map_to_init, const unsigned char map_id);
Position get_pacman_spawn(const unsigned char map_id);
Position get_blinky_spawn(const unsigned char map_id);

unsigned int speed_to_timer_count(unsigned char speed_value);
unsigned int seconds_to_timer_count(float seconds);
unsigned short seconds_to_ticks(unsigned short seconds);
float ticks_to_seconds(unsigned char ticks);



#endif //__PACMAN_UTILS_H