#include "pacman_ctrl.h"


/* (Private) System Imports */

#include <stdlib.h>


/* (Private) Hardware Imports */

//#include "../interfaces/hw_abstraction.h"		// Transitive Import


/* (Private) User Imports */

#include "pacman_core.h"
#include "pacman_view.h"
#include "pacman_utils.h"
#include "pacman_sound.h"


/* (Private) Functions Prototypes */

void handle_time_update();
void handle_fail();
void handle_game_over();
void handle_draw_pause();
void handle_pacman_move();
void handle_pacman_update(Pacman* pacman, Position next_pos, unsigned char* prev_cell, unsigned char* next_cell, unsigned char is_collision);
void update_pacman_animation();
Position calculate_next_position(Pacman pacman);
unsigned char handle_tp(Position* pos, Direction direction);
void handle_score_update(CellType pill_type);
void handle_lives_update();
void handle_special_pill_generation();
void handle_pill_transformation(Position pos);
Position generate_random_position();
void handle_draw_stat_time(unsigned char time_value);
void handle_draw_stat_score(unsigned short score_value);
void handle_draw_stat_lives(unsigned char lives_value);
void handle_CAN_send();


/* Functions Implementations */

void handle_level_init(unsigned short is_first_init) {
	game_status.spc_pills_gen = 0;
	game_run.pills_left = GAME_CONFIG.std_pill_count;
	game_run.time = GAME_CONFIG.max_time;
	
	get_game_map(&game_run.game_map, GAME_CONFIG.map_id);
	game_run.pacman.direction = DIRECTION_STILL;
	game_run.pacman.prev_pos = game_run.pacman.curr_pos;
	game_run.pacman.curr_pos = get_pacman_spawn(GAME_CONFIG.map_id);
	
	if(is_first_init) {
		draw_game_map();
		draw_stat_area();
		handle_draw_pause();
	}
	else {
		handle_draw_stat_time(game_run.time);
	}
}

void handle_next_tick() {	
	static unsigned short tick_count;
	static unsigned char blinky_respawn_tick_count;
	static unsigned char blinky_blue_tick_count;
	
	tick_count++;
	
	if(game_status.is_blinky_blue) {
		blinky_blue_tick_count++;
		if(blinky_blue_tick_count >= GAME_TIMINGS.blinky_blue_ticks) {
			blinky_blue_tick_count = 0;
			// TODO turn blinky back to normal
		}
	}
	else if(game_status.is_blinky_dead) {
		blinky_respawn_tick_count++;
		if(blinky_respawn_tick_count >= GAME_TIMINGS.blinky_respawn_ticks) {
			blinky_respawn_tick_count = 0;
			// TODO revive blinky
		}
	}
	
	handle_pacman_move();
	
	if((tick_count % TICK_SECOND_RATIO) == 0) {
		handle_time_update();
	}
	
	if((tick_count % GAME_CONFIG.spc_pill_gen_ratio) == 0) {
		handle_special_pill_generation();
	}
	
	if((tick_count % GAME_TIMINGS.blinky_accel_interval_ticks) == 0) {
		// TODO handle_blinky_acceleration();
	}
}

void handle_time_update() {
	game_run.time--;
	handle_draw_stat_time(game_run.time);
	
	if(game_run.time == 0) {
		handle_fail();
	}
}

void handle_fail() {
	if(game_run.lives == 0) {
		handle_game_over();
		return;
	}
	
	HW_TIMER_toggle_timers(1, 1, 0, 0);
	
	game_status.is_fail = 1;
	draw_cell(CELL_EMPTY, game_run.pacman.curr_pos);
	draw_screen_fail();
	
	game_run.lives--;
	handle_lives_update();
	handle_level_init(0);
}

void handle_game_over() {
	game_status.is_end = 1;
	HW_TIMER_toggle_timers(1, 1, 0, 0);
	draw_screen_game_over();
}

void handle_victory() {
	if(game_run.pills_left == 0) {
		game_status.is_end = 1;
		HW_TIMER_toggle_timers(1, 1, 0, 0);
		draw_screen_victory();
	}
}

void handle_pause() {
	if(game_status.is_end) {
		return;
	}
	
	HW_TIMER_toggle_timers(1, 1, 0, 0);
	
	if(game_status.is_fail) {
		game_status.is_fail = 0;
		undraw_screen_pause();
		draw_game_map();
		return;
	}		

	game_status.is_pause = !game_status.is_pause;
	handle_draw_pause();
}

void handle_draw_pause() {
	if(game_status.is_pause) {
		draw_screen_pause();
	}
	else {
		undraw_screen_pause();
		redraw_after_pause();
	}
}

void handle_pacman_move() {
	Position next_pos = calculate_next_position(game_run.pacman);
	unsigned char* prev_cell = &game_run.game_map[game_run.pacman.curr_pos.y][game_run.pacman.curr_pos.x];
	unsigned char* next_cell = &game_run.game_map[next_pos.y][next_pos.x];
	
	switch(*next_cell) {
		case(CELL_EMPTY):
		case(CELL_TP_L):
		case(CELL_TP_R):
			handle_pacman_update(&game_run.pacman, next_pos, prev_cell, next_cell, 0);
			break;
		case(CELL_WALL):
			handle_pacman_update(&game_run.pacman, next_pos, prev_cell, next_cell, 1);
			break;
		case(CELL_STD_PILL):
		case(CELL_SPC_PILL):
			handle_pacman_update(&game_run.pacman, next_pos, prev_cell, next_cell, 0);
			
			handle_score_update(*next_cell);
			handle_lives_update();
			handle_victory();
		
			*next_cell = CELL_EMPTY;
			break;
		case(CELL_PACMAN):
			//impossible
			break;
	}
}

