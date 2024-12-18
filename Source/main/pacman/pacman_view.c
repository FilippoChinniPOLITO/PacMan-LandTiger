#include "pacman_view.h"


/* (Private) Type Definitions */

typedef enum {
	STYLE_FULL		= 0,
	STYLE_BORDER	= 1,
	STYLE_CIRCLE	= 2,
	STYLE_PACMAN   	= 3
} FillStyle;


/* (Private) Function Prototypes */

void draw_game_map_portion(unsigned char portion_start, unsigned char portion_end);
void draw_empty(Position pos);
void draw_wall(Position pos);
void draw_standard_pill(Position pos);
void draw_special_pill(Position pos);
void draw_pacman(Position pos);
void draw_full_line(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short color);
void draw_border_line(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short color);
void draw_circle(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short color);
void draw_pacman_model(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short color, Direction orientation, unsigned char animation_frame);
void generic_fill_cell(Position pos, unsigned short color, unsigned char fill_style);
void generic_fill_cell_with_reduction(Position pos, unsigned char reduction, unsigned short color, FillStyle fill_style);
void generic_write(unsigned short x_start, unsigned short y_start, char* string, unsigned short color, unsigned short color_background);
void write_centered(char* string, unsigned short color, unsigned short color_background, unsigned char is_full_width);
void write_centered_set_y(unsigned short start_y, char* string, unsigned short color, unsigned short color_background, unsigned char is_full_width);
unsigned short calc_abs_pos_x(unsigned char coordinate_x);
unsigned short calc_abs_pos_y(unsigned char coordinate_y);
char* convert_int_to_string(unsigned short value);
char* convert_int_to_string_with_padding(unsigned short value, unsigned char padding);
char* center_string_in_spaces(char* string);


/* File-Scope Globale Variables */

static char buffer[32];


/* (Private) Constants Definitions */

#define D_CELL_SIZE					(SCALE)
#define D_CELL_OFFSET				(D_CELL_SIZE-1)

#define CHAR_WIDTH_PIXELS			8
#define CHAR_HEIGTH_PIXELS			16

#define WRITINGS_SCREEN_CENTER_Y	(SCREEN_CENTER_Y - (CHAR_HEIGTH_PIXELS / 2))

#define PAUSE_WRITING_START_Y		9
#define PAUSE_WRITING_END_Y			15


/* Function Implementations */

void draw_screen_pause() {
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y - CHAR_HEIGTH_PIXELS, " ", COL_BLUE, COL_WHITE, 1);
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y, "PAUSE", COL_BLUE, COL_WHITE, 1);
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y + CHAR_HEIGTH_PIXELS, " ", COL_BLUE, COL_WHITE, 1);
}

void undraw_screen_pause() {
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y - CHAR_HEIGTH_PIXELS, " ", COL_BLACK, COL_BLACK, 1);
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y, " ", COL_BLACK, COL_BLACK, 1);
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y + CHAR_HEIGTH_PIXELS, " ", COL_BLACK, COL_BLACK, 1);
}

void draw_screen_victory() {
	LCD_Clear(COL_BLACK);
	write_centered("VICTORY!!!", COL_GREEN, COL_BLACK, 0);
}

void draw_screen_game_over() {
	LCD_Clear(COL_BLACK);
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y - CHAR_HEIGTH_PIXELS, "G A M E", COL_RED, COL_BLACK, 0);
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y, " ", COL_RED, COL_BLACK, 0);
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y + CHAR_HEIGTH_PIXELS, "O V E R", COL_RED, COL_BLACK, 0);
}

void draw_screen_fail() {
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y - CHAR_HEIGTH_PIXELS, "FAIL!", COL_ORANGE, COL_WHITE, 1);
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y, "-1 Life", COL_ORANGE, COL_WHITE, 1);
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y + CHAR_HEIGTH_PIXELS, "Press PAUSE to Continue...", COL_ORANGE, COL_WHITE, 1);
}

void draw_stat_area() {
	generic_write(TIMER_AREA_START_X+16, PLAYER_GUI_HEADER_Y, "TIME", COL_WHITE, COL_BLACK);
	generic_write(SCORE_AREA_START_X+16, PLAYER_GUI_HEADER_Y, "SCORE", COL_WHITE, COL_BLACK);
	generic_write(LIVES_AREA_START_X+16, PLAYER_GUI_HEADER_Y, "LIVES", COL_WHITE, COL_BLACK);
	
	draw_stat_time(game_run.time);
	draw_stat_score(game_run.score);
	draw_stat_lives(game_run.lives);
}

