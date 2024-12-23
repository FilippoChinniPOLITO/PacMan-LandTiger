#include "pacman_init.h"


/* (Private) Function Prototypes */

void init_time_timer();
void init_pacman_timer();
void init_spc_pills_timer();


/* (Private) Constants Definitions */


/* Function Implementations */

void init_game() {
	game_run.lives = GAME_CONFIG.start_lives;
	game_run.score = 0;
	game_status.is_pause = 1;
	
	handle_level_init(1);
}

void init_game_timers() {
	init_time_timer();
	init_pacman_timer();
	init_spc_pills_timer();
}

void init_time_timer() {
	init_timer(1, 0, 0, 3, 0x017D7840);			//1s
	//init_timer(1, 0, 0, 3, 0x00017840);		//test simulator
	//init_timer(1, 0, 0, 3, 0x0017D784);		//test board
}

void init_pacman_timer() {
	if(!IS_SIMULATOR)
		init_timer(2, 0, 0, 3, 0x00319750);		//0.13s
	else
		init_timer(2, 0, 0, 3, 0x00225510);		//0.09s
		//init_timer(2, 0, 0, 3, 0x0019D771);
}

void init_spc_pills_timer() {
	init_timer(3, 0, 0, 3, 0x010B0760);			//0.7s
}


