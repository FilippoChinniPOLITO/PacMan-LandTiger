#include "pacman_ctrl.h"


/* (Private) System Imports */

#include <stdlib.h>
#include <math.h>


/* (Private) Hardware Imports */

//#include "../interfaces/hw_abstraction.h"		// Transitive Import


/* (Private) User Imports */

#include "pacman_core.h"
#include "pacman_view.h"
#include "pacman_utils.h"
#include "pacman_sound.h"


/* (Private) File-Scope Global Variables */

static unsigned char ghosts_weak_tick_count_reset_flag = 0;


/* (Private) Functions Prototypes */

void handle_soft_level_init();
void handle_pacman_spawn();
void handle_blinky_spawn();
void handle_time_update();
void handle_fail();
void handle_game_over();
void handle_draw_pause();
void handle_pacman_move();
void handle_character_update(Character* character, Position next_pos, unsigned char* prev_cell, unsigned char* next_cell, CharacterType character_type);
Direction calculate_blinky_direction(Character blinky, Character pacman);
unsigned char calculate_special_distance(Position pos1, Position pos2);
unsigned char calculate_manhattan_distance(Position pos1, Position pos2);
unsigned char calculate_euclidean_distance(Position pos1, Position pos2);
Position calculate_next_position(Character character);
void update_character_animation(Character* character);
unsigned char handle_tp(Position* pos, Direction direction);
void handle_pacman_ghost_collision();
void handle_blinky_death();
void handle_blinky_revive();
void handle_pill_eat(CellType pill_type);
void handle_score_update(unsigned short score_increase);
void handle_lives_update();
void handle_special_pill_generation();
void handle_pill_transformation(Position pos);
Position generate_random_position();
void handle_blinky_acceleration();
unsigned char position_equals(Position pos1, Position pos2);
void handle_change_playing_sound(SoundID sound_id);
void handle_draw_stat_time(unsigned char time_value);
void handle_draw_stat_score(unsigned short score_value);
void handle_draw_stat_lives(unsigned char lives_value);
void handle_CAN_send();


/* Functions Implementations */

void handle_level_init() {
	game_status.spc_pills_gen = 0;
	game_run.pills_left = GAME_CONFIG.std_pill_count;
	game_run.time = GAME_CONFIG.max_time;
	
	get_game_map(&game_run.game_map, GAME_CONFIG.map_id);
	
	handle_soft_level_init();
	
	draw_game_map();
	draw_stat_area();
	handle_draw_pause();
	
	game_status.is_init_over = 1;
}

void handle_soft_level_init() {
	handle_pacman_spawn();
	handle_blinky_spawn();
}

void handle_pacman_spawn() {
	game_run.pacman.direction = DIRECTION_STILL;
	game_run.pacman.prev_pos = game_run.pacman.curr_pos;
	game_run.pacman.curr_pos = get_pacman_spawn(GAME_CONFIG.map_id);
}

void handle_blinky_spawn() {
	game_run.blinky.direction = DIRECTION_STILL;
	game_run.blinky.prev_pos = game_run.blinky.curr_pos;
	game_run.blinky.curr_pos = get_blinky_spawn(GAME_CONFIG.map_id);
}

void handle_next_tick() {	
	static unsigned short tick_count;
	static unsigned short blinky_respawn_tick_count;
	static unsigned short ghosts_weak_tick_count;
	
	tick_count++;
	
	if(ghosts_weak_tick_count_reset_flag) {
		ghosts_weak_tick_count_reset_flag = 0;
		ghosts_weak_tick_count = 0;
	}
	
	if(game_status.is_ghosts_weak) {
		ghosts_weak_tick_count++;
		if(ghosts_weak_tick_count >= GAME_TIMINGS.ghost_weak_ticks) {
			ghosts_weak_tick_count = 0;
			game_status.is_ghosts_weak = 0;
		}
	}
	
	else if(game_status.is_blinky_dead) {
		blinky_respawn_tick_count++;
		if(blinky_respawn_tick_count >= GAME_TIMINGS.ghost_respawn_ticks) {
			blinky_respawn_tick_count = 0;
			handle_blinky_revive();
		}
	}
	
	handle_pacman_move();
	
	if((tick_count % GAME_CONFIG.spc_pill_gen_ratio) == 0) {
		handle_special_pill_generation();
	}
	
	if((tick_count % GAME_TIMINGS.blinky_accel_interval_ticks) == 0) {
		handle_blinky_acceleration();
	}
	
	if((tick_count % TICK_SECOND_RATIO) == 0) {
		handle_time_update();
	}
}