void draw_stat_time(unsigned char time_value) {
	generic_write(TIMER_VALUE_START_X, TIMER_VALUE_START_Y, convert_int_to_string_with_padding(time_value, 2), COL_WHITE, COL_BLACK);
}

void draw_stat_score(unsigned short score_value) {
	generic_write(SCORE_VALUE_START_X, SCORE_VALUE_START_Y, convert_int_to_string_with_padding(score_value, 4), COL_WHITE, COL_BLACK);
}

void draw_stat_lives(unsigned char lives_value) {
	const unsigned char PADDING_LIVES = D_CELL_SIZE / 2;
	unsigned char i;
	for(i=0; i < GAME_CONFIG.max_lives; i++) {
		unsigned short temp = ((D_CELL_SIZE + PADDING_LIVES) * i);
		
		if(i < lives_value)
			draw_pacman_model(LIVES_VALUE_START_X + temp, LIVES_VALUE_START_Y, LIVES_VALUE_START_X + temp + D_CELL_OFFSET, LIVES_VALUE_START_Y + D_CELL_OFFSET, COL_YELLOW, DIRECTION_RIGHT, 0);
		else
			draw_full_line(LIVES_VALUE_START_X + temp, LIVES_VALUE_START_Y, LIVES_VALUE_START_X + temp + D_CELL_OFFSET, LIVES_VALUE_START_Y + D_CELL_OFFSET, COL_BLACK);
	}
}

void draw_game_map() {
	draw_game_map_portion(0, MAP_HEIGTH);
}

void redraw_after_pause() {
	//This presumes that the PAUSE screen will be placed over the Map and will start and end at that exact coordinates
	draw_game_map_portion(PAUSE_WRITING_START_Y, PAUSE_WRITING_END_Y);
}

void draw_game_map_portion(unsigned char portion_start, unsigned char portion_end) {
	unsigned char i;
	unsigned char j;
	CellType temp;
	Position temp_pos;
	
	for(i=portion_start; i < portion_end; i++) {
		for(j=0; j < MAP_WIDTH; j++) {
			temp = (CellType) game_run.game_map[i][j];
			temp_pos = (Position) {.x = j, .y = i};
			draw_cell(temp, temp_pos);
		}
	}
}

void draw_cell(CellType cell_type, Position cell_pos) {
	switch(cell_type) {
		case CELL_EMPTY:
			draw_empty(cell_pos);
			break;
		case CELL_WALL:
			draw_wall(cell_pos);
			break;
		case CELL_TP_L:
			draw_empty(cell_pos);
			break;
		case CELL_TP_R:
			draw_empty(cell_pos);
			break;
		case CELL_STD_PILL:
			draw_empty(cell_pos); 	//This is to fix the "Lingering Spc Pill Bug"; it is bad, but is the easiest fix
			draw_standard_pill(cell_pos);
			break;
		case CELL_SPC_PILL:
			draw_special_pill(cell_pos);
			break;
		case CELL_PACMAN:
			draw_pacman(cell_pos);
			break;
		default:
			draw_empty(cell_pos);
			break;
	}
}

void draw_empty(Position pos) {
	generic_fill_cell(pos, COL_BLACK, STYLE_FULL);
}

void draw_wall(Position pos) {
	generic_fill_cell(pos, COL_BLUE, STYLE_BORDER);
}

void draw_standard_pill(Position pos) {
	generic_fill_cell_with_reduction(pos, 3, COL_PINK, STYLE_CIRCLE);
}

void draw_special_pill(Position pos) {
	generic_fill_cell_with_reduction(pos, 1, COL_LIME, STYLE_CIRCLE);
}

void draw_pacman(Position pos) {
	generic_fill_cell_with_reduction(pos, 1, COL_YELLOW, STYLE_PACMAN);
}

void generic_fill_cell(Position pos, unsigned short color, FillStyle fill_style) {
	generic_fill_cell_with_reduction(pos, 0, color, fill_style);
}

