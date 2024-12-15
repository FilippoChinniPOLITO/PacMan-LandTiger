#include "pacman_ctrl.h"


/* (Private) Function Prototypes */

void handle_game_over();


/* (Private) Constants Definitions */


/* Function Implementations */

void handle_time_update() {
	game_run.time--;
	draw_stat_time(game_run.time);
	
	if(game_run.time == 0) {
		handle_game_over();
	}
}

void handle_game_over() {
	if(game_run.lives == 0) {
		draw_screen_game_over();
		return;
	}
	
	game_run.lives--;
	draw_stat_lives(game_run.lives);
}



