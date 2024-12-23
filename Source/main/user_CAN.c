#include "user_CAN.h"


static unsigned char prev_stat_time;
static unsigned short prev_stat_score;
static unsigned char prev_stat_lives;


void interrupt_CAN_write_stat(unsigned char data_array[8]) {
	unsigned char stat_time;
	unsigned short stat_score;
	unsigned char stat_lives;
	
	// Decode
	stat_time = data_array[0];
	stat_lives = data_array[1];
	stat_score = (data_array[2] << 8);
	stat_score |= data_array[3];
	
	// Update
	if(stat_time != prev_stat_time) {
		prev_stat_time = stat_time;
		draw_stat_time(stat_time);
	}
	
	if(stat_score != prev_stat_score) {
		prev_stat_score = stat_score;
		draw_stat_score(stat_score);
	}
	
	if(stat_lives != prev_stat_lives) {
		prev_stat_lives = stat_lives;
		draw_stat_lives(stat_lives);
	}
}

