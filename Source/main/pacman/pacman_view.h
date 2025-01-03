#ifndef __PACMAN_VIEW_H
#define __PACMAN_VIEW_H


/* (Public) User Imports */

#include "pacman_core.h"


/* (Public) Constants Definitions */

#define COL_WHITE	0xffff
#define COL_BLACK	0x0000
#define COL_YELLOW	0xffc0
#define COL_CYAN	0x07ff
#define COL_MAGENTA	0xf814
#define COL_RED		0xf840
#define COL_GREEN	0x07e2
#define COL_BLUE	0x00bf
#define COL_PINK	0xfefb
#define COL_ORANGE	0xfd43
#define COL_LIME	0xbfec
#define COL_OCHRE	0xff30


/* (Public) Functions Prototypes */

void draw_stat_area();
void draw_game_map();
void draw_stat_time(unsigned char time_value);
void draw_stat_score(unsigned short score_value);
void draw_stat_lives(unsigned char lives_value);
void draw_screen_pause();
void undraw_screen_pause();
void redraw_after_pause();
void draw_screen_victory();
void draw_screen_game_over();
void draw_screen_fail();
void draw_cell(CellType cell_type, Position pos);
void draw_pacman(Position pos);
void draw_blinky(Position pos);



#endif //__PACMAN_VIEW_H