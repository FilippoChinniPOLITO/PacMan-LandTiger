#ifndef __PACMAN_CORE_H
#define __PACMAN_CORE_H


/* System Imports */

#include "LPC17xx.h"


/* User Imports */

//


/* Constants Definitions */

#define SCREEN_HEIGTH	320
#define SCREEN_WIDTH	240

#define MAP_HEIGTH		31
#define MAP_WIDTH		28

#define SCALE			8

#define PADDING_LEFT	(SCREEN_WIDTH - (MAP_WIDTH * SCALE)) / 2
#define PADDING_RIGHT	(SCREEN_WIDTH - (MAP_WIDTH * SCALE)) / 2

#define MAP_START_Y		(SCREEN_HEIGTH - (MAP_HEIGTH * SCALE))

#define TIMER_AREA_START_X	(0 + PADDING_LEFT)
#define SCORE_AREA_START_X	TIMER_AREA_START_X + ((SCREEN_WIDTH - (PADDING_LEFT + PADDING_RIGHT)) / 3)
#define LIVES_AREA_START_X	SCORE_AREA_START_X + ((SCREEN_WIDTH - (PADDING_LEFT + PADDING_RIGHT)) / 3)

#define SECTIONS_PADDING	SCREEN_WIDTH / 10


/* Type Definitions */

typedef enum {
    EMPTY 		= 0,	// Empty Space
    WALL 		= 1,	// Wall
    TP_CELL_L 	= 2,	// Teleport Area Left
	TP_CELL_R	= 3,	// Teleport Area Right
    STD_PILL 	= 4,	// Pill
    SPC_PILL 	= 5,	// Special Pill
    PACMAN 		= 6		// PacMan
} CellType;

typedef uint8_t GameMap[MAP_HEIGTH][MAP_WIDTH];

typedef struct {
	unsigned char x;
	unsigned char y;
} Position;

typedef struct {
	unsigned char map_id;				// ID of Selected Game Map
	unsigned char max_time;				// Start Time
    unsigned char start_lives;			// Number of Life at the start
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
	Position pacman;			// Current PacMan Position	
} GameRunning;


/* Maps */

static const GameMap MAP_1 = 
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
		{1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1},
        {1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1},
        {1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1},
        {1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1},
        {1, 4, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 4, 1},
        {1, 4, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 4, 1},
        {1, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 1},
        {1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 4, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 4, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 4, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1}, 
        {1, 1, 1, 1, 1, 1, 4, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 4, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 4, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 4, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 4, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 4, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 4, 1, 1, 1, 1, 1, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1},
        {1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1},
        {1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1},
        {1, 4, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 4, 1},
        {1, 1, 1, 4, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 4, 1, 1, 1},
        {1, 1, 1, 4, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 4, 1, 1, 1},
        {1, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 1},
        {1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1},
        {1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};


/* (Public) Functions Prototypes */
	
	
/* Global Variables and Constants */

extern const GameConfig GAME_CONFIG;
extern GameRunning game_run;



#endif //__PACMAN_CORE_H
