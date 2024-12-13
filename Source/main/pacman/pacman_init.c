#include "pacman_init.h"


/* (Private) Function Prototypes */

void init_game_view();
void init_level();
void init_map_view();


/* (Private) Constants Definitions */


/* Function Implementations */

void init_game() {
	game_run.lives = GAME_CONFIG.start_lives;
	game_run.score = 0;
	game_run.is_pause = 1;
	
	init_level();
	init_game_view();
}

void init_game_view() {
	draw_stat_area();
}

void init_level() {
	game_run.pills_left = GAME_CONFIG.std_pill_count;
	game_run.time = GAME_CONFIG.max_time;
	
	get_game_map(&game_run.game_map, GAME_CONFIG.map_id);
	
	init_map_view();
}

void init_map_view() {
	draw_game_map();
}

