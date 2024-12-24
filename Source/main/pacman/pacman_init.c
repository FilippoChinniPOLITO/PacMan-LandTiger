#include "pacman_init.h"


/* (Private) Function Prototypes */

void init_main_timer();
void init_game_timings();
unsigned char calculate_blinky_acceleration_steps();


/* (Private) Constants Definitions */


/* Function Implementations */

void init_game() {
	init_game_timings();
	init_main_timer();
	
	game_run.lives = GAME_CONFIG.start_lives;
	game_run.score = 0;
	game_status.is_pause = 1;
	
	handle_level_init(1);
}

void init_main_timer() {
	init_timer(0, 0, 0, 3, GAME_TIMINGS.pacman_speed_timer_count);
}

void init_game_timings() {
	GAME_TIMINGS.pacman_speed_timer_count = speed_to_timer_count(GAME_CONFIG.pacman_speed);
	GAME_TIMINGS.blinky_speed_timer_count = speed_to_timer_count(GAME_CONFIG.blinky_min_speed);
	GAME_TIMINGS.blinky_respawn_ticks = seconds_to_ticks(GAME_CONFIG.blinky_respawn_time);
	GAME_TIMINGS.blinky_blue_ticks = seconds_to_ticks(GAME_CONFIG.blinky_blue_time);
	GAME_TIMINGS.blinky_accel_interval_ticks = calculate_blinky_acceleration_steps();
}

unsigned char calculate_blinky_acceleration_steps() {
	const unsigned char accel_steps = (GAME_CONFIG.blinky_max_speed - GAME_CONFIG.blinky_min_speed) + 1;
	const unsigned short tot_time_ticks = seconds_to_ticks((float) GAME_CONFIG.max_time);
	return (tot_time_ticks / (accel_steps+1));
}