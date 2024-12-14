#include "pacman_view.h"


/* (Private) Type Definitions */

typedef enum {
	STYLE_FULL		= 0,
	STYLE_BORDER	= 1,
	STYLE_CIRCLE	= 2,
	STYLE_PACMAN   	= 3
} FillStyle;


/* (Private) Function Prototypes */

void draw_cell(CellType cell_type, Position pos);
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
unsigned short calc_abs_pos_x(unsigned char coordinate_x);
unsigned short calc_abs_pos_y(unsigned char coordinate_y);


/* (Private) Constants Definitions */


/* Function Implementations */

void draw_stat_area() {
	GUI_Text(TIMER_AREA_START_X, PLAYER_GUI_HEADER_Y, (uint8_t*) "  TIME ", COL_WHITE, COL_BLACK);
	GUI_Text(SCORE_AREA_START_X, PLAYER_GUI_HEADER_Y, (uint8_t*) "   SCORE ", COL_WHITE, COL_BLACK);
	GUI_Text(LIVES_AREA_START_X, PLAYER_GUI_HEADER_Y, (uint8_t*) "   LIVES ", COL_WHITE, COL_BLACK);
	
	GUI_Text(TIMER_AREA_START_X, TIMER_VALUE_START_Y, (uint8_t*) "   59 ", COL_WHITE, COL_BLACK);
	GUI_Text(SCORE_AREA_START_X, SCORE_VALUE_START_Y, (uint8_t*) "   1350 ", COL_WHITE, COL_BLACK);
	GUI_Text(LIVES_AREA_START_X, LIVES_VALUE_START_Y, (uint8_t*) "   O O O", COL_WHITE, COL_BLACK);
}

void draw_game_map() {
	unsigned char i;
	unsigned char j;
	CellType temp;
	Position temp_pos;
	
	for(i=0; i < MAP_HEIGTH; i++) {
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
	generic_fill_cell_with_reduction(pos, 2, COL_ORANGE, STYLE_CIRCLE);
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
	
	x_display_end = x_display_start + (SCALE-1) - (reduction*2);
	y_display_end = y_display_start + (SCALE-1) - (reduction*2);
	
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
				LCD_SetPoint(x_start+2, y_start+1, COL_BLACK);
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

unsigned short calc_abs_pos_x(unsigned char coordinate_x) {
	return MAP_START_X + (coordinate_x * SCALE);
}

unsigned short calc_abs_pos_y(unsigned char coordinate_y) {
	return MAP_START_Y + (coordinate_y * SCALE);
}