void handle_time_update() {
	game_run.time--;
	handle_draw_stat_time(game_run.time);
	
	if(game_run.time == 0) {
		handle_game_over();
	}
}

void handle_fail() {
	if(game_run.lives == 0) {
		handle_game_over();
		return;
	}
	
	handle_change_playing_sound(SOUND_ID_LOSE_LIFE);
	
	HW_TIMER_toggle_timers(1, 1, 0, 0);
	game_status.is_fail = 1;
	
	draw_cell(CELL_EMPTY, game_run.pacman.curr_pos);
	draw_cell(CELL_EMPTY, game_run.blinky.curr_pos);
	
	draw_screen_fail();
	
	game_run.lives--;
	handle_lives_update();
	
	handle_soft_level_init();
}

void handle_game_over() {
	handle_change_playing_sound(SOUND_ID_GAME_OVER);
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
	if(game_status.is_end || !game_status.is_init_over) {
		return;
	}
	
	HW_TIMER_toggle_timers(1, !game_status.is_blinky_dead, 0, 0);
	
	if(game_status.is_fail) {
		game_status.is_fail = 0;
		undraw_screen_pause();
		redraw_after_pause();
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
	
	handle_character_update(&game_run.pacman, next_pos, prev_cell, next_cell, CHAR_PACMAN);
	
	if(*next_cell == CELL_STD_PILL || *next_cell == CELL_SPC_PILL) {
		handle_pill_eat(*next_cell);
		*next_cell = CELL_EMPTY;
	}
	
	if(position_equals(game_run.pacman.curr_pos, game_run.blinky.curr_pos) && !game_status.is_blinky_dead) {
		handle_pacman_ghost_collision();
	}
}

void handle_blinky_move() {
	game_run.blinky.direction = calculate_blinky_direction(game_run.blinky, game_run.pacman);
	Position next_pos = calculate_next_position(game_run.blinky);
	unsigned char* prev_cell = &game_run.game_map[game_run.blinky.curr_pos.y][game_run.blinky.curr_pos.x];
	unsigned char* next_cell = &game_run.game_map[next_pos.y][next_pos.x];
	
	handle_character_update(&game_run.blinky, next_pos, prev_cell, next_cell, CHAR_BLINKY);
	
	if(position_equals(game_run.pacman.curr_pos, game_run.blinky.curr_pos) && !game_status.is_blinky_dead) {
		handle_pacman_ghost_collision();
	}
}

void handle_character_update(Character* character, Position next_pos, unsigned char* prev_cell, unsigned char* next_cell, CharacterType character_type) {
	character->prev_pos = character->curr_pos;
	
	if(*next_cell != CELL_WALL)
		character->curr_pos = next_pos;
	
	update_character_animation(character);
	
	draw_cell(*prev_cell, character->prev_pos);
	
	switch(character_type) {
		case(CHAR_PACMAN):
			draw_pacman(character->curr_pos);
			break;
		case(CHAR_BLINKY):
			draw_blinky(character->curr_pos);
			break;
	}
}

Direction calculate_blinky_direction(Character blinky, Character pacman) {
	const Position candidate_moves[4] = {
		{.x = blinky.curr_pos.x+1,	.y = blinky.curr_pos.y},	//RIGHT
		{.x = blinky.curr_pos.x-1,	.y = blinky.curr_pos.y},	//LEFT
		{.x = blinky.curr_pos.x,	.y = blinky.curr_pos.y-1},	//UP
		{.x = blinky.curr_pos.x,	.y = blinky.curr_pos.y+1},	//DOWN
	};
	
	Direction best_direction = blinky.direction;
	
	unsigned char longest_distance = 0;
	unsigned char shortest_distance = 255;
	
	Position i_pos;
	unsigned char i_distance;
	
	unsigned char i;
	for(i=1; i<=4; i++) {
		i_pos = candidate_moves[i-1];
		
		if(game_run.game_map[i_pos.y][i_pos.x] == CELL_WALL || position_equals(i_pos, blinky.prev_pos)) {
			continue;
		}
		
		//i_distance = calculate_manhattan_distance(i_pos, pacman.curr_pos);
		i_distance = calculate_special_distance(i_pos, pacman.curr_pos);
		
		if(game_status.is_ghosts_weak) {	// Frightened Mode
			if(i_distance > longest_distance) {
				longest_distance = i_distance;
				best_direction = i;
			}
		}
		else {								// Chase Mode
			if(i_distance < shortest_distance) {
				shortest_distance = i_distance;
				best_direction = i;
			}
		}
	}
	game_run.blinky.direction = best_direction;
	return best_direction;
}

unsigned char calculate_special_distance(Position pos1, Position pos2) {
	const float WEIGHT_MANHATTAN = 0.5;
	const float WEIGHT_EUCLIDEAN = 1.0f - WEIGHT_MANHATTAN;
	
	unsigned char manhattan = calculate_manhattan_distance(pos1, pos2);
	unsigned char euclidean = calculate_euclidean_distance(pos1, pos2);
	
	return ((manhattan * WEIGHT_MANHATTAN) + (euclidean * WEIGHT_EUCLIDEAN));
}

unsigned char calculate_manhattan_distance(Position pos1, Position pos2) {
	unsigned char add1 = abs(((short) pos1.x) - ((short) pos2.x));
	unsigned char add2 = abs(((short) pos1.y) - ((short) pos2.y));
	return (add1 + add2);
}

unsigned char calculate_euclidean_distance(Position pos1, Position pos2) {
	short dx = pos1.x - pos2.x;
	short dy = pos1.y - pos2.y;
	return sqrt((dx*dx) + (dy*dy));
}

Position calculate_next_position(Character character) {
	Position next_pos = character.curr_pos;
	
	switch(character.direction) {
		case(DIRECTION_STILL):
			break;
		case(DIRECTION_RIGHT):
			if(handle_tp(&next_pos, character.direction))	break;
			next_pos.x++;
			break;
		case(DIRECTION_LEFT):
			if(handle_tp(&next_pos, character.direction))	break;
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

void update_character_animation(Character* character) {
	if(++character->animation_frame > 3)
		character->animation_frame = 0;
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

void handle_pacman_ghost_collision() {
	if(game_status.is_ghosts_weak) {
		handle_blinky_death();
		handle_score_update(GAME_CONFIG.ghost_eat_value);
	}
	else {
		handle_fail();
	}
}

void handle_blinky_death() {
	handle_change_playing_sound(SOUND_ID_EAT_GHOST);
	
	game_status.is_blinky_dead = 1;
	game_status.is_ghosts_weak = 0;
	
	ghosts_weak_tick_count_reset_flag = 1;
	
	HW_TIMER_disable_timer(1);
}

void handle_blinky_revive() {
	handle_blinky_spawn();
	HW_TIMER_enable_timer(1);
	game_status.is_blinky_dead = 0;
}

void handle_pill_eat(CellType pill_type) {
	if(pill_type == CELL_STD_PILL) {
		handle_score_update(GAME_CONFIG.std_pill_value);
	}
	else if(pill_type == CELL_SPC_PILL) {
		handle_change_playing_sound(SOUND_ID_EAT_SPC_PILL);
		game_status.is_ghosts_weak = 1;
		ghosts_weak_tick_count_reset_flag = 1;
		handle_score_update(GAME_CONFIG.spc_pill_value);
	}
	
	game_run.pills_left--;
	handle_victory();
}

void handle_score_update(unsigned short score_increase) {
	game_run.score += score_increase;
	
	handle_draw_stat_score(game_run.score);
	handle_lives_update();
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

void handle_blinky_acceleration() {
	game_status.blinky_curr_speed++;
	
	HW_TIMER_disable_timer(1);
	HW_TIMER_reset_timer(1);
	HW_TIMER_init_timer(1, 0, 0, 3, speed_to_timer_count(game_status.blinky_curr_speed));
	
	if(!game_status.is_blinky_dead)
		HW_TIMER_enable_timer(1);
}

unsigned char position_equals(Position pos1, Position pos2) {
	return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
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

void handle_change_playing_sound(SoundID sound_id) {
	set_sound_playing(sound_id);
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



