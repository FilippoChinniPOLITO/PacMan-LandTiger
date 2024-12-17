#include "pacman_init.h"


/* (Private) Function Prototypes */


/* (Private) Constants Definitions */


/* Function Implementations */

void init_game() {
	game_run.lives = GAME_CONFIG.start_lives;
	game_run.score = 0;
	game_status.is_pause = 1;
	
	handle_level_init(1);
}


