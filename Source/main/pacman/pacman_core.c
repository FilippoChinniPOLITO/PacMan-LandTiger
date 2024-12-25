#include "pacman_core.h"


/* (Public) Global Variables Definitions */

const GameConfig GAME_CONFIG = {
	.map_id = 2,
	.max_time = 60,
	.start_lives = 1,
	.max_lives = 5,
	.pts_to_life = 1000,
	.std_pill_count = 240,
	.spc_pill_count = 6,
	.std_pill_value = 10,
	.spc_pill_value = 50,
	.pacman_speed = PACMAN_SPEED,
	.blinky_min_speed = 4,
	.blinky_max_speed = 7,
	.spc_pill_gen_ratio = 3,
	.blinky_respawn_time = 3,
	.blinky_blue_time = 10
};

GameTimings GAME_TIMINGS;

GameRunning game_run;

GameStatus game_status;


/* (Private) Function Prototypes */


/* Functions Implementations */

