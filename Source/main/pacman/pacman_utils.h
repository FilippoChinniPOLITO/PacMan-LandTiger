#ifndef __PACMAN_UTILS_H
#define __PACMAN_UTILS_H


/* System Imports */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/* User Imports */

#include "pacman_core.h"


/* Constants Definitions */

#define MAPS_FILE "./pacman_maps.txt"


/* (Public) Functions Prototypes */

void parse_map_file(const char *filename, GameMap game_map, short id);
	


#endif //__PACMAN_UTILS_H
