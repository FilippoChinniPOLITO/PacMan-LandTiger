#include "pacman_core.h"


/* (Private) Function Prototypes */


/* Global Variables and Costants Definitions */

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


/* Function Implementations */

unsigned int speed_to_timer_count(unsigned char speed_value) {
	return seconds_to_timer_count(TICK_SPEED_CONV * ((TICK_SECOND_RATIO * 2) - speed_value));
}

unsigned int seconds_to_timer_count(float seconds) {
	return ((unsigned int) (seconds * SYSTEM_FREQUENCY));
}

unsigned char seconds_to_ticks(float seconds) {
	return ((unsigned char) (seconds / TICK_DURATION));
}

float ticks_to_seconds(unsigned char ticks) {
	return (((float) ticks) * TICK_DURATION);
}

