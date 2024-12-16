#ifndef __PACMAN_UTILS_H
#define __PACMAN_UTILS_H


/* System Imports */

#include <stdio.h>
#include <string.h>


/* User Imports */

#include "pacman_core.h"


/* Constants Definitions */


/* Global Variables */

//GameMap map_1;
//GameMap map_2;
//GameMap map_3;


/* (Public) Functions Prototypes */

void get_game_map(GameMap* map_to_init, const short map_id);
Position get_pacman_spawn(const short map_id);
void toggle_timer(uint8_t timer_num);
unsigned int get_timer_value (uint8_t timer_num);
unsigned int get_RIT_value();



#endif //__PACMAN_UTILS_H