void generic_fill_cell_with_reduction(Position pos, unsigned char reduction, unsigned short color, FillStyle fill_style) {
	unsigned short x_display_start;
	unsigned short y_display_start;
	unsigned short x_display_end;
	unsigned short y_display_end;
	
	x_display_start = calc_abs_pos_x(pos.x) + reduction;
	y_display_start = calc_abs_pos_y(pos.y) + reduction;
	
	x_display_end = x_display_start + D_CELL_OFFSET - (reduction*2);
	y_display_end = y_display_start + D_CELL_OFFSET - (reduction*2);
	
	switch(fill_style) {
		case(STYLE_FULL):
			draw_full_line(x_display_start, y_display_start, x_display_end, y_display_end, color);
			break;
		case(STYLE_BORDER):
			draw_border_line(x_display_start, y_display_start, x_display_end, y_display_end, color);
			break;
		case(STYLE_CIRCLE):
			draw_circle(x_display_start, y_display_start, x_display_end, y_display_end, color);
			break;
		case(STYLE_PACMAN):
			draw_pacman_model(x_display_start, y_display_start, x_display_end, y_display_end, color, game_run.pacman.direction, game_run.pacman.animation_frame);
			break;
	}
}

void draw_full_line(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short color) {
	unsigned short i;
	const short line_width = ((y_start - y_end) * -1) + 1;

	for(i=0; i < line_width; i++) {
		LCD_DrawLine(x_start, (y_start + i), x_end, (y_start + i), color);
	}
}

void draw_border_line(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short color) {
	unsigned short i;
	const short line_width = ((y_start - y_end) * -1) + 1;

	i = 0;
	LCD_DrawLine(x_start, (y_start + i), x_end, (y_start + i), color);
	while(i < line_width-1) {
		LCD_SetPoint(x_start, (y_start + i), color);
		LCD_SetPoint(x_end, (y_start + i), color);
		i++;
	}
	LCD_DrawLine(x_start, (y_start + i), x_end, (y_start + i), color);
}

void draw_circle(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short color) {
	unsigned short i;
	const short radius = ((x_start - x_end) * -1) / 2;
	unsigned char is_even;
	
	is_even = (radius % 2 == 0) ? 1 : 0;
	
	for(i=0; i <= radius; i++) {
		LCD_DrawLine(x_start+i, (y_start + radius - i), x_end-i, (y_start + radius - i), color);
		LCD_DrawLine(x_start+i, (y_start + radius + i + is_even), x_end-i, (y_start + radius + i + is_even), color);
	}
}

