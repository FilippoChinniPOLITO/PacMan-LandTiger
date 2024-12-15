#ifndef __PACMAN_CORE_H
#define __PACMAN_CORE_H


/* System Imports */

#include "LPC17xx.h"


/* User Imports */

//


/* Constants Definitions */

//General (Display Related)
#define SCREEN_HEIGTH	320
#define SCREEN_WIDTH	240

#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)
#define SCREEN_CENTER_Y	(SCREEN_HEIGTH / 2)

//Game Map
#define MAP_HEIGTH		31
#define MAP_WIDTH		28

#define SCALE			8

#define PADDING_LEFT	(SCREEN_WIDTH - (MAP_WIDTH * SCALE)) / 2
#define PADDING_RIGHT	(SCREEN_WIDTH - (MAP_WIDTH * SCALE)) / 2

#define PADDING_TOP		8
#define PADDING_BOTTOM	8

#define MAP_START_X		(0 + PADDING_LEFT)
#define MAP_START_Y		(SCREEN_HEIGTH - (MAP_HEIGTH * SCALE) - PADDING_BOTTOM)

//Player GUI
#define DELIMITER_PADDING	8

#define PLAYER_GUI_LENGHT	(SCREEN_WIDTH - (PADDING_TOP + PADDING_BOTTOM + DELIMITER_PADDING + (MAP_HEIGTH * SCALE)))
#define PLAYER_GUI_WIDTH	(SCREEN_WIDTH - (PADDING_LEFT + PADDING_RIGHT))

#define PLAYER_GUI_START_X	(0 + PADDING_LEFT)
#define PLAYER_GUI_START_Y	(0 + PADDING_TOP)

#define TIMER_AREA_START_X	(PLAYER_GUI_START_X + 0)
#define SCORE_AREA_START_X	(TIMER_AREA_START_X + (PLAYER_GUI_WIDTH / 3))
#define LIVES_AREA_START_X	(SCORE_AREA_START_X + (PLAYER_GUI_WIDTH / 3))

#define PLAYER_GUI_HEADER_Y	(PLAYER_GUI_START_Y + 0)

#define TIMER_VALUE_START_Y	(PLAYER_GUI_HEADER_Y + 20)
#define SCORE_VALUE_START_Y	(PLAYER_GUI_HEADER_Y + 20)
#define LIVES_VALUE_START_Y	(PLAYER_GUI_HEADER_Y + 24)

#define TIMER_VALUE_START_X	(TIMER_AREA_START_X + 20)
#define SCORE_VALUE_START_X	(SCORE_AREA_START_X + 16)
#define LIVES_VALUE_START_X	(LIVES_AREA_START_X + 16)


/* Type Definitions */

typedef enum {
	DIRECTION_STILL	= 0,
	DIRECTION_RIGHT	= 1,
	DIRECTION_LEFT	= 2,
	DIRECTION_UP	= 3,
	DIRECTION_DOWN	= 4
} Direction;

typedef enum {
    CELL_EMPTY 		= 0,	// Empty Space
    CELL_WALL 		= 1,	// Wall
    CELL_TP_L 		= 2,	// Teleport Area Left
	CELL_TP_R		= 3,	// Teleport Area Right
    CELL_STD_PILL 	= 4,	// Pill
    CELL_SPC_PILL 	= 5,	// Special Pill
    CELL_PACMAN 	= 6		// PacMan
} CellType;

typedef uint8_t GameMap[MAP_HEIGTH][MAP_WIDTH];

typedef struct {
	unsigned char x;
	unsigned char y;
} Position;

typedef struct {
	Position curr_pos;
	Position prev_pos;
	Direction direction;
	unsigned char animation_frame;
} Pacman;

typedef struct {
	unsigned char map_id;				// ID of Selected Game Map
	unsigned char max_time;				// Start Time
    unsigned char start_lives;			// Number of Life at the start
	unsigned char max_lives;			// Max Number of Lives
    unsigned short pts_to_life;			// Points to gain Extra Life
	unsigned char std_pill_count;		// Number of Standard Pills
    unsigned char spc_pill_count;		// Number of Special Pills
	unsigned short std_pill_value;		// Score Points of a Standard Pill
	unsigned short spc_pill_value;		// Score Points of a Special Pill
} GameConfig;

typedef struct {
	GameMap game_map;			// Game Map
	unsigned char time;			// Current Time left
	unsigned char lives;		// Current Lives of the Player
	unsigned short score;		// Current Score of the Player
	unsigned char pills_left;	// Number of Pills Left
	Pacman pacman;				// PacMan Position and Attributes
	unsigned char is_pause;		// State of the Game (if paused)
} GameRunning;


/* (Public) Functions Prototypes */
	
	
/* Global Variables and Constants */

extern const GameConfig GAME_CONFIG;
extern GameRunning game_run;



#endif //__PACMAN_CORE_H
