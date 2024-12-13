#include "pacman_view.h"


/* (Private) Function Prototypes */

void draw_cell(CellType cell_type, Position pos);
void draw_empty(Position pos);
void draw_wall(Position pos);
void draw_standard_pill(Position pos);
void draw_special_pill(Position pos);
void draw_pacman(Position pos);
void draw_full_line(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end, unsigned short color);
void generic_fill_cell(Position pos, unsigned short color, unsigned char fill_style);
unsigned short calc_abs_pos_x(unsigned char coordinate_x);
unsigned short calc_abs_pos_y(unsigned char coordinate_y);


/* (Private) Constants Definitions */


/* (Private) Type Definitions */

typedef enum {
	FULL	= 0,
	BORDER	= 1
} FillStyle;


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
		case EMPTY:
			draw_empty(cell_pos);
			break;
		case WALL:
			draw_wall(cell_pos);
			break;
		case TP_CELL_L:
			draw_empty(cell_pos);
			break;
		case TP_CELL_R:
			draw_empty(cell_pos);
			break;
		case STD_PILL:
			draw_standard_pill(cell_pos);
			break;
		case SPC_PILL:
			draw_special_pill(cell_pos);
			break;
		case PACMAN:
			draw_pacman(cell_pos);
			break;
		default:
			draw_empty(cell_pos);
			break;
	}
}

void draw_empty(Position pos) {
	generic_fill_cell(pos, COL_BLACK, FULL);
}

void draw_wall(Position pos) {
	generic_fill_cell(pos, COL_BLUE, BORDER);
}

void draw_standard_pill(Position pos) {
	unsigned short x_display;
	unsigned short y_display;
	
	x_display = calc_abs_pos_x(pos.x);
	y_display = calc_abs_pos_y(pos.y);
	
	draw_full_line(x_display+3, y_display+3, (x_display + (SCALE-1))-3, (y_display + (SCALE-1))-3, COL_PINK);
}

void draw_special_pill(Position pos) {
}

void draw_pacman(Position pos) {

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

void generic_fill_cell(Position pos, unsigned short color, FillStyle fill_style) {
	unsigned short x_display;
	unsigned short y_display;
	
	x_display = calc_abs_pos_x(pos.x);
	y_display = calc_abs_pos_y(pos.y);
	
	switch(fill_style) {
		case(FULL):
			draw_full_line(x_display, y_display, (x_display + (SCALE-1)), (y_display + (SCALE-1)), color);
			break;
		case(BORDER):
			draw_border_line(x_display, y_display, (x_display + (SCALE-1)), (y_display + (SCALE-1)), color);
			break;
	}
}

unsigned short calc_abs_pos_x(unsigned char coordinate_x) {
	return MAP_START_X + (coordinate_x * SCALE);
}

unsigned short calc_abs_pos_y(unsigned char coordinate_y) {
	return MAP_START_Y + (coordinate_y * SCALE);
}