void draw_pacman_model(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short color, Direction orientation, unsigned char animation_frame) {
	// Expects the Input to form a 7*7 Square
	// I tried drawing PacMan procedurally, but it is too difficult
	
	animation_frame = (animation_frame >= 2);	//A Frame lasts 2 Ticks (to make the animation more visible, expecially on physical board)
	
	switch(orientation) {
		case(DIRECTION_STILL):
		case(DIRECTION_RIGHT):
			if(animation_frame == 0) {
				LCD_DrawLine(x_start+1, y_start+0, x_end-1, y_start+0, color);
				LCD_DrawLine(x_start+0, y_start+1, x_end-2, y_start+1, color);
				LCD_SetPoint(x_start+2, y_start+1, COL_BLACK);
				LCD_DrawLine(x_start+0, y_start+2, x_end-3, y_start+2, color);
				LCD_DrawLine(x_start+0, y_start+3, x_end-4, y_start+3, color);
				LCD_DrawLine(x_start+0, y_start+4, x_end-3, y_start+4, color);
				LCD_DrawLine(x_start+0, y_start+5, x_end-2, y_start+5, color);
				LCD_DrawLine(x_start+1, y_start+6, x_end-1, y_start+6, color);
			}
			else if(animation_frame == 1) {
				LCD_DrawLine(x_start+1, y_start+0, x_end-1, y_start+0, color);
				LCD_DrawLine(x_start+0, y_start+1, x_end-1, y_start+1, color);
				LCD_SetPoint(x_start+2, y_start+1, COL_BLACK);
				LCD_DrawLine(x_start+0, y_start+2, x_end-2, y_start+2, color);
				LCD_DrawLine(x_start+0, y_start+3, x_end-3, y_start+3, color);
				LCD_DrawLine(x_start+0, y_start+4, x_end-2, y_start+4, color);
				LCD_DrawLine(x_start+0, y_start+5, x_end-1, y_start+5, color);
				LCD_DrawLine(x_start+1, y_start+6, x_end-1, y_start+6, color);
			}
			break;
		case(DIRECTION_LEFT):
			if(animation_frame == 0) {
				LCD_DrawLine(x_start+1, y_start+0, x_end-1, y_start+0, color);
				LCD_DrawLine(x_start+2, y_start+1, x_end-0, y_start+1, color);
				LCD_SetPoint(x_start+4, y_start+1, COL_BLACK);
				LCD_DrawLine(x_start+3, y_start+2, x_end-0, y_start+2, color);
				LCD_DrawLine(x_start+4, y_start+3, x_end-0, y_start+3, color);
				LCD_DrawLine(x_start+3, y_start+4, x_end-0, y_start+4, color);
				LCD_DrawLine(x_start+2, y_start+5, x_end-0, y_start+5, color);
				LCD_DrawLine(x_start+1, y_start+6, x_end-1, y_start+6, color);
			}
			else if(animation_frame == 1) {
				LCD_DrawLine(x_start+1, y_start+0, x_end-1, y_start+0, color);
				LCD_DrawLine(x_start+1, y_start+1, x_end-0, y_start+1, color);
				LCD_SetPoint(x_start+4, y_start+1, COL_BLACK);
				LCD_DrawLine(x_start+2, y_start+2, x_end-0, y_start+2, color);
				LCD_DrawLine(x_start+3, y_start+3, x_end-0, y_start+3, color);
				LCD_DrawLine(x_start+2, y_start+4, x_end-0, y_start+4, color);
				LCD_DrawLine(x_start+1, y_start+5, x_end-0, y_start+5, color);
				LCD_DrawLine(x_start+1, y_start+6, x_end-1, y_start+6, color);
			}
			break;
		case(DIRECTION_UP):
			if(animation_frame == 0) {
				LCD_SetPoint(x_start, y_start+1, COL_YELLOW);
				LCD_SetPoint(x_end, y_start+1, COL_YELLOW);
				LCD_DrawLine(x_start+0, y_start+2, x_start+1, y_start+2, color);
				LCD_DrawLine(x_end-0, y_start+2, x_end-1, y_start+2, color);
				LCD_DrawLine(x_start+0, y_start+3, x_start+2, y_start+3, color);
				LCD_DrawLine(x_end-0, y_start+3, x_end-2, y_start+3, color);
				LCD_DrawLine(x_start+0, y_start+4, x_end-0, y_start+4, color);
				LCD_DrawLine(x_start+0, y_start+5, x_end-0, y_start+5, color);
				LCD_DrawLine(x_start+1, y_start+6, x_end-1, y_start+6, color);
				LCD_SetPoint(x_start+1, y_start+4, COL_BLACK);
			}
			else if(animation_frame == 1) {
				LCD_DrawLine(x_start+0, y_start+1, x_start+1, y_start+1, color);
				LCD_DrawLine(x_end-0, y_start+1, x_end-1, y_start+1, color);
				LCD_DrawLine(x_start+0, y_start+2, x_start+2, y_start+2, color);
				LCD_DrawLine(x_end-0, y_start+2, x_end-2, y_start+2, color);
				LCD_DrawLine(x_start+0, y_start+3, x_end-0, y_start+3, color);
				LCD_DrawLine(x_start+0, y_start+4, x_end-0, y_start+4, color);
				LCD_DrawLine(x_start+0, y_start+5, x_end-0, y_start+5, color);
				LCD_DrawLine(x_start+1, y_start+6, x_end-1, y_start+6, color);
				LCD_SetPoint(x_start+1, y_start+4, COL_BLACK);
			}
			break;
		case(DIRECTION_DOWN):
			if(animation_frame == 0) {
				LCD_DrawLine(x_start+1, y_start+0, x_end-1, y_start+0, color);
				LCD_DrawLine(x_start+0, y_start+1, x_end-0, y_start+1, color);
				LCD_DrawLine(x_start+0, y_start+2, x_end-0, y_start+2, color);
				LCD_DrawLine(x_start+0, y_start+3, x_start+2, y_start+3, color);
				LCD_DrawLine(x_end-0, y_start+3, x_end-2, y_start+3, color);
				LCD_DrawLine(x_start+0, y_start+4, x_start+1, y_start+4, color);
				LCD_DrawLine(x_end-0, y_start+4, x_end-1, y_start+4, color);
				LCD_SetPoint(x_start, y_start+5, COL_YELLOW);
				LCD_SetPoint(x_end, y_start+5, COL_YELLOW);
				LCD_SetPoint(x_end-1, y_start+2, COL_BLACK);
			}
			else if(animation_frame == 1) {
				LCD_DrawLine(x_start+1, y_start+0, x_end-1, y_start+0, color);
				LCD_DrawLine(x_start+0, y_start+1, x_end-0, y_start+1, color);
				LCD_DrawLine(x_start+0, y_start+2, x_end-0, y_start+2, color);
				LCD_DrawLine(x_start+0, y_start+3, x_end-0, y_start+3, color);
				LCD_DrawLine(x_start+0, y_start+4, x_start+2, y_start+4, color);
				LCD_DrawLine(x_end-0, y_start+4, x_end-2, y_start+4, color);
				LCD_DrawLine(x_start+0, y_start+5, x_start+1, y_start+5, color);
				LCD_DrawLine(x_end-0, y_start+5, x_end-1, y_start+5, color);
				LCD_SetPoint(x_end-1, y_start+2, COL_BLACK);
			}
			break;
	}
}

