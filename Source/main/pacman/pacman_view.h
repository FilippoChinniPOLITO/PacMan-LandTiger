#ifndef __PACMAN_VIEW_H
#define __PACMAN_VIEW_H


/* System Imports */


/* Hardware Imports */

#include "../GLCD/GLCD.h"


/* User Imports */

#include "pacman_core.h"


/* Constants Definitions */

#define COL_WHITE	0xffff
#define COL_BLACK	0x0000
#define COL_YELLOW	0xffc0
#define COL_CYAN	0x07ff
#define COL_MAGENTA	0xf814
#define COL_RED		0xf840
#define COL_GREEN	0x07e2
#define COL_BLUE	0x00bf
#define COL_PINK	0xfefb


/* Type Definitions */


/* (Public) Functions Prototypes */

void draw_stat_area();
void draw_game_map();
void draw_move_pacman();



#endif //__PACMAN_VIEW_H
