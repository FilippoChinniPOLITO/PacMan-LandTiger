#include "pacman_init.h"


/* (Private) Hardware Imports */

//#include "../interfaces/hw_abstraction.h"		// Transitive Import


/* (Private) User Imports */

#include "pacman_core.h"
#include "pacman_ctrl.h"
//#include "pacman_utils.h"		// Transitive Import


/* (Private) Functions Prototypes */

void init_main_timer();
void init_game_timings();
void init_blinky_timer();
unsigned short calculate_blinky_acceleration_steps();


/* Function Implementations */

void init_game() {
	init_game_timings();
	init_main_timer();
	init_blinky_timer();
	
	game_run.lives = GAME_CONFIG.start_lives;
	game_run.score = 0;
	game_status.is_pause = 1;
	
	game_status.blinky_curr_speed = GAME_CONFIG.blinky_min_speed;
	
	handle_level_init();
}

void init_main_timer() {
	HW_TIMER_init_timer(0, 0, 0, 3, GAME_TIMINGS.pacman_speed_timer_count);
}

void init_blinky_timer() {
	HW_TIMER_init_timer(1, 0, 0, 3, GAME_TIMINGS.blinky_speed_timer_count);
}

void init_game_timings() {
	GAME_TIMINGS.pacman_speed_timer_count = speed_to_timer_count(GAME_CONFIG.pacman_speed);
	GAME_TIMINGS.blinky_speed_timer_count = speed_to_timer_count(GAME_CONFIG.blinky_min_speed);
	GAME_TIMINGS.ghost_respawn_ticks = seconds_to_ticks(GAME_CONFIG.ghost_respawn_time);
	GAME_TIMINGS.ghost_weak_ticks = seconds_to_ticks(GAME_CONFIG.ghost_weak_time);
	GAME_TIMINGS.blinky_accel_interval_ticks = calculate_blinky_acceleration_steps();
}

unsigned short calculate_blinky_acceleration_steps() {
	const unsigned char accel_steps = (GAME_CONFIG.blinky_max_speed - GAME_CONFIG.blinky_min_speed) + 1;
	const unsigned short tot_time_ticks = seconds_to_ticks(GAME_CONFIG.max_time);

	return (tot_time_ticks / accel_steps);
}

