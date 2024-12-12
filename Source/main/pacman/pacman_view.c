#include "pacman_view.h"


/* (Private) Function Prototypes */


/* (Private) Constants Definitions */


/* Function Implementations */

void draw_stat_area() {
	GUI_Text(TIMER_AREA_START_X, 0, (uint8_t*) " COUNTDOWN ", White, Black);
	GUI_Text(SCORE_AREA_START_X, 0, (uint8_t*) " SCORE ", White, Black);
	GUI_Text(LIVES_AREA_START_X, 0, (uint8_t*) " LIVES ", White, Black);
}

void draw_game_map() {

}