void handle_pacman_update(Pacman* pacman, Position next_pos, unsigned char* prev_cell, unsigned char* next_cell, unsigned char is_collision) {
	pacman->prev_pos = pacman->curr_pos;
	
	if(!is_collision)
		pacman->curr_pos = next_pos;
	
	update_pacman_animation();
	
	draw_cell(*prev_cell, pacman->prev_pos);
	draw_cell(CELL_PACMAN, pacman->curr_pos);
}

void update_pacman_animation() {
	if(++game_run.pacman.animation_frame > 3)
		game_run.pacman.animation_frame = 0;
}

Position calculate_next_position(Pacman pacman) {
	Position next_pos = pacman.curr_pos;
	
	switch(pacman.direction) {
		case(DIRECTION_STILL):
			break;
		case(DIRECTION_RIGHT):
			if(handle_tp(&next_pos, pacman.direction))	break;
			next_pos.x++;
			break;
		case(DIRECTION_LEFT):
			if(handle_tp(&next_pos, pacman.direction))	break;
			next_pos.x--;
			break;
		case(DIRECTION_UP):
			next_pos.y--;
			break;
		case(DIRECTION_DOWN):
			next_pos.y++;
			break;		
	}
	return next_pos;
}

unsigned char handle_tp(Position* pos, Direction direction) {
	unsigned char* temp = &game_run.game_map[pos->y][pos->x];
	
	if((*temp == CELL_TP_L) && (direction == DIRECTION_LEFT)) {
		pos->x = MAP_WIDTH-1;
		return 1;
	}
	if((*temp == CELL_TP_R) && (direction == DIRECTION_RIGHT)) {
		pos->x = 0;
		return 1;
	}
	
	return 0;
}

void handle_direction_change(Direction new_direction) {
	game_run.pacman.direction = new_direction;
}

void handle_score_update(CellType pill_type) {
	if(pill_type == CELL_STD_PILL) {
		game_run.score += GAME_CONFIG.std_pill_value;
		game_run.pills_left--;
	}
	else if(pill_type == CELL_SPC_PILL) {
		game_run.score += GAME_CONFIG.spc_pill_value;
		game_run.pills_left--;
	}
	
	handle_draw_stat_score(game_run.score);
}

void handle_lives_update() {
	static unsigned char increases;
	if(game_run.score >= ((increases+1) * GAME_CONFIG.pts_to_life)) {
		increases++;
		if(game_run.lives < GAME_CONFIG.max_lives) {
			game_run.lives++;
		}
	}
	
	handle_draw_stat_lives(game_run.lives);
}

void handle_special_pill_generation() {
	const unsigned char PROBABILITY_TRESHOLD = 60;	//60%
	unsigned char temp;
	
	if(game_status.spc_pills_gen >= GAME_CONFIG.spc_pill_count) {
		return;
	}
	
	temp = (HW_TIMER_get_timer_value(2) ^ HW_TIMER_get_timer_value(3) ^ HW_RIT_get_RIT_value()) % 101;
	if(temp > PROBABILITY_TRESHOLD) {
		return;
	}
	
	Position rand_pos = generate_random_position();
	if(game_run.game_map[rand_pos.y][rand_pos.x] == CELL_STD_PILL) {
		game_run.game_map[rand_pos.y][rand_pos.x] = CELL_SPC_PILL;
		handle_pill_transformation(rand_pos);
		game_status.spc_pills_gen++;
	}
}

void handle_pill_transformation(Position pos) {
	draw_cell(CELL_SPC_PILL, pos);
}

Position generate_random_position() {
	srand(HW_TIMER_get_timer_value(2) ^ HW_TIMER_get_timer_value(3) ^ HW_RIT_get_RIT_value() ^ 1103515245);
	return (Position) {.y = (rand() % MAP_HEIGTH), .x = (rand() % MAP_WIDTH)};
}


void handle_play_full_sound() {
	play_full_sound();
}

void handle_play_single_sound() {
	play_single_sound();
}

void handle_play_single_sound_duration() {
	play_single_sound_duration();
}


void handle_draw_stat_time(unsigned char time_value) {
	if(IS_SIMULATOR) {	// No CAN in Simulator
		draw_stat_time(time_value);
	}
	else {
		handle_CAN_send();
	}
}

void handle_draw_stat_score(unsigned short score_value) {
	if(IS_SIMULATOR) {	// No CAN in Simulator
		draw_stat_score(score_value);
	}
	else {
		handle_CAN_send();
	}
}
void handle_draw_stat_lives(unsigned char lives_value) {
	if(IS_SIMULATOR) {	// No CAN in Simulator
		draw_stat_lives(lives_value);
	}
	else {
		handle_CAN_send();
	}
}

void handle_CAN_send() {
	HW_CAN_send_pacman_stats(game_run.time, game_run.score, game_run.lives);
}



