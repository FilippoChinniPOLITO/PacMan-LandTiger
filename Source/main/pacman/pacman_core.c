#include "pacman_core.h"


/* (Private) Function Prototypes */


/* Global Variables and Costants Definitions */

const GameConfig GAME_CONFIG = {
	.map_id = 1,
	.max_time = 60,
	.start_lives = 1,
	.max_lives = 5,
	.pts_to_life = 1000,
	.std_pill_count = 240,
	.spc_pill_count = 6,
	.std_pill_value = 10,
	.spc_pill_value = 50
};

GameRunning game_run;

GameStatus game_status;


/* Function Implementations */