void generic_write(unsigned short x_start, unsigned short y_start, char* string, unsigned short color, unsigned short color_background) {
	GUI_Text(x_start, y_start, (uint8_t*) string, color, color_background);
}

void write_centered(char* string, unsigned short color, unsigned short color_background, unsigned char is_full_width) {
	write_centered_set_y(WRITINGS_SCREEN_CENTER_Y, string, color, color_background, is_full_width);
}

void write_centered_set_y(unsigned short start_y, char* string, unsigned short color, unsigned short color_background, unsigned char is_full_width) {
	if(is_full_width) {
		generic_write(0, start_y, center_string_in_spaces(string), color, color_background);
	}
	else {
		const unsigned short calculated_left_padding = ((SCREEN_WIDTH / CHAR_WIDTH_PIXELS) - strlen(string)) / 2;
		const unsigned short calculated_start_x = (calculated_left_padding + (strlen(string) % 2)) * CHAR_WIDTH_PIXELS;
		generic_write(calculated_start_x, start_y, string, color, color_background);
	}
}

unsigned short calc_abs_pos_x(unsigned char coordinate_x) {
	return MAP_START_X + (coordinate_x * D_CELL_SIZE);
}

unsigned short calc_abs_pos_y(unsigned char coordinate_y) {
	return MAP_START_Y + (coordinate_y * D_CELL_SIZE);
}

char* convert_int_to_string(unsigned short value) {
	memset(buffer, 0, sizeof(&buffer));
	sprintf(buffer, "%d", value);
	return buffer;
}

char* convert_int_to_string_with_padding(unsigned short value, unsigned char padding) {
	unsigned char index;
	unsigned char i;
	
	convert_int_to_string(value);
	
	index = strlen(buffer);
	for(i=0; i < padding; i++)
		buffer[index+i] = ' ';
	
	buffer[index+i] = '\0';
	return buffer;
}

char* center_string_in_spaces(char* string) {
	const unsigned short STR_LEN = strlen(string);
	const unsigned short TOT_STR_PADDING = (SCREEN_WIDTH / 8) - STR_LEN;
	const unsigned short LEFT_STR_PADDING = TOT_STR_PADDING / 2;
	const unsigned short RIGHT_STR_PADDING = TOT_STR_PADDING - LEFT_STR_PADDING;
	const unsigned char is_odd = STR_LEN % 2;
	unsigned char len;
	unsigned char i;
	
	memset(buffer, 0, sizeof(&buffer));
	
	for(i=0; i < (LEFT_STR_PADDING + is_odd); i++) {
		buffer[i] = ' ';
	}
	buffer[i] = '\0';
	
	strcat(buffer, string);
	len = strlen(buffer);
	
	for(i=len; i < (len + RIGHT_STR_PADDING - is_odd); i++) {
		buffer[i] = ' ';
	}
	buffer[i] = '\0';
	
	return buffer;
}